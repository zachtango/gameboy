#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include "constants.h"

#define BYTES_PER_BANK 0x4000

class Cartridge {
public:
    Cartridge() {
        std::cout << "CARTRIDGE INITIALIZED\n";

        rom_only = true;
        number_of_banks = 2;

        // MBC Registers set to 0x00 on power up
        ram_enable = 0;
        // 0x00 interpretted as 0x01
        rom_bank_number = 1;
        bank_mode_select = 0;
    }

    ~Cartridge() {
        std::cout << "Deleting memory\n";
        delete[] memory;
    }

    /* LOAD ROM */
    void load_rom(const char file_name[]);

    /* MEMORY FUNCTIONS */
    BYTE read(WORD address);
    void write(WORD address, BYTE value);

private:
    /* MEMORY FUNCTIONS HELPER */
    BYTE *memory_map(WORD address);

    // https://gbdev.io/pandocs/The_Cartridge_Header.html#0147--cartridge-type
    bool rom_only;

    // https://gbdev.io/pandocs/Memory_Map.html
    // Just banks and external RAM
    BYTE *memory;
    BYTE number_of_banks;
    
    // https://gbdev.io/pandocs/MBC1.html
    // MBC1 Registers
    BYTE ram_enable;
    BYTE rom_bank_number;
    bool bank_mode_select;

    // helpers
    BYTE rom_bank_number_mask;
};

#endif