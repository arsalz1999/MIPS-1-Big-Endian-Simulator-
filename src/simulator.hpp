#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include "memory.hpp"
#include <stdlib.h>
#include <string>
#include "register.hpp"
#include <stdint.h>

class simulator{
    private:

        //constructing objects
        memory mem;
        registers register_map;

        //creating needed variables for RIJ type Instructions
        uint16_t funct;
        uint16_t shift_amount;
        uint16_t rd;
        uint16_t rt;
        uint16_t rs;
        uint16_t opcode;

        int32_t u_immediate;
        int32_t immediate;
        int16_t imm_16;
        int32_t ext_immediate;

        uint32_t target_address;

        int32_t  op1_s;
        int32_t  op2_s;
        uint32_t op1;
        uint32_t op2;

    public:
        simulator(std::string binaryfile);
        void run();
        void execute();

        void execute_R(uint32_t instruction);
        void execute_J(uint32_t instruction);
        void execute_I(uint32_t instruction);

        //R type instrucrions
        void ADD(uint16_t& dest_reg, int32_t& operand1, int32_t& operand2);
        void ADDU(uint16_t& dest_reg, uint32_t& operand1, uint32_t& operand2);
        void AND(uint16_t& dest_reg, uint32_t& operand1, uint32_t& operand2);
        void DIV(int32_t& operand1, int32_t& operand2);
        void DIVU(uint32_t& operand1, uint32_t& operand2);
        void JALR(uint16_t& dest_reg, int32_t& operand1);
        void JR(int32_t& operand1);
        void MFHI(uint16_t& dest_reg);
        void MFLO(uint16_t& dest_reg);
        void MTHI(uint32_t& operand1);
        void MTLO(uint32_t& operand1);
        void MULT(int32_t& operand1, int32_t& operand2);
        void MULTU(uint32_t& operand1, uint32_t& operand2);
        void OR(uint16_t& dest_reg, uint32_t& operand1, uint32_t& operand2);
        void SLL(uint16_t& dest_reg, uint32_t& operand2, uint16_t& shamt);
        void SLLV(uint16_t& dest_reg, uint32_t& operand1, uint32_t& operand2);
        void SLT(uint16_t& dest_reg, int32_t& operand1, int32_t& operand2);
        void SLTU(uint16_t& dest_reg, uint32_t& operand1, uint32_t& operand2);
        void SRA(uint16_t& dest_reg, int32_t& operand2, uint16_t& shamt);
        void SRAV(uint16_t& dest_reg, uint32_t& operand1, int32_t& operand2);
        void SRL(uint16_t& dest_reg, uint32_t& operand2, uint16_t& shamt);
        void SRLV(uint16_t& dest_reg, uint32_t& operand1, uint32_t& operand2);
        void SUB(uint16_t& dest_reg, int32_t& operand1, int32_t& operand2);
        void SUBU(uint16_t& dest_reg, uint32_t& operand1, uint32_t& operand2);
        void XOR(uint16_t& dest_reg, uint32_t& operand1, uint32_t& operand2);

        //J type instrucrions
        void J(uint32_t& target);
        void JAL(uint32_t& target);

        //I type instrucrions
        void ADDI(uint16_t& dest_reg, int32_t& operand1, int32_t& operand2);
        void ADDIU(uint16_t& dest_reg, int32_t& operand1, int32_t& operand2);
        void ANDI(uint16_t& dest_reg, uint32_t& operand1, int32_t& imm);
        void ORI(uint16_t& dest_reg, uint32_t& operand1, int32_t& imm);
        void XORI(uint16_t& dest_reg, uint32_t& operand1, int32_t& imm);

        void BEQ(int32_t& operand1, int32_t& operand2, int16_t& offset);
        void BLTZ(int32_t& operand1,int16_t& offset);
        void BLTZAL(int32_t& operand1,int16_t& offset);
        void BGEZ(int32_t& operand1,int16_t& offset);
        void BGEZAL(int32_t& operand1,int16_t& offset);
        void BGTZ(int32_t& operand1,int16_t& offset);
        void BLEZ(int32_t& operand1,int16_t& offset);
        void BNE(int32_t& operand1, int32_t& operand2, int16_t& offset);
        void BRANCH(int16_t& offset);

        void SLTI(uint16_t& dest_reg, int32_t& operand1, int32_t& operand2);
        void SLTIU(uint16_t& dest_reg, uint32_t& operand1, int32_t& operand2);
        void LUI(uint16_t& dest_reg, int32_t& imm);




        //memory instructions
        void LB();
        void LBU();
        void LH();
        void LHU();
        void LW();
        void LWL();
        void LWR();
        void SB();
        void SH();
        void SW();



};

#endif
