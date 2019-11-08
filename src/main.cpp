#include "compiler.hpp"
#include <string>
#include <sstream>
#include <iostream>

//program takes in binary in the command prompt

int main(int argc, char* argv[]){

    std::string name_bin;
    if(argc>1) name_bin = argv[1];
    else std::exit(-21);

    compiler comp(name_bin);
    comp.run();

    return 0;

}
