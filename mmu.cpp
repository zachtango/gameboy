#include <fstream>
#include <iostream>

#include "mmu.h"

void MMU::read_rom(const char file_name[]) {
    std::ifstream file(file_name, std::ios::binary);

    if(!file.is_open()) {
        throw "Error opening file";
    }
    
    // BYTE byte;
    // int i = 0;
    // while(file.read((char*) &byte, 1)) {
    //     // M[i] = byte;
    //     i += 1;
    //     // if(i >= 100 && i <= 110) {
    //         std::cout << std::dec << i << ' ';
    //         std::cout << std::hex << std::setw(2) << 
    //             std::setfill('0') << static_cast<int>(byte) << '\n';
    //     // }
    // }

    file.seekg(0, std::ios::end);
    int size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::cout << "size " << size << std::endl;
    file.read((char*) M, size);
}

void MMU::print_rom() {
    for(int i = 0; i < 0x100; i++) {
        std::cout << std::dec << i << ' ';
        std::cout << std::hex << std::setw(2) << 
            std::setfill('0') << static_cast<int>(M[i]) << '\n';
    }
}



