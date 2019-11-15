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
        int32_t load_from_memory(int pc_position);
        int32_t load_byte_from_memory(int pc_position);
        uint32_t load_unsigned_byte_from_memory(int pc_position);
        int32_t load_half_word_from_memory(int pc_position);
        uint32_t load_unsigned_half_word_from_memory(int pc_oosition);
        int32_t load_word_right_from_memory(int pc_position);
        int32_t memory::load_word_left_from_memory(int pc_position);
};

#endif