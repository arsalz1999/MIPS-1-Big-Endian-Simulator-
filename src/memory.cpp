#include "memory.hpp"
#include <iostream>
#include <fstream>


memory::memory(std::string name_bin){

    ADDR_DATA.resize(0x4000000);
    std::fill(ADDR_DATA.begin(), ADDR_DATA.end(), 0);
    //reading the binary file
    std::ifstream input_file(name_bin.c_str(), std::ifstream::binary);

    if(!input_file.is_open()){
        std::exit(-21);
    }

    //calculating the size of binary file
    input_file.seekg(0, std::ios::end);
    int bin_size = input_file.tellg();
    input_file.seekg(0, std::ios::beg);

    if(bin_size > 0x1000000){
        std::exit(-11);
    }


    //loads the input binaries into an array
    char bin_input[bin_size];
    input_file.read(bin_input, bin_size);
    input_file.close();

    //computes the number of instructions
    total_instructions = sizeof(bin_input)/4;
    
    //loading the binary instructions into instruction memory
    int add_index=0;
    for (int index = 0; index < total_instructions; index++){
        add_index = index * 4;
        uint32_t instruction = ((bin_input[add_index]<<24)&0xFF000000)|((bin_input[add_index+1]<<16)&0x00FF0000)|((bin_input[add_index+2]<<8)&0x0000FF00)|((bin_input[add_index+3])&0x000000FF);
        ADDR_INSTR.push_back(instruction);
    }
}

//helper function for later - reminds me what the cin.good thing does haha
void memory::check_flags(){
  if(!std::cin.good()) std::exit(-21);
}
bool memory::end_of_file(){
  return(end_of_file());
}

uint32_t memory::read_instruction(uint32_t pc){
    if((pc < 0x11000000) && (pc >= 0x10000000) && (pc % 4 == 0)){
        uint32_t pc_position = (pc-0x10000000)/4;
        if(pc_position < ADDR_INSTR.size()) return ADDR_INSTR[pc_position];
        else return 0;
    }
    else std::exit(-11);
}

void memory::store_mem(int pc_position, int32_t to_store){
  if(pc_position==0x30000004){
    char data_output= int8_t(to_store&0xFF);
    if(!std::cout.good()) std::exit(-21);
    std::putchar(data_output);
    return;
  }

  if ((pc_position%4 == 0) && (pc_position>=0x20000000) && (pc_position<0x24000000)){
    uint32_t real_pc_position = (pc_position-0x20000000);
    
    ADDR_DATA[real_pc_position+0] = int8_t((to_store&0xFF000000)>>24);
    ADDR_DATA[real_pc_position+1] = int8_t((to_store&0xFF0000)>>16);
    ADDR_DATA[real_pc_position+2] = int8_t((to_store&0xFF00)>>8);
    ADDR_DATA[real_pc_position+3] = int8_t(to_store&0xFF);
  
  }
  else  std::exit(-11);
}

void memory::store_mem_b(int pc_position, int8_t to_store){
  if((pc_position>=0x30000004)&&(pc_position<0x30000008)){
      char data_output= to_store&0xFF;
      if(!std::cout.good()) std::exit(-21);
      if(pc_position==0x30000007) std::putchar(data_output);
      else std::putchar(0);
      return;
  }

  if ((pc_position>=0x20000000) && (pc_position<0x24000000)){
    uint32_t real_pc_position = (pc_position-0x20000000);
    ADDR_DATA[real_pc_position] = to_store;
  }
  else std::exit(-11);
  
}

void memory::store_mem_hw(int pc_position, int16_t to_store){
  if((pc_position==0x30000004) || (pc_position==0x30000006)){
    char data_output= int8_t(to_store&0xFF);
    if(!std::cout.good()) std::exit(-21);
    if(pc_position==0x30000006)   std::putchar(data_output);
    else std::putchar(0);
    return;
  }

  if ((pc_position>=0x20000000) && (pc_position<0x24000000) && (pc_position%2==0)){
    uint32_t real_pc_position = (pc_position-0x20000000);
    ADDR_DATA[real_pc_position] = int8_t((to_store&0xFF00)>>8);
    ADDR_DATA[real_pc_position+1] = int8_t((to_store&0xFF));
  }
  else std::exit(-11);
}

