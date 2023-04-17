#include <fstream>
#include <iostream>

#include "mmu.h"

void MMU::read_rom(const char file_name[]) {
    std::ifstream file(file_name, std::ios::binary);

    if(!file.is_open()) {
        throw "Error opening file";
    }
    
    file.seekg(0, std::ios::end);
    int size = file.tellg();
    file.seekg(0, std::ios::beg);

    file.read((char*) M, size);
}

void MMU::print_rom() {
    for(int i = 0; i < 10; i++) {
        std::cout << std::dec << i << ' ';
        std::cout << std::hex << std::setw(2) << 
            std::setfill('0') << static_cast<int>(M[i]) << '\n';
    }
}



