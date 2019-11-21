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
        void check_flags();
        bool end_of_file();
        uint32_t read_instruction(uint32_t pc);
        int32_t load_mem(int pc_position);
        int32_t load_mem_s_b(int pc_position);
        uint32_t load_mem_u_b(int pc_position);
        int32_t load_mem_hw(int pc_position);
        uint32_t load_mem_uhw(int pc_oosition);
        int32_t load_mem_wr(int pc_position);
        int32_t load_mem_wl(int pc_position);
        int8_t load_instr_b(int pc_position);
        int16_t load_instr_hw(int pc_position);

        void store_mem(int pc_position, int32_t to_store);
        void store_mem_b(int pc_position, int8_t to_store);
        void store_mem_hw(int pc_position, int16_t to_store);


};

#endif
