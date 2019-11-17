#include "simulate.hpp"
#include <iostream>

simulate::simulate(std::string binaryfile) : mem(binaryfile){

}

//all the functions go here

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//R type
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void simulate::execute(){
    if((register_map.program_counter >= 0x10000000) && (register_map.program_counter < 0x11000000)){
    uint32_t instruction_current = mem.read_instruction(register_map.program_counter);
    opcode = (instruction_current & 0xFC000000) >> 26;
    if((opcode & 0b111111) == 0){
      execute_R(instruction_current);
    }
    else if( (opcode & 0b111111) == 3 || (opcode & 0b111111) == 2 ){
      execute_J(instruction_current);
    }
    else{
      execute_I(instruction_current);
    }
    register_map.program_counter += 4; //increment PC
  }
  else{
    std::exit(-11);
  }
}

void simulate::run(){
  while((register_map.program_counter != 0) && ((register_map.program_counter >= 0x10000000) && (register_map.program_counter <= 0x11000000)))
  {
    simulate::execute();
  }
  if(register_map.program_counter==0)
  {
    uint8_t exitCode = (register_map.read_register(2)&0x000000FF);
    std::cerr<<"exitcode is = "<< static_cast<int16_t>(exitCode) <<std::endl;
    std::exit(exitCode);
  }
  else std::exit(-11);
}

void simulate::execute_R(uint32_t instruction){

    funct = instruction & 0b111111;
    shamt = (instruction>>6) & 0b11111;
    rd = (instruction>>11) & 0b11111;
    rt = (instruction>>16) & 0b11111;
    rs = (instruction>>21) & 0b11111;

    op1 = register_map.read_register(rs);
    op2 = register_map.read_register(rt);
    op1_s = register_map.read_register(rs);
    op2_s = register_map.read_register(rt);

    switch(funct){

    case 32: ADD();  break;
    case 33: ADDU(); break;
    case 36: AND();  break;
    case 26: DIV();  break;
    case 27: DIVU(); break;
    case 9:  JALR(); break;
    case 8:  JR();   break;
    case 16: MFHI(); break;
    case 18: MFLO(); break;
    case 17: MTHI(); break;
    case 19: MTLO(); break;
    case 24: MULT(); break;
    case 25: MULTU();break;
    case 37: OR();   break;
    case 0:  SLL();  break;
    case 4:  SLLV(); break;
    case 42: SLT();  break;
    case 43: SLTU(); break;
    case 3:  SRA();  break;
    case 7:  SRAV(); break;
    case 2:  SRL();  break;
    case 6:  SRLV(); break;
    case 34: SUB();  break;
    case 35: SUBU(); break;
    case 38: XOR();  break;
  }
}

void simulate::execute_J(uint32_t instruction){
  target_address = instruction & 0x03FFFFFFF;
  if(opcode == 2){
    J();
  }
  else{
    JAL();
  }
}

void simulate::execute_I(uint32_t instruction){

  immediate = instruction & 0xFFFF;
  ext_immediate = instruction & 0xFFFF;
  rt = (instruction>>16) & 0b11111;
  rs = (instruction>>21) & 0b11111;

  op1 = register_map.read_register(rs);
  op1_s = register_map.read_register(rs);
  op2 = register_map.read_register(rt);
  op2_s = register_map.read_register(rt);

}


void simulate::ADD(){
    if((op1_s>0 && op2_s>0 && op1_s+op2_s<=0) || (op1_s<0 && op2_s<0 && op1_s+op2_s>=0)){
        std::exit(-10);
    }
    else{
      register_map.write_register(rd, (op1_s + op2_s));
    }
}


void simulate::ADDU(){
  register_map.write_register(rd, (op1 + op2));
}

void simulate::SUB(){
  if((op1_s>0 && op2_s<0 && op1_s-op2_s<=0) || (op1_s<0 && op2_s>0 && op1_s-op2_s>=0)){
    std::exit(-10);
  }
  else{
    register_map.write_register(rd, (op1_s - op2_s));
  }
}

void simulate::SUBU(){
  register_map.write_register(rd, (op1 - op2));
}

void simulate::AND(){
  register_map.write_register(rd,(op1&op2));
}

void simulate::OR(){
  register_map.write_register(rd,(op1|op2));
}

void simulate::XOR(){
    register_map.write_register(rd,(op1^op2));
}

void simulate::SLL(){
    register_map.write_register(rd,(op2<<shamt));
}

void simulate::SRL(){
    register_map.write_register(rd,(op2>>shamt));
}

void simulate::SRA(){
    register_map.write_register(rd,(op2_s>>shamt));
}

void simulate::SLLV(){
    register_map.write_register(rd,(op2<<op1));
}

void simulate::SRLV(){
    register_map.write_register(rd,(op2>>op1));
}

void simulate::SRAV(){
    register_map.write_register(rd,(op2_s>>op1));
}


void simulate::SLT(){
    if(op1_s<op2_s) register_map.write_register(rd,1);
    else register_map.write_register(rd,0);
}


void simulate::SLTU(){
     if(op1<op2) register_map.write_register(rd,1);
    else register_map.write_register(rd,0);
}

void simulate::MFHI(){
    register_map.write_register(rd,register_map.hi);
}

void simulate::MTHI(){
    register_map.hi = op1;
}


void simulate::MFLO(){
    register_map.write_register(rd,register_map.lo);
}

void simulate::MTLO(){
    register_map.lo = op1;
}

void simulate::JR(){
    uint32_t op1copy = op1;
    register_map.program_counter += 4;
    execute();
    register_map.program_counter = op1copy;
}

