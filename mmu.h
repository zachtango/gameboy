#ifndef MMU_H
#define MMU_H

#include "constants.h"

class Cartridge;
class PPU;
class Timer;
class Joypad;
class Interrupts;

class MMU {
public:
    MMU(Cartridge &cartridge,
        PPU &ppu,
        Timer &timer,
        Joypad &joypad,
        Interrupts &interrupts) :

        cartridge(cartridge),
        ppu(ppu),
        timer(timer),
        interrupts(interrupts),
        joypad(joypad) {

        std::cout << "MMU INITIALIZED\n";
    }

    /* MEMORY FUNCTIONS */
    BYTE read(WORD address);
    void write(WORD address, BYTE value);
private:
    /* DEVICES WITH REGISTERS */
    Cartridge &cartridge;
    PPU &ppu;
    Timer &timer;
    Joypad &joypad;
    Interrupts &interrupts;

    /* UNIMPLEMENTED IO REGISTERS */
    // 0xFF00 - 0xFF7F (some implemented)
    BYTE io_registers[0x0080];

    /* INTERNAL RAM */
    // 0xC000 - 0xDFFF
    BYTE wram[0x2000];

    // 0xFF80 - 0xFFFE
    BYTE hram[0x07E];
};

#endif