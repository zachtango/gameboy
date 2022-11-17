#include "mmu.h"

using namespace std;

uint8_t MMU::readByte(uint16_t addr){
    return M[addr];
}

uint16_t MMU::readWord(uint16_t addr){
    uint8_t lo = M[addr];
    uint8_t hi = M[addr + 1];

    return (hi << 8u) | lo;
}