void simulate::JALR(){
    uint32_t op1copy = op1;
    register_map.program_counter += 4;
    register_map.write_register(rd,(register_map.program_counter+4));
    execute();
    register_map.program_counter = op1copy;
}

void simulate::DIV(){
  if(op2_s==0){
    std::exit(-10);
  }
  else{
    register_map.hi = op1_s / op2_s;
    register_map.lo = op1_s % op2_s;
  }

}

void simulate::DIVU(){
  if(op2==0){
    std::exit(-10);
  }
  else{
    register_map.hi = op1 / op2;
    register_map.lo = op1 % op2;
  }
}

void simulate::MULT(){
  int64_t product = int64_t(op1_s) * int64_t(op2_s)
  register_map.lo = product&0xFFFFFFFF;
  register_map.hi = (product>>32)&0xFFFFFFFF;
}

void simulate::MULTU(){
  uint64_t product = int64_t(op1) * int64_t(op2);
  register_map.lo = product&0xFFFFFFFF;
  register_map.hi = (product>>32)&0xFFFFFFFF;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//I type
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Opcode = 8
void simulate::ADDI(){
  if((op1_s>0 && ext_immediate>0 && op1_s+ext_immediate<=0) || (op1_s<0 && ext_immediate<0 && op1_s+ext_immediate>=0)){
    std::exit(-10);
  }
  else{
    register_map.write_register(rt, (op1_s + ext_immediate));
  }
}

//Opcode = 9
void simulate::ADDIU(){
  register_map.write_register(rt, (op1_s + ext_immediate));
}

//Opcode = 10
void simulate::SLTI(){
  if(op1_s<ext_immediate) register_map.write_register(rt,1);
  else register_map.write_register(rt,0);
}

//Opcode = 11
void simulate::SLTIU(){
  if(op1<uint32_t(immediate)) register_map.write_register(rt,1);
  else register_map.write_register(rt,0);
}

//Opcode = 12
void simulate::ANDI(){
  register_map.write_register(rt,(op1&immediate));
}

//Opcode = 13
void simulate::ORI(){
  register_map.write_register(rt,(op1|immediate));
}

//Opcode = 14
void simulate::XORI(){
  register_map.write_register(rt,(op1^immediate));
}

//Opcode = 15
void simulate::LUI(){
  register_map.write_register(rt, uint32_t(immediate)<<16);
}

//Opcode = 4
void simulate::BEQ(){
  if(s_op1==s_op2){
    int32_t offset = immediate;
    register_map.program_counter += 4;
    int32_t pc_copy = register_map.program_counter;
    execute();
    register_map.program_counter = pc_copy + (offset<<2);
  }
}

//Opcode = 5
void simulate::BNE(){
  if(s_op1!=s_op2){
    int32_t offset = immediate;
    register_map.program_counter += 4;
    int32_t pc_copy = register_map.program_counter;
    execute();
    register_map.program_counter = pc_copy + (offset<<2);
  }
}

//Opcode = 6
void simulate::BLEZ(){
  if(s_op1<=0){
    int32_t offset = immediate;
    register_map.program_counter += 4;
    int32_t pc_copy = register_map.program_counter;
    execute();
    register_map.program_counter = pc_copy + (offset<<2);
  }
}

//Opcode = 7
void simulate::BGTZ(){
  if(s_op1>0){
    int32_t offset = immediate;
    register_map.program_counter += 4;
    int32_t pc_copy = register_map.program_counter;
    execute();
    register_map.program_counter = pc_copy + (offset<<2);
  }
}


//opcode = 1 for these 4 BRANCHES

//opcode= 1 && rt = 0x0
void simulate::BLTZ(){
  if(s_op1<0){
    int32_t offset = immediate;
    register_map.program_counter += 4;
    int32_t pc_copy = register_map.program_counter;
    execute();
    register_map.program_counter = pc_copy + (offset<<2);
  }
}
//opcode= 1 && rt = 0x1
void simulate::BGEZ(){
  if(s_op1>=0){
    int32_t offset = immediate;
    register_map.program_counter += 4;
    int32_t pc_copy = register_map.program_counter;
    execute();
    register_map.program_counter = pc_copy + (offset<<2);
  }
}

//opcode= 1 && rt = 0x10
void simulate::BLTZAL(){
  if(s_op1<=0){
    int32_t offset = immediate;
    register_map.program_counter += 4;
    register_map.write_register(31,(register_map.program_counter+4));
    int32_t pc_copy = register_map.program_counter;
    execute();
    register_map.program_counter = pc_copy + (offset<<2);
  }
}

//opcode= 1 && rt = 0x11
void simulate::BGEZAL(){
  if(s_op1>=0){
    int32_t offset = immediate;
    register_map.program_counter += 4;
    register_map.write_register(31,(register_map.program_counter+4));
    int32_t pc_copy = register_map.program_counter;
    execute();
    register_map.program_counter = pc_copy + (offset<<2);
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//J type
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//opcode= 2
void simulate::J(){
    register_map.program_counter += 4;
    int32_t pc_copy = register_map.program_counter;
    execute();
    register_map.program_counter = ((pc_copy&0xF000000)|(target_address<<2));
}

//opcode= 3
void simulate::JAL(){
    uint32_t op1copy = op1;
    register_map.program_counter += 4;
    int32_t pc_copy = register_map.program_counter;
    register_map.write_register(31,(register_map.program_counter+4));
    execute();
    register_map.program_counter = ((pc_copy&0xF000000)|(target_address<<2));
}
