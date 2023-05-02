#include <fstream>
#include <iostream>

#include "mmu.h"
#include "helpers.h"


void MMU::print_rom() {
    for(int i = 0; i < 0x100; i++) {
        std::cout << std::dec << i << ' ';
        std::cout << std::hex << std::setw(2) << 
            std::setfill('0') << static_cast<int>(M[i]) << '\n';
    }
}

BYTE MMU::read(WORD address) {
    if(address < 0x100 && *boot)
        return BOOT_M[address];
    else if(address == 0xFF00) {
        BYTE j = M[0xFF00];
        std::cout << "read joypad\n";
        std::cout << std::hex << (int) joypad->p1 << '\n';
        if(!get_bit(j, 5)) {
            // Upper 4 bits
            return j | (0x00FF & ~(joypad->p1 >> 4));
        } else if(!get_bit(j, 4)) {
            // Lower 4 bits
            return j | (0x0FF & ~(joypad->p1));
        }
    }

    return M[address];
}

void MMU::write(WORD address, BYTE b) {
    // if(address == 0xFF02 or address == 0xFF01) {
    //     std::cout << "WROTE TO " << std::hex << address << ' ' << (int) M[0xff02] << '\n';
    // }

    if( (address >= 0x8000 && address <= 0x9FFF && M[address] != b) ) {
        *write_to_tile = true;
        // std::cout << "write to tile " << std::hex << (int) b << '\n';
    }

    M[address] = b;

    if(address == 0xFF04) {
        *reset_div = true;
    }

    // https://gbdev.io/pandocs/OAM_DMA_Transfer.html
    else if(address == 0xFF46) {
        *write_to_dma = true;
        std::cout << "Write to DMA\n";
        // DMA transfer

        /*
            Source:      $XX00-$XX9F   ;XX = $00 to $DF
            Destination: $FE00-$FE9F
        */
        WORD source = b << 8;
        for(WORD i = 0; i < 0xA0; i++) {
            M[0xFE00 + i] = M[source + i];
        }
    } else if(address == 0xFFFF) {
        std::cout << "Write to IE register\n";
        std::cout << std::hex << (int) b << '\n';
    }

    if (M[0xff02] == 0x81) {
        char c = M[0xff01];
        printf("%c", c);
        M[0xff02] = 0;
    }
}

void MMU::load_boot_rom(const char file_name[]) {
    std::ifstream file(file_name, std::ios::binary);

    if(!file.is_open()) {
        throw "Error opening boot ROM file";
    }

    // Get rom size
    file.seekg(0, std::ios::end);
    UINT size = file.tellg();
    
    file.seekg(0, std::ios::beg);

    // Load boot rom data into Memory
    file.read((char*) BOOT_M, size);

    std::cout << "Loading boot ROM: " << file_name << '\n';
}

void MMU::load_rom(const char file_name[]) {
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

    std::ifstream file(file_name, std::ios::binary);

    if(!file.is_open()) {
        throw "Error opening ROM file";
    }

    // Get rom size
    file.seekg(0, std::ios::end);
    UINT size = file.tellg();
    
    file.seekg(0, std::ios::beg);
    
    // Load rom data into Memory
    file.read((char*) M, size);
    
    // for(int i = 0; i < size; i++) {
    //     if(M[i] == 0xFB) {
    //         std::cout << "contains EI at " << std::hex << i << '\n';
    //     }
    // }

    // https://gbdev.io/pandocs/The_Cartridge_Header.html

    // 0x14D Header checksum
    BYTE checksum = 0;
    for(WORD address = 0x0134; address <= 0x014C; address += 1) {
        checksum = checksum - M[address] - 1;
    }

    // Print ROM info
    std::cout << "Loading ROM: " << file_name << '\n';

    // Title
    std::cout << "\tTitle: ";

    for(WORD address = 0x0134; address <= 0x0143; address += 1) {
        if(M[address] == 0)
            break;
        std::cout << M[address];
    }
    std::cout << '\n';

    // ROM size
    std::cout << "\tROM Size: " << (int) (32 * (1 << size)) << "KiB\n";

    // ROM type
    std::cout << "\tROM Type: " << rom_type[M[0x0147]] << '\n';

    // Header checksum
    std::cout << "\tChecksum: " << std::hex << std::setw(2) << std::setfill('0') << (int) checksum << ' ' <<
       ( ((checksum & 0xFF) == M[0x014D]) ? "Passed" : "Failed" ) << '\n';

}