int32_t memory::load_mem(int pc_position){
  if(pc_position==0x30000000){
    int data_input = std::getchar();
    if(std::cin.eof()) return 0xFFFFFFFF;
    check_flags();
    return (data_input);
  }
  if((pc_position % 4 == 0) && (pc_position >= 0x20000000) && (pc_position < 0x24000000)){
    uint32_t real_pc_position = (pc_position-0x20000000);
    return (((int32_t(ADDR_DATA[real_pc_position]<<24))&0xFF000000)|((int32_t(ADDR_DATA[real_pc_position+1]<<16))&0x00FF0000)|((int32_t(ADDR_DATA[real_pc_position+2])<<8)&0x0000FF00)|(int32_t(ADDR_DATA[real_pc_position+3])&0x000000FF));
  }
  else std::exit(-11);
  
}

int32_t memory::load_mem_s_b(int pc_position){
  if((pc_position>=0x30000000)&&(pc_position<0x30000004)){
    char data_input = std::getchar();
    if(end_of_file()) return -1;
    
    check_flags();
    
    if(pc_position==0x30000003) return int32_t(data_input);
    return 0x00000000;
  }

  if((pc_position >= 0x20000000) && (pc_position < 0x24000000)){
    uint32_t real_pc_position = (pc_position-0x20000000);
    int result = (0x000000FF & ADDR_DATA[real_pc_position]);
    int  bitmask = 0x00000080;
    if(bitmask & ADDR_DATA[real_pc_position]) result = result + 0xFFFFFF00;
    return result;
  }
  else std::exit(-11);
}

uint32_t memory::load_mem_u_b(int pc_position){
  if((pc_position >= 0x30000000) && (pc_position < 0x30000004)){
    char data_input = std::getchar();
    if(end_of_file()) return 0x000000FF;
    check_flags();
    if(pc_position==0x30000003) return (int32_t(data_input)&0x000000FF);
    return 0;
  }
  if((pc_position >= 0x20000000) && (pc_position < 0x24000000)){
    uint32_t real_pc_position = (pc_position - 0x20000000);
    return uint32_t(int32_t(ADDR_DATA[real_pc_position])&0x000000FF);
  }
  else std::exit(-11);
}

int32_t memory::load_mem_hw(int pc_position){
  if((pc_position==0x30000000) || (pc_position==0x30000002)){
    char data_input = std::getchar();
    if(end_of_file()) return 0xFFFFFFFF;
    check_flags();
    if(pc_position==0x30000002) return (int32_t)(data_input) & 0xFF;
    return 0;
  }
  if((pc_position >= 0x20000000) && (pc_position < 0x24000000) && (pc_position % 2 == 0)){
    uint32_t real_pc_position = (pc_position-0x20000000);
    int16_t sign_ext_halfword = (((ADDR_DATA[real_pc_position]<<8)&0xFF00)|(ADDR_DATA[real_pc_position+1]&0xFF));
    return (int32_t)sign_ext_halfword;
  }
  else std::exit(-11);
}

uint32_t memory::load_mem_uhw(int pc_position){
  if((pc_position==0x30000000) || (pc_position==0x30000002)){
    char data_input = std::getchar();
    if(end_of_file()) return 0x0000FFFF;
    check_flags();
    if(pc_position==0x30000002) return (int32_t(data_input)&0x000000FF);
    return 0;
  }
  if((pc_position >= 0x20000000) && (pc_position < 0x24000000) && (pc_position % 2 == 0)){
    uint32_t real_pc_position = (pc_position-0x20000000);
    return (int32_t((int16_t(ADDR_DATA[real_pc_position]<<8)&0xFF00)|(int16_t(ADDR_DATA[real_pc_position+1])&0xFF))&0xFFFF);
  }
  else std::exit(-11);
}

