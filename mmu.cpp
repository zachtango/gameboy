#include <fstream>


#include "helpers.h"
#include "mmu.h"
#include "cartridge.h"
#include "ppu.h"
#include "timer.h"
#include "joypad.h"
#include "interrupts.h"

BYTE MMU::read(WORD address) {
    // std::cout << "read: " << (int) address << '\n';
    // MEMORY MAP
    // https://gbdev.io/pandocs/Memory_Map.html?highlight=wram#memory-map

    // 0x0000 - 0x3FFF bank0
    // 0x4000 - 0x7FFF bank1
    if(address < 0x8000)
        return cartridge.read(address);

    // 0x8000 - 0x9FFF VRAM
    else if(address < 0xA000)
        return ppu.read(address);

    // 0xA000 - 0xBFFF external RAM
    else if(address < 0xC000)
        return cartridge.read(address);

    // 0xC000 - 0xDFFF WRAM
    else if(address < 0xE000)
        return wram[address - 0xC000];

    // 0xE000 - 0xFDFF Mirror of 0xC000 - 0xDDFF, Prohibited area by Nintendo
    else if(address < 0xFE00)
        return wram[address - 0xFE00];

    // 0xFE00 - 0xFE9F Sprite attribute table (OAM)
    else if(address < 0xFEA0)
        return ppu.read(address);

    // 0xFEA0 - 0xFEFF Prohibited area by Nintendo
    else if(address < 0xFF00)
        return 0x00;

    // 0xFF00 - 0xFF7F IO Registers
    else if(address < 0xFF80) {
        /*
            https://gbdev.io/pandocs/Hardware_Reg_List.html
            Reading unreadable bits of registers yields 1 (write only registers yield 0xFF)
        */

        /*
            Implemented IO registers
            
            0xFF00              Joypad Mixed
            0xFF04 - 0xFF07     Timer R/W
            0xFF0F              Interrupt flag R/W
            0xFF40 - 0xFF45     PPU R/W, Mixed
            0xFF46              DMA R/W
            0xFF47 - 0xFF49     PPU R/W
            0xFF4A - 0xFF4B     PPU R/W
        */
        if(address == 0xFF00) {
            
            return joypad.read(address);
        }
        else if(0xFF04 <= address && address <= 0xFF07)
            return timer.read(address);

        else if(address == 0xFF0F)
            return interrupts.read(address);
        
        else if( (0xFF40 <= address && address <= 0xFF45) ||
            (0xFF47 <= address && address <= 0xFF49) ||
            (0xFF4A <= address && address <= 0xFF4B) )
            return ppu.read(address);

        else if(address == 0xFF46)
            return io_registers[address - 0xFF00];

        /*
            R/W unimplemented IO registers
            0xFF01
            0xFF10
            0xFF12
            0xFF17
            0xFF1A
            0xFF1C
            0xFF21 - 0XFF22
            0xFF24 - 0xFF25
            0xFF30 - 0xFF3F
        */

        switch(address) {
            case 0xFF01:
            case 0xFF10:
            case 0xFF12:
            case 0xFF17:
            case 0xFF1A:
            case 0xFF1C:
            case 0xFF21:
            case 0xFF22:
            case 0xFF24:
            case 0xFF25:
                return io_registers[address - 0xFF00];
        }
        if(0xFF30 <= address && address <= 0xFF3F)
            return io_registers[address - 0xFF00];

        /*
            Mixed unimplemented IO registers
            0xFF00 bit 7 - 6 Unused
            0xFF02 bit 2 Unused
            0xFF11 bit 5 - 0 Write only
            0xFF14 bit 7, 2 - 0 Write only
            0xFF16 bit 5 - 0 Write only
            0xFF19 bit 7, 2 - 0 Write only
            0xFF1E bit 7, 2 - 0 Write only
            0xFF23 bit 7, 5 - 0 Write only, Unused
            0xFF26 bit 6 - 4 Unused
        */
        BYTE b = io_registers[address - 0xFF00];
        switch(address) {
            case 0xFF00:
                return b | 0b11000000;
            case 0xFF02:
                return b | 0x00000100;
            case 0xFF11:
            case 0xFF16:
                return b | 0b00111111;
            case 0xFF14:
            case 0xFF19:
            case 0xFF1E:
                return b | 0b00000111;
            case 0xFF23:
                return b | 0b00111111;
            case 0xFF26:
                return b | 0x01110000;
        }

        /*
            0xFF registers
            All others
        */
        return 0xFF;
    }

    // 0xFF80 - 0xFFFE HRAM
    else if(address < 0xFFFF)
        return hram[address - 0xFF80];

    // 0xFFFF
    else if(address == 0xFFFF)
        return interrupts.read(address);
    
    std::cerr << "Address out of MMU range\n";
    throw "Address out of MMU range";
}

