#include "simulator.hpp"
#include <iostream>

simulator::simulator(std::string binaryfile) : mem(binaryfile), register_map(){

}

//all the functions go here

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//R type
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void simulator::execute(){
    if((register_map.program_counter >= 0x10000000) && (register_map.program_counter < 0x11000000)){
    uint32_t instruction_current = mem.read_instruction(register_map.program_counter);
    opcode = (instruction_current & 0xFC000000) >> 26;
    std::cout << "pc is " << register_map.program_counter << std::endl;
    if((opcode & 0b111111) == 0){
      std::cout<<"executed R type"<<std::endl;
      execute_R(instruction_current);
    }
    else if( (opcode & 0b111111) == 3 || (opcode & 0b111111) == 2 ){
      std::cout<<"executed J type"<<std::endl;
      execute_J(instruction_current);
    }
    else{
      std::cout<<"executed I type"<<std::endl;
      execute_I(instruction_current);
    }
    register_map.program_counter += 4; //increment PC
  }
  else{
    std::cout<<"here";
    std::exit(-11);
  }
}

void simulator::run(){
  while((register_map.program_counter != 0) && ((register_map.program_counter >= 0x10000000) && (register_map.program_counter <= 0x11000000)))
  {
    std::cout << "execute called" << std::endl;
    simulator::execute();
  }
  if(register_map.program_counter==0)
  {
    uint8_t exitCode = (register_map.read_register(2)&0x000000FF);
    std::cerr<<"exitcode is = "<< static_cast<int16_t>(exitCode) <<std::endl;
    std::exit(exitCode);
  }
  else { std::cout<<"here1"; std::exit(-11);}
}

void simulator::execute_R(uint32_t instruction){

    funct = instruction & 0b111111;
    shift_amount = (instruction >> 6 & 0b11111);
    rd = (instruction >>11 & 0b11111);
    rt = (instruction >>16 & 0b11111);
    rs = (instruction>>21 & 0b11111);

    std::cout << "funct is "<<funct << std::endl;


    op1 = register_map.read_register(rs);
    op2 = register_map.read_register(rt);
    op1_s = register_map.read_register(rs);
    op2_s = register_map.read_register(rt);

    switch(funct){

    case 32: ADD(rd,op1_s,op2_s);  break;
    case 33: ADDU(rd,op1,op2); break;
    case 36: AND(rd,op1,op2);  break;
    case 26: DIV(op1_s,op2_s);  break;
    case 27: DIVU(op1,op2); break;
    case 9:  JALR(rd,op1_s); break;
    case 8:  JR(op1_s);   break;
    case 16: MFHI(rd); break;
    case 18: MFLO(rd); break;
    case 17: MTHI(op1); break;
    case 19: MTLO(op1); break;
    case 24: MULT(op1_s,op2_s); break;
    case 25: MULTU(op1,op2);break;
    case 37: OR(rt,op1,op2);   break;
    case 0:  SLL(rd,op2,shift_amount);  break;
    case 4:  SLLV(rd,op1,op2); break;
    case 42: SLT(rd,op1_s,op2_s);  break;
    case 43: SLTU(rd,op1,op2); break;
    case 3:  SRA(rd,op2_s,shift_amount);  break;
    case 7:  SRAV(rd,op1,op2_s); break;
    case 2:  SRL(rd,op2,shift_amount);  break;
    case 6:  SRLV(rd,op1,op2); break;
    case 34: SUB(rd,op1_s,op2_s);  break;
    case 35: SUBU(rd,op1,op2); break;
    case 38: XOR(rd,op1,op2);  break;
    default: std::exit(-12);
  }
}

void simulator::execute_J(uint32_t instruction){
  target_address = instruction & 0x03FFFFFFF;
  if(opcode == 2){
    J(target_address);
  }
  else{
    JAL(target_address);
  }
}