int32_t memory::load_mem_wr(int pc_position){
  if(pc_position >= 0x30000000 && pc_position < 0x30000004){
    int data_input = std::getchar();
    if(end_of_file()) return 0xFFFFFFFF;
    check_flags();
    if(pc_position==0x30000003) return (data_input);
    else return 0;
  }
  if((pc_position >= 0x20000000) && (pc_position < 0x24000000)){
    uint32_t real_pc_position = (pc_position-0x20000000);
    switch (real_pc_position % 4) {
      case 0: return(int32_t(ADDR_DATA[real_pc_position])&0x000000FF); break;
      case 1: return(int32_t(((ADDR_DATA[real_pc_position-1]<<8)&0x0000FF00)|(ADDR_DATA[real_pc_position]&0x000000FF))); break;
      case 2: return(int32_t(((ADDR_DATA[real_pc_position-2]<<16)&0x00FF0000)|((ADDR_DATA[real_pc_position-1]<<8)&0x0000FF00)|(ADDR_DATA[real_pc_position]&0x000000FF))); break;
      case 3: return(int32_t(((ADDR_DATA[real_pc_position-3]<<24)&0xFF000000)|((ADDR_DATA[real_pc_position-2]<<16)&0x00FF0000)|((ADDR_DATA[real_pc_position-1]<<8)&0x0000FF00)|(ADDR_DATA[real_pc_position]&0x000000FF))); break;
    }
  }
  else std::exit(-11);
}

int32_t memory::load_mem_wl(int pc_position){
  if(pc_position >= 0x30000000 && pc_position < 0x30000004){
    int data_input = std::getchar();
    if(end_of_file()) return 0xFFFFFFFF;
    check_flags();
    if(pc_position == 0x30000000) return (data_input);
    else return 0;
  }
  if((pc_position < 0x24000000) && (pc_position >= 0x20000000)){
    uint32_t real_pc_position = (pc_position - 0x20000000);
    switch (real_pc_position % 4){
      case 0: return (((ADDR_DATA[real_pc_position]<<24)&0xFF000000)|((ADDR_DATA[real_pc_position+1]<<16)&0x00FF0000)|((ADDR_DATA[real_pc_position+2]<<8)&0x0000FF00)|(ADDR_DATA[real_pc_position+3]&0x000000FF)); break;
      case 1: return (int32_t(((ADDR_DATA[real_pc_position]<<24)&0xFF000000)|((ADDR_DATA[real_pc_position+1]<<16)&0x00FF0000)|((ADDR_DATA[real_pc_position+2]<<8)&0x0000FF00))&0xFFFFFF00); break;
      case 2: return (int32_t(((ADDR_DATA[real_pc_position]<<24)&0xFF000000)|((ADDR_DATA[real_pc_position+1]<<16)&0x00FF0000))&0xFFFF0000); break;
      case 3: return(int32_t(ADDR_DATA[real_pc_position]<<24)&0xFF000000); break;
      }
  }
  else std::exit(-11);
}

int8_t memory::load_instr_b(int pc_position){
  int offset = pc_position % 4;
  uint32_t real_pc_position = (pc_position - offset - 0x10000000)/4;
  if(real_pc_position<ADDR_INSTR.size()){
    switch (offset) {
      case 0: return((ADDR_INSTR[real_pc_position]&0xFF000000)>>24); break;
      case 1: return((ADDR_INSTR[real_pc_position]&0x00FF0000)>>16); break;
      case 2: return((ADDR_INSTR[real_pc_position]&0x0000FF00)>>8); break;
      case 3: return(ADDR_INSTR[real_pc_position]&0x000000FF); break;
    }
  }
  else return 0;

}

int16_t memory::load_instr_hw(int pc_position){
  int offset = pc_position % 4;
  uint32_t real_pc_position = (pc_position - offset - 0x10000000)/4;
  if(ADDR_INSTR.size() > real_pc_position){
    switch (offset) {
      case 0: return((ADDR_INSTR[real_pc_position]&0xFFFF0000)>>16); break;
      case 2: return(ADDR_INSTR[real_pc_position]&0x0000FFFF); break;
    }
  }
  else return 0;

}





