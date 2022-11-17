#include <stdint.h>

#define MEMORY_SIZE 0x10000

class MMU {

    uint8_t M[MEMORY_SIZE];

public:
    uint8_t readByte(uint16_t addr); // read 8 bit byte M[addr]
    uint16_t readWord(uint16_t addr); // read 16 bit word lo M[addr] hi M[addr + 1]

};


