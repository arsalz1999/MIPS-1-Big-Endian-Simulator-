

int main(){
  if((register.pc >= 0x10000000) && (register.pc < 0x11000000)){ //Checks if pc is pointing to instr memory
    uint32_t instruction = memory.readinstruction(register.pc) //reads in current instruction
    uint16_t opcode = instruction >> 26; // shifts right by 26 to extract the 6 bit opcode
    if(opcode&0b111111 == 0){  // 0 = opcode for Rtype
      executeRtype();
    }
    else if(opcode&0b111111 <=3){ // 1,2,3 = opcode for Jtype
      executeJtype();
    }
    else{  //otherwise it must be Itype
      executeItype();
    }
    register.pc = register.pc + 4; //increment PC
  }
  else{
    std::exit(-11);
  }
}
