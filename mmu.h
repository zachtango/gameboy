#ifndef MMU_H
#define MMU_H
#include <iostream>
#include "constants.h"

#define MEMORY_BYTES 0x10000

class MMU {
public:
    MMU() : ref_count(new int(1)), 
        M(new BYTE[MEMORY_BYTES]),
        BOOT_M(new BYTE[256]),
        write_to_tile(new bool(false)),
        reset_div(new bool(false)),
        boot(new bool(true)) {}

    MMU(MMU &rhs) : M(rhs.M),
        BOOT_M(rhs.BOOT_M),
        ref_count(rhs.ref_count),
        write_to_tile(rhs.write_to_tile),
        reset_div(rhs.reset_div),
        boot(rhs.boot) {
        *ref_count += 1;
    }

    ~MMU() { 
        if(--(*ref_count) == 0) {
            delete ref_count;
            delete write_to_tile;
            delete reset_div;
            delete[] M;
        }
    }

    int *ref_count;

    void write(WORD address, BYTE b);
    
    BYTE read(WORD address) {
        if(address < 0x100 && *boot)
            return BOOT_M[address];
        return M[address];
    }

    void print_rom();

    void load_boot_rom(const char file[]);
    void load_rom(const char file[]);

    bool *write_to_tile;

    bool *reset_div;
    bool *boot;

// private:
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
        0xFF00 - 0xFF7F I/O Registers (0xFF0F is Interrupt Flag)
        0xFF80 - 0xFFFE HRAM
        0xFFFF Interrupt Enable
    */
    BYTE *M;

    BYTE *BOOT_M;

};

#endif