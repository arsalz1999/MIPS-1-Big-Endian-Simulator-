#include <stdint.h>

class registers{
    private:
        int32_t register_vector[32];
    public:
        registers();

        //setting counter
        uint32_t program_counter = 0x10000000;
        uint32_t hi = 0;
        uint32_t lo = 0;


        //read register
        uint32_t read_register(int current);
        //write register
        void write_register(int current, uint32_t data);
};
