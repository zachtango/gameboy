#include "cartridge.h"
#include <fstream>


static char *rom_type[0x100] {
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

/* LOAD ROM */
void Cartridge::load_rom(const char file_name[]) {
    // Open file for reading
    std::ifstream file(file_name, std::ios::binary);

    if(!file.is_open()) {
        std::cerr << "Error opening ROM file\n";
        throw "Error opening ROM file";
    }

    // Get ROM size
    file.seekg(0, std::ios::end);
    U32 rom_size = file.tellg();
    
    file.seekg(0, std::ios::beg);
    
    // Load ROM data into banks FIXME bank
    file.read((char*) bank0, rom_size);

    // https://gbdev.io/pandocs/The_Cartridge_Header.html
    // Output ROM metadata 

    // 0x14D Header checksum
    BYTE checksum = 0;
    for(WORD address = 0x0134; address <= 0x014C; address += 1) {
        checksum = checksum - bank0[address] - 1;
    }

    // Print ROM info
    std::cout << "Loading ROM: " << file_name << '\n';

    // Title
    std::cout << "\tTitle: ";

    for(WORD address = 0x0134; address <= 0x0143; address += 1) {
        if(bank0[address] == '\0')
            break;
        std::cout << bank0[address];
    }
    std::cout << '\n';

    // ROM size
    std::cout << "\tROM Size: " << (int) (32 * (1 << rom_size)) << "KiB\n";

    // ROM type
    std::cout << "\tROM Type: " << rom_type[bank0[0x0147]] << '\n';

    // Header checksum
    std::cout << "\tChecksum: " << std::hex << std::setw(2) << std::setfill('0') << (int) checksum << 
        ' ' << ( ((checksum & 0xFF) == bank0[0x014D]) ? "Passed" : "Failed" ) << '\n';
}

/* MEMORY FUNCTIONS */
BYTE Cartridge::read(WORD address) {
    BYTE *p = memory_map(address);

    // std::cout << (int) address << ' ' << (int) *p << '\n';
    return *p;
}

void Cartridge::write(WORD address, BYTE value) {
    BYTE *p = memory_map(address);
    *p = value;
}

/* Given 16 bit address
    Return pointer to the BYTE in memory the address refers to
*/
BYTE* Cartridge::memory_map(WORD address) {

    // 0x0000 - 0x3FFF
    if(0x0000 <= address && address <= 0x3FFF)
        return &bank0[address];
        // return &bank0[address];
    
    // 0x4000 - 0x7FFF
    if(0x4000 <= address && address <= 0x7FFF)
        return &bank0[address];
        // return &bank1[address - 0x4000];

    // 0xA000 - 0xBFFF
    if(0xA000 <= address && address <= 0xBFFF)
        return &external_ram[address - 0xA000];

    std::cerr << std::hex << address << " Address not in Cartridge range\n";
    throw "Address not in Cartridge range";
}