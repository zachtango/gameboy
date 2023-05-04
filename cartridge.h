#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include "constants.h"

class Cartridge {
public:
    // FIXME HANDLE MBCs
    
    /* LOAD ROM */
    void load_rom(const char file_name[]);

    /* MEMORY FUNCTIONS */
    BYTE read(WORD address);
    void write(WORD address, BYTE value);

private:
    /* MEMORY FUNCTIONS HELPER */
    BYTE *memory_map(WORD address);

    // https://gbdev.io/pandocs/Memory_Map.html
    
    // 0x0000 - 0x3FFF
    BYTE bank0[0x4000];
    
    // 0x4000 - 0x7FFF
    BYTE bank1[0x4000];

    // 0xA000 - 0xBFFF
    BYTE external_ram[0x2000];
};

#endif