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
    total_instructions = bin_size/4;
    std::cout << "The memory class works with the makefile" << std::endl;
    std::cout << "the number of instructions are : " << total_instructions << std::endl;
}

uint32_t memory::read_instruction(uint32_t pc){
    if((pc >= 0x10000000) && (pc < 0x11000000) && (pc % 4 == 0)){
        uint32_t pc_position = 0.25*(pc-0x10000000);
        if(pc_position < ADDR_INSTR.size()) return ADDR_INSTR[pc_position];
        else return 0;
    }
    else std::exit(-11);
}
