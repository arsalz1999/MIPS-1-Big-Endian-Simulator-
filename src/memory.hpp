#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <vector>
#include <stdint.h>
#include <string>

class memory{
    private:
        std::vector<uint8_t> ADDR_DATA;
        std::vector<uint32_t> ADDR_INSTR;
        int total_instructions;

    public:
        memory(std::string binaryfile);
        uint32_t read_instruction(uint32_t pc);
};

#endif