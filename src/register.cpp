#include "register.hpp"
#include <iostream>

registers::registers(){
    std::fill(register_vector, register_vector + 32, 0);
}

void registers::write_register(int current, uint32_t data){

    if(current == 0) register_vector[current] = 0;

    else if((current > 0) && (current < 32)) register_vector[current] = data;

    else std::exit(-11);
}

uint32_t registers::read_register(int current){
    if(current >= 0 && current < 32) return register_vector[current];
    else std::exit(-11);
}
