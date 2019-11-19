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
        int32_t load_word_left_from_memory(int pc_position);
        int8_t load_byte_from_instruction(int pc_position);
        int16_t load_half_word_from_instruction(int pc_position);

        void store_to_memory(int pc_position, int32_t value);
        void store_byte_to_memory(int pc_position, int8_t value);
        void store_halfword_to_memory(int pc_position, int16_t value);


};

#endif