void simulator::execute_I(uint32_t instruction){
  opcode = (instruction & 0xFC000000) >> 26;
  u_immediate = instruction & 0xFFFF;
  // immediate = instruction & 0xFFFF;
  imm_16 = instruction & 0xFFFF;
  immediate = imm_16;
  ext_immediate = instruction & 0xFFFF;
  rt = (instruction>>16) & 0b11111;
  rs = (instruction>>21) & 0b11111;

  op1 = register_map.read_register(rs);
  op1_s = register_map.read_register(rs);
  op2 = register_map.read_register(rt);
  op2_s = register_map.read_register(rt);

  if(opcode == 1){
    if(rt==0){
      BLTZ(op1_s,imm_16);
    }
    else if(rt==0x01){
      BGEZ(op1_s,imm_16);
    }

    else if(rt==0x10){
      BLTZAL(op1_s,imm_16);
    }

    else if(rt==0x11){
      BGEZAL(op1_s,imm_16);
    }

    else{
      std::exit(-12);
    }

  }
    switch(opcode){

    case 8: ADDI(rt,op1_s,immediate);  break;
    case 9: ADDIU(rt,op1_s,immediate); break;
    case 10: SLTI(rt,op1_s,immediate);  break;
    case 11: SLTIU(rt,op1,u_immediate);  break;
    case 12: ANDI(rt,op1,immediate); break;
    case 13: ORI(rt,op1,immediate); break;
    case 14: XORI(rt,op1,immediate); break;
    case 15: LUI(rt,u_immediate); break;
    case 4: BEQ(op1_s,op2_s,imm_16); break;
    case 5: BNE(op1_s,op2_s,imm_16); break;
    case 6: BLEZ(op1_s,imm_16); break;
    case 7: BGTZ(op1_s,imm_16); break;

    //Memory instructions
    case 32: LB(); break;
    case 33: LH(); break;
    case 34: LWL(); break;
    case 35: LW(); break;
    case 36: LBU(); break;
    case 37: LHU(); break;
    case 38: LWR(); break;
    case 40: SB(); break;
    case 41: SH(); break;
    case 43: SW(); break;
    default: std::exit(-12);
    }
}


void simulator::ADD(uint16_t& dest_reg, int32_t& operand1, int32_t& operand2){
    if((operand1>0 && operand2>0 && operand1+operand2<=0) || (operand1<0 && operand2<0 && operand1+operand2>=0)){
        std::exit(-10);
    }
    else{
      register_map.write_register(dest_reg, (operand1 + operand2));
    }
}

void simulator::ADDU(uint16_t& dest_reg, uint32_t& operand1, uint32_t& operand2){
  register_map.write_register(dest_reg, (operand1 + operand2));
}

void simulator::SUB(uint16_t& dest_reg, int32_t& operand1, int32_t& operand2){
  if((operand1>0 && operand2<0 && operand1-operand2<=0) || (operand1<0 && operand2>0 && operand1-operand2>=0)){
    std::exit(-10);
  }
  else{
    register_map.write_register(dest_reg, (operand1 - operand2));
  }
}

void simulator::SUBU(uint16_t& dest_reg, uint32_t& operand1, uint32_t& operand2){
  register_map.write_register(dest_reg, (operand1 - operand2));
}

void simulator::AND(uint16_t& dest_reg, uint32_t& operand1, uint32_t& operand2){
  register_map.write_register(dest_reg,(operand1&operand2));
}

void simulator::OR(uint16_t& dest_reg, uint32_t& operand1, uint32_t& operand2){
  register_map.write_register(dest_reg,(operand1|operand2));
}

void simulator::XOR(uint16_t& dest_reg, uint32_t& operand1, uint32_t& operand2){
    register_map.write_register(dest_reg,(operand1^operand2));
}

void simulator::SLL(uint16_t& dest_reg, uint32_t& operand2, uint16_t& shamt){
    register_map.write_register(dest_reg,(operand2<<shamt));
}

void simulator::SRL(uint16_t& dest_reg, uint32_t& operand2, uint16_t& shamt){
    register_map.write_register(dest_reg,(operand2>>shamt));
}

void simulator::SRA(uint16_t& dest_reg, int32_t& operand2, uint16_t& shamt){
    register_map.write_register(dest_reg,(operand2>>shamt));
}

void simulator::SLLV(uint16_t& dest_reg, uint32_t& operand1, uint32_t& operand2){
    register_map.write_register(dest_reg,(operand2<<operand1));
}

