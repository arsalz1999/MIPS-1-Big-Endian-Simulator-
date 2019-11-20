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
        uint16_t shamt;
        uint16_t rd;
        uint16_t rt;
        uint16_t rs;
        uint16_t opcode;

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
        void ADD();
        void ADDU();
        void AND();
        void DIV();
        void DIVU();
        void JALR();
        void JR();
        void MFHI();
        void MFLO();
        void MTHI();
        void MTLO();
        void MULT();
        void MULTU();
        void OR();
        void SLL();
        void SLLV();
        void SLT();
        void SLTU();
        void SRA();
        void SRAV();
        void SRL();
        void SRLV();
        void SUB();
        void SUBU();
        void XOR();

        void J();
        void JAL();

        void ADDI();
        void ADDIU();
        void ANDI(uint16_t& dest_reg, uint32_t& operand1, int32_t& imm);
        void BEQ(int32_t& operand1, int32_t& operand2, int16_t& offset);
        void BLTZ(int32_t& operand1,int16_t& offset);
        void BLTZAL(int32_t& operand1,int16_t& offset);
        void BGEZ(int32_t& operand1,int16_t& offset);
        void BGEZAL(int32_t& operand1,int16_t& offset);
        void BGTZ(int32_t& operand1,int16_t& offset);
        void BLEZ(int32_t& operand1,int16_t& offset);
        void BNE(int32_t& operand1, int32_t& operand2, int16_t& offset);
        void BRANCH(int16_t offset);

        void LB();
        void LBU();
        void LH();
        void LHU();
        void LUI();
        void LW();
        void LWL();
        void LWR();

        void ORI(uint16_t& dest_reg, uint32_t& operand1, int32_t& imm);
        void SLTI();
        void SLTIU();
        void SB();
        void SH();
        void SW();
        void XORI(uint16_t& dest_reg, uint32_t& operand1, int32_t& imm);


};

#endif