void MMU::write(WORD address, BYTE value) {
    
    // MEMORY MAP
    // https://gbdev.io/pandocs/Memory_Map.html?highlight=wram#memory-map

    // 0x0000 - 0x3FFF bank0
    // 0x4000 - 0x7FFF bank1
    if(address < 0x8000) {
        std::cerr << "Should not be writing to ROM (Read Only Memory)\n";
        return;
    }

    // 0x8000 - 0x9FFF VRAM
    else if(address < 0xA000)
        ppu.write(address, value);

    // 0xA000 - 0xBFFF external RAM
    else if(address < 0xC000)
        cartridge.write(address, value);

    // 0xC000 - 0xDFFF WRAM
    else if(address < 0xE000)
        wram[address - 0xC000] = value;

    // 0xE000 - 0xFDFF Mirror of 0xC000 - 0xDDFF, Prohibited area by Nintendo
    else if(address < 0xFE00)
        wram[address - 0xFE00] = value;

    // 0xFE00 - 0xFE9F Sprite attribute table (OAM)
    else if(address < 0xFEA0)
        ppu.write(address, value);

    // 0xFEA0 - 0xFEFF Prohibited area by Nintendo
    else if(address < 0xFF00)
        0x00;

    // 0xFF00 - 0xFF7F IO Registers
    else if(address < 0xFF80) {
        /*
            https://gbdev.io/pandocs/Hardware_Reg_List.html
            Reading unreadable bits of registers yields 1 (write only registers yield 0xFF)
        */

        /*
            Implemented IO registers
            
            0xFF00              Joypad Mixed
            0xFF04 - 0xFF07     Timer R/W
            0xFF0F              Interrupt flag R/W
            0xFF40 - 0xFF45     PPU R/W, Mixed
            0xFF46              DMA R/W
            0xFF47 - 0xFF49     PPU R/W
            0xFF4A - 0xFF4B     PPU R/W
        */
        if(address == 0xFF00)
            joypad.write(address, value);

        else if(0xFF04 <= address && address <= 0xFF07)
            timer.write(address, value);

        else if(address == 0xFF0F)
            interrupts.write(address, value);
        
        else if( (0xFF40 <= address && address <= 0xFF45) ||
            (0xFF47 <= address && address <= 0xFF49) ||
            (0xFF4A <= address && address <= 0xFF4B) )
            ppu.write(address, value);

        else if(address == 0xFF46) {
            // https://gbdev.io/pandocs/OAM_DMA_Transfer.html
            io_registers[address - 0xFF00] = value;
            
            // value written is XX
            WORD a = value << 8;

            // transfer values from XX00 - XX9F to FE00 - FE9F (Sprite attribute table OAM)
            for(WORD i = 0; i < 0xA0; i++) {
                ppu.write(
                    0xFE00 + i,
                    read(a + i)
                );
            }

            // transfer takes 640 T cycles --> tick PPU and Timer (CPU can't do anything but access HRAM)
            for(int i = 0; i < 640; i++) {
                timer.tick();
                ppu.tick();
            }
        }
        else
            io_registers[address - 0xFF00] = value;
    }

    // 0xFF80 - 0xFFFE HRAM
    else if(address < 0xFFFF)
        hram[address - 0xFF80] = value;

    // 0xFFFF
    else if(address == 0xFFFF)
        interrupts.write(address, value);
}
