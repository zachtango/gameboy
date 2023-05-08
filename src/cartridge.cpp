#include "cartridge.h"
#include <fstream>


static const char *rom_type_map[0x100] {
    [0x00] = "ROM ONLY",
    [0x01] = "MBC1",
    [0x02] = "MBC1+RAM",
    [0x03] = "MBC1+RAM+BATTERY",
    [0x05] = "MBC2",
    [0x06] = "MBC2+BATTERY",
    [0x08] = "ROM+RAM 1",
    [0x09] = "ROM+RAM+BATTERY 1",
    [0x0B] = "MMM01",
    [0x0C] = "MMM01+RAM",
    [0x0D] = "MMM01+RAM+BATTERY",
    [0x0F] = "MBC3+TIMER+BATTERY",
    [0x10] = "MBC3+TIMER+RAM+BATTERY 2",
    [0x11] = "MBC3",
    [0x12] = "MBC3+RAM 2",
    [0x13] = "MBC3+RAM+BATTERY 2",
    [0x19] = "MBC5",
    [0x1A] = "MBC5+RAM",
    [0x1B] = "MBC5+RAM+BATTERY",
    [0x1C] = "MBC5+RUMBLE",
    [0x1D] = "MBC5+RUMBLE+RAM",
    [0x1E] = "MBC5+RUMBLE+RAM+BATTERY",
    [0x20] = "MBC6",
    [0x22] = "MBC7+SENSOR+RUMBLE+RAM+BATTERY",
    [0xFC] = "POCKET CAMERA",
    [0xFD] = "BANDAI TAMA5",
    [0xFE] = "HuC3",
    [0xFF] = "HuC1+RAM+BATTERY",
};

static const char *rom_size_map[0x55] = {
    [0x00] = "32 KiB",
    [0x01] = "64 KiB",
    [0x02] = "128 KiB",
    [0x03] = "256 KiB",
    [0x04] = "512 KiB",
    [0x05] = "1 MiB",
    [0x06] = "2 MiB",
    [0x07] = "4 MiB",
    [0x08] = "8 MiB",
    [0x52] = "1.1 MiB",
    [0x53] = "1.2 MiB",
    [0x54] = "1.5 MiB",
};

static const int rom_banks_map[0x55] = {
    [0x00] = 2,
    [0x01] = 4,
    [0x02] = 8,
    [0x03] = 16,
    [0x04] = 32,
    [0x05] = 64,
    [0x06] = 128,
    [0x07] = 256,
    [0x08] = 512,
    [0x52] = 72,
    [0x53] = 80,
    [0x54] = 96
};

/* LOAD ROM */
void Cartridge::load_rom(const char file_name[]) {
    // Open file for reading
    std::ifstream file(file_name, std::ios::binary);

    if(!file.is_open()) {
        std::cerr << "Error opening ROM file\n";
        throw "Error opening ROM file";
    }

    // Get ROM size at 0x0148
    file.seekg(0x0148, std::ios::beg);
    
    BYTE rom_size_id;
    file.read((char*) &rom_size_id, 1);

    number_of_banks = rom_banks_map[rom_size_id];
    U32 rom_size = number_of_banks * BYTES_PER_BANK;

    // create bank mask
    rom_bank_number_mask = 0b00011111;
    
    U32 leading_zeros = __builtin_clz(number_of_banks) - 24;
    if(leading_zeros > 2) {
        rom_bank_number_mask >>= leading_zeros - 2;
    }

    // load ROM into memory
    file.seekg(0, std::ios::beg);
    
    // allocate enough memory
    memory = new BYTE[rom_size];

    // Load ROM data into banks
    file.read((char*) memory, rom_size);
    
    // for MBCs
    if(memory[0x0147] != 0)
        rom_only = false;
    

    // https://gbdev.io/pandocs/The_Cartridge_Header.html
    // Output ROM metadata 

    // 0x14D Header checksum
    BYTE checksum = 0;
    for(WORD address = 0x0134; address <= 0x014C; address += 1) {
        checksum = checksum - memory[address] - 1;
    }

    // Print ROM info
    std::cout << "Loading ROM: " << file_name << '\n';

    // Title
    std::cout << "\tTitle: ";

    for(WORD address = 0x0134; address <= 0x0143; address += 1) {
        if(memory[address] == '\0')
            break;
        std::cout << memory[address];
    }
    std::cout << '\n';

    // ROM type
    std::cout << "\tROM Type: " << rom_type_map[memory[0x0147]] << '\n';

    // ROM size
    std::cout << "\tROM Size: " << rom_size_map[memory[0x0148]] << '\n';

    // Number of Banks
    std::cout << "\tNumber of Banks: " << std::dec << (int) number_of_banks << '\n';

    // Header checksum
    std::cout << "\tChecksum: " << std::hex << std::setw(2) << std::setfill('0') << (int) checksum << 
        ' ' << ( ((checksum & 0xFF) == memory[0x014D]) ? "Passed" : "Failed" ) << '\n';
}

/* MEMORY FUNCTIONS */
BYTE Cartridge::read(WORD address) {
    BYTE *p = memory_map(address);
    return *p;
}

void Cartridge::write(WORD address, BYTE value) {
    if(rom_only) {
        std::cerr << "Should not be writing to ROM (Read Only Memory)\n";
        return;
    }

    // https://gbdev.io/pandocs/MBC1.html
    // MBCs

    // MBC1 Registers
    if(address < 0x2000) {
        // 0x0000 - 0x1FFF RAM Enable
        ram_enable = value;
    } 
    else if(address < 0x4000) {
        // 0x2000 - 0x3FFF ROM Bank Number
        // https://gbdev.io/pandocs/MBC1.html#20003fff--rom-bank-number-write-only
        
        // only care about lower 5 bits
        value &= 0b00011111;

        // 0x00 values gets converted to 0x01
        if(value == 0)
            value = 1;

        // if ROM Bank Number is set to higher value than number of banks, we mask the required number of bits
        rom_bank_number = value & rom_bank_number_mask;
    }
    else if(address < 0x6000) {
        // RAM Bank Number -- OR -- Upper Bits of ROM Bank Number
        
    }
    else if(address < 0x8000) {
        // Banking Mode Select
        bank_mode_select = value;   
    }
}

/* Given 16 bit address
    Return pointer to the BYTE in memory the address refers to
*/
BYTE* Cartridge::memory_map(WORD address) {

    // 0x0000 - 0x3FFF
    if(address < 0x4000)
        return &memory[address];
    
    // 0x4000 - 0x7FFF
    else if(address << 0x8000) {
        // rom_bank_number is 1 if rom_only, otherwise it handles MBC
        U32 bank_address = rom_bank_number == 0 ? address - 0x4000 : ((rom_bank_number - 1) * 0x4000) + address;
        // std::cout << "rom_bank_number: " << (int) rom_bank_number << " address: " << (int) address << '\n';
        return &memory[bank_address];
    }

    // 0xA000 - 0xBFFF
    else if(0xA000 <= address && address <= 0xBFFF)
        return &memory[address];

    std::cerr << std::hex << address << " Address not in Cartridge range\n";
    throw "Address not in Cartridge range";
}