void simulator::SRLV(uint16_t& dest_reg, uint32_t& operand1, uint32_t& operand2){
    register_map.write_register(dest_reg,(operand2>>operand1));
}

void simulator::SRAV(uint16_t& dest_reg, uint32_t& operand1, int32_t& operand2){
    register_map.write_register(dest_reg,(operand2>>operand1));
}

void simulator::SLT(uint16_t& dest_reg, int32_t& operand1, int32_t& operand2){
    if(operand1<operand2) register_map.write_register(dest_reg,1);
    else register_map.write_register(dest_reg,0);
}

void simulator::SLTU(uint16_t& dest_reg, uint32_t& operand1, uint32_t& operand2){
     if(operand1<operand2) register_map.write_register(dest_reg,1);
    else register_map.write_register(dest_reg,0);
}

void simulator::MFHI(uint16_t& dest_reg){
    register_map.write_register(dest_reg,register_map.hi);
}

void simulator::MTHI(uint32_t& operand1){
    register_map.hi = operand1;
}

void simulator::MFLO(uint16_t& dest_reg){
    register_map.write_register(dest_reg,register_map.lo);
}

void simulator::MTLO(uint32_t& operand1){
    register_map.lo = operand1;
}

void simulator::JR(int32_t& operand1){
    std::cout<< "called JR" <<std::endl;
    int32_t op1copy = operand1;
    register_map.program_counter += 4;
    execute();
    register_map.program_counter = op1copy-4;
    std::cout<< "end JR" <<std::endl;
}

void simulator::JALR(uint16_t& dest_reg, int32_t& operand1){
    int32_t op1copy = operand1;
    register_map.program_counter += 4;
    register_map.write_register(dest_reg,(register_map.program_counter+4));
    execute();
    register_map.program_counter = op1copy -4;
}

void simulator::DIV(int32_t& operand1, int32_t& operand2){
  if(operand2==0){
    std::exit(-10);
  }
  else{
    register_map.hi = operand1 / operand2;
    register_map.lo = operand1 % operand2;
  }

}

void simulator::DIVU(uint32_t& operand1, uint32_t& operand2){
  if(operand2==0){
    std::exit(-10);
  }
  else{
    register_map.hi = op1 / operand2;
    register_map.lo = op1 % operand2;
  }
}

void simulator::MULT(int32_t& operand1, int32_t& operand2){
  int64_t product = int64_t(operand1) * int64_t(operand2);
  register_map.lo = product & 0xFFFFFFFF;
  register_map.hi = (product>>32) & 0xFFFFFFFF;
}

