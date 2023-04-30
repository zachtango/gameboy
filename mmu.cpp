#include <fstream>
#include <iostream>

#include "mmu.h"


void MMU::print_rom() {
    for(int i = 0; i < 0x100; i++) {
        std::cout << std::dec << i << ' ';
        std::cout << std::hex << std::setw(2) << 
            std::setfill('0') << static_cast<int>(M[i]) << '\n';
    }
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
        std::cout << "Write to DMA\n";
        // DMA transfer

        /*
            Source:      $XX00-$XX9F   ;XX = $00 to $DF
            Destination: $FE00-$FE9F
        */
        WORD source = b << 8;
        for(int i = 0; i < 0xA0; i++) {
            M[source + i] = M[0xFE00 + i];
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
        if(M[address] == 0) {
            std::cout << '\n';
            break;
        }
        std::cout << M[address];
    }

    // ROM size
    std::cout << "\tROM size: " << (int) (32 * (1 << size)) << "KiB\n";

    // Header checksum
    std::cout << "\tChecksum: " << std::hex << std::setw(2) << std::setfill('0') << (int) checksum << ' ' <<
       ( ((checksum & 0xFF) == M[0x014D]) ? "Passed" : "Failed" ) << '\n';

}

