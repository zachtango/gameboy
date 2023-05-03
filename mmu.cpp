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
    
    if(address == 0xFF00) {
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