void simulator::MULTU(uint32_t& operand1, uint32_t& operand2){
  uint64_t product = int64_t(op1) * int64_t(operand2);
  register_map.lo = product&0xFFFFFFFF;
  register_map.hi = (product>>32)&0xFFFFFFFF;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//I type
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Opcode = 8
void simulator::ADDI(uint16_t& dest_reg, int32_t& operand1, int32_t& operand2){
  if((operand1>0 && operand2>0 && operand1+operand2<=0) || (operand1<0 && operand2<0 && operand1+operand2>=0)){
    std::exit(-10);
  }
  else{
    std::cout << "op1 + op2 = " <<operand1 << " + " << operand2 << " = " << operand1 + operand2 << std::endl;
    register_map.write_register(rt, (operand1 + operand2));
  }
}

//Opcode = 9
void simulator::ADDIU(uint16_t& dest_reg, int32_t& operand1, int32_t& operand2){
  register_map.write_register(rt, (operand1 + operand2));
}

//Opcode = 10
void simulator::SLTI(uint16_t& dest_reg, int32_t& operand1, int32_t& operand2){
  if(op1_s<immediate) register_map.write_register(rt,1);
  else register_map.write_register(rt,0);
}

//Opcode = 11
void simulator::SLTIU(uint16_t& dest_reg, uint32_t& operand1, uint32_t& operand2){
  if(op1<uint32_t(immediate)) register_map.write_register(rt,1);
  else register_map.write_register(rt,0);
}

//Opcode = 12
void simulator::ANDI(uint16_t& dest_reg, uint32_t& operand1, int32_t& imm){
  register_map.write_register(dest_reg,(operand1&imm));
}

//Opcode = 13
void simulator::ORI(uint16_t& dest_reg, uint32_t& operand1, int32_t& imm){
  register_map.write_register(dest_reg,(operand1|imm));
}

//Opcode = 14
void simulator::XORI(uint16_t& dest_reg, uint32_t& operand1, int32_t& imm){
  register_map.write_register(dest_reg,(operand1^imm));
}

//Opcode = 15
void simulator::LUI(uint16_t& dest_reg, uint32_t& imm){
  register_map.write_register(dest_reg, (imm<<16));
}

//Opcode = 4
void simulator::BEQ(int32_t& operand1, int32_t& operand2, int16_t& offset){
  if(operand1==operand2){
    BRANCH(offset);
  }
}

//Opcode = 5
void simulator::BNE(int32_t& operand1, int32_t& operand2, int16_t& offset){
  //std::cout << "offset is "<< (offset<<2) << std::endl;
  if(operand1!=operand2){
    BRANCH(offset);
  }
}

//Opcode = 6
void simulator::BLEZ(int32_t& operand1,int16_t& offset){
  std::cout <<std::endl <<"BLEZ: OFFSET is " << offset << std::endl;
  std::cout <<"operand1 is " << operand1 << std::endl;
  if(operand1<=0){
    BRANCH(offset);
  }
  std::cout << "BLEZ END" << std::endl<<std::endl;
}

//Opcode = 7
void simulator::BGTZ(int32_t& operand1,int16_t& offset){
  if(operand1>0){
    BRANCH(offset);
  }
}


//opcode = 1 for these 4 BRANCHES

//opcode= 1 && rt = 0x0
void simulator::BLTZ(int32_t& operand1,int16_t& offset){
  if(operand1<0){
    BRANCH(offset);
  }
}
//opcode= 1 && rt = 0x1
void simulator::BGEZ(int32_t& operand1,int16_t& offset){
  if(operand1>=0){
    BRANCH(offset);
  }
}

//opcode= 1 && rt = 0x10
void simulator::BLTZAL(int32_t& operand1,int16_t& offset){
  if(operand1<0){
    register_map.write_register(31,(register_map.program_counter+8));
    BRANCH(offset);
  }
}

//opcode= 1 && rt = 0x11
void simulator::BGEZAL(int32_t& operand1,int16_t& offset){
  if(operand1>=0){
    register_map.write_register(31,(register_map.program_counter+4));
    BRANCH(offset);
  }
}

void simulator::BRANCH(int16_t& offset){
  register_map.program_counter += 4;
  int32_t pc_copy = register_map.program_counter;
  execute();
  register_map.program_counter = (pc_copy + (offset<<2))-4;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//J type
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//opcode= 2
void simulator::J(uint32_t& target){
  register_map.program_counter += 4;
  int32_t pc_copy = register_map.program_counter;
  execute();
  register_map.program_counter = ((pc_copy&0xF000000)|(target<<2))-4;
}

//opcode= 3
void simulator::JAL(uint32_t& target){
  register_map.program_counter += 4;
  int32_t pc_copy = register_map.program_counter;
  register_map.write_register(31,(register_map.program_counter+4));
  execute();
  register_map.program_counter = ((pc_copy&0xF000000)|(target_address<<2))-4;
}

void simulator::LB(){
  uint32_t address = (uint32_t)(op1_s + ext_immediate);
  if((address < 0x11000000) && (address >= 0x10000000))
  {
    int8_t instr_byte = mem.load_byte_from_instruction(address);
    int byte = (0x000000FF & instr_byte);
    int  bitmask = 0x00000080;
    if(bitmask & instr_byte)
    {
      byte += 0xFFFFFF00;
    }
    register_map.write_register(rt, byte);
  }
  else register_map.write_register(rt, mem.load_byte_from_memory(address));
}

void simulator::LBU(){
  uint32_t address = (op1_s + ext_immediate);
  if((address < 0x11000000) && (address >= 0x10000000)){
    int8_t instr_byte = mem.load_byte_from_instruction(address);
    register_map.write_register(rt,(uint32_t(instr_byte)&0x000000FF));
  }
  else{
    register_map.write_register(rt, mem.load_unsigned_byte_from_memory(address));
  }
}

void simulator::LH(){
  uint32_t address = (op1_s + ext_immediate);
  if((address < 0x11000000) && (address >= 0x10000000) && (address % 2 == 0))
  {
    int16_t instr_half_word = mem.load_half_word_from_instruction(address);
    register_map.write_register(rt, (int32_t)instr_half_word);
  }
  else register_map.write_register(rt, mem.load_half_word_from_memory(address));
}

void simulator::LHU(){
  uint32_t address = (op1_s + ext_immediate);
  if((address < 0x11000000) && (address >= 0x10000000))
  {
    int16_t instr_half_word = mem.load_half_word_from_instruction(address);
    register_map.write_register(rt, int32_t(instr_half_word)&0x0000FFFF);
  }
  else register_map.write_register(rt, mem.load_unsigned_half_word_from_memory(address));
}

void simulator::LW(){
  uint32_t address = (op1_s + ext_immediate);
  std::cout<<"this is the adress its accessing " << address <<std::endl;
  if((address < 0x11000000) && (address >= 0x10000000)){
    std::cout << "fails at mem read or reg write" << std::endl;
    register_map.write_register(rt, mem.read_instruction(address));
    }
  else register_map.write_register(rt, mem.load_from_memory(address));
}

void simulator::LWL(){
  uint32_t address = (op1_s + ext_immediate);
  if((address < 0x11000000) && (address >= 0x10000000))
  {
    int32_t instr_word = mem.read_instruction(address-(address % 4));
    switch ((address % 4)) {
      case 0: register_map.write_register(rt,instr_word); break;
      case 1: register_map.write_register(rt, (op2_s&0x000000FF)|((instr_word&0x00FFFFFF)<<8)); break;
      case 2: register_map.write_register(rt, (op2_s&0x0000FFFF)|((instr_word&0x0000FFFF)<<16)); break;
      case 3: register_map.write_register(rt, (op2_s&0x00FFFFFF)|((instr_word&0x000000FF)<<24)); break;
    }
  }
  else
  {
    int32_t lwl_word = mem.load_word_left_from_memory(address);
    switch ((address % 4)) {
      case 0: register_map.write_register(rt,lwl_word); break;
      case 1: register_map.write_register(rt, (op2_s&0x000000FF)|lwl_word); break;
      case 2: register_map.write_register(rt, (op2_s&0x0000FFFF)|lwl_word); break;
      case 3: register_map.write_register(rt, (op2_s&0x00FFFFFF)|lwl_word); break;
    }
  }
}

void simulator::LWR()
{
  uint32_t address = (op1_s+ext_immediate);
  if((address < 0x11000000) && (address >= 0x10000000)){
    int32_t instr_word = mem.read_instruction(address-(address % 4));
    switch ((address % 4))
    {
      case 3: register_map.write_register(rt,instr_word); break;
      case 0: register_map.write_register(rt, (op2_s&0xFFFFFF00)|((instr_word&0xFF000000)>>24)); break;
      case 1: register_map.write_register(rt, (op2_s&0xFFFF0000)|((instr_word&0xFFFF0000)>>16)); break;
      case 2: register_map.write_register(rt, (op2_s&0xFF000000)|((instr_word&0xFFFFFF00)>>8)); break;
    }
  }
  else
  {
    int32_t lwr_word = mem.load_word_right_from_memory(address);
    switch ((address % 4))
    {
      case 3: register_map.write_register(rt,lwr_word); break;
      case 0: register_map.write_register(rt, (op2_s&0xFFFFFF00)|lwr_word); break;
      case 1: register_map.write_register(rt, (op2_s&0xFFFF0000)|lwr_word); break;
      case 2: register_map.write_register(rt, (op2_s&0xFF000000)|lwr_word); break;
    }
  }
}

void simulator::SB(){
  int8_t value = op2_s&0xFF;
  mem.store_byte_to_memory((op1_s+ext_immediate),value);
}

void simulator::SH(){
  int16_t val16 = op2_s&0xFFFF;
  mem.store_halfword_to_memory((op1_s + ext_immediate), val16);
}

void simulator::SW() {mem.store_to_memory((op1_s+ext_immediate),op2_s);}
