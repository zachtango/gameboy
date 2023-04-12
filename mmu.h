#include "constants.h"

#define MEMORY_BYTES 0x10000

class MMU {
public:

    void write(WORD address, BYTE b) { M[address] = b; }
    BYTE read(WORD address) { return M[address]; }

private:
    /*
        0x0000 - 0x3FFF Cartridge
        0x4000 - 0x7FFF Cartridge
        0x8000 - 0x9FFF Video RAM
        0xA000 - 0xBFFF External RAM
        0xC000 - 0xCFFF WRAM
        0xD000 - 0xDFFF WRAM
        0xE000 - 0xFDFF ECHO RAM
        0xFE00 - 0xFE9F Sprite Attribute Table (OAM)
        0xFEA0 - 0xFEFF Not Usable
        0xFF00 - 0xFF7F I/O Registers
        0xFF80 - 0xFFFE HRAM
        0xFFFF Interrupt Enable register
    */
    WORD M[MEMORY_BYTES];


};
