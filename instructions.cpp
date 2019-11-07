

int main(){
  if((register.pc >= 0x10000000) && (register.pc < 0x11000000)){ //Checks if pc is pointing to instr memory
    uint32_t instruction = memory.readinstruction(register.pc) //reads in current instruction
    uint16_t opcode = instruction >> 26; // shifts right by 26 to extract the 6 bit opcode
    if(opcode&0b111111 == 0){  // 0 = opcode for Rtype
      executeRtype();
    }
    else if(opcode&0b111111=3 || opcode&0b111111=2 ){ // 2,3 = opcode for Jtype
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


  //executeRtype

  uint16_t funct = instruction & 0b111111;
  uint16_t shamt = (instruction>>6) & 0b11111;
  uint16_t rd = (instruction>>11) & 0b11111;
  uint16_t rt = (instruction>>16) & 0b11111;
  uint16_t rs = (instruction>>21) & 0b11111;
  uint32_t op1 = register.read(rs);
  int32_t s_op1 = register.read(rs);
  uint32_t op2 = register.read(rt);
  int32_t s_op2 = register.read(rt);

  //ADD CASE 32
  if((s_op1>0 && s_op2>0 && s_op1+s_op2<=0) || (s_op1<0 && s_op2<0 && s_op1+s_op2>=0)){
    exit(-10);
  }
  else{
    register.write(rd, (s_op1 + s_op2));
  }


  //ADDU CASE 33
  register.write(rd, (op1 + op2));

  //SUB CASE 34
  if((s_op1>=0 && s_op2<0 && s_op1-s_op2<=0) || (s_op1<0 && s_op2>0 && s_op1-s_op2>=0)){
    exit(-10);
  }
  else{
    register.write(rd, (s_op1 - s_op2));
  }

  //SUBU CASE 35
  register.write(rd, (op1 - op2));

  //AND CASE 36
  register.write(rd,(op1&op2));

  //OR CASE 37
  register.write(rd,(op1|op2));

  //XOR CASE 38
  register.write(rd,(op1^op2));

  //SLL CASE 0
  register.write(rd,(op2<<shamt));

  //SRL CASE 2
  register.write(rd,(op2>>shamt));

  //SRA CASE 3
  register.write(rd,(s_op2>>shamt));

  //SLLV CASE 4
  register.write(rd,(op2<<op1));

  //SRLV CASE 6
  register.write(rd,(op2>>op1));

  //SRAV CASE 7
  register.write(rd,(s_op2>>op1));

  //SLT CASE 42
  if(s_op1<s_op2){
    register.write(rd,1);
  }
  else{
    register.write(rd,0);
  }

  //SLTU CASE 43
  if(op1<op2){
    register.write(rd,1);
  }
  else{
    register.write(rd,0);
  }

  //MFHI CASE 16
  register.write(rd,register.hi);

  //MTHI CASE 17
  register.hi = op1;

  //MFLO CASE 18
  register.write(rd,register.lo);

  //MTLO CASE 19
  register.lo = op1;

  //JR CASE 8
  uint32_t op1copy = op1;
  register.pc = register.pc + 4;
  execute_instruction();
  register.pc = op1copy;

  //JALR CASE 9
  uint32_t op1copy = op1;
  register.pc = register.pc + 4;
  execute_instruction();
  register.write(rd,(register.pc+4));
  register.pc = op1copy;

