#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "cpu.h"

#define VBLANK_INTERRUPT 0b00001
#define LCD_STAT_INTERRUPT 0b00010
#define TIMER_INTERRUPT 0b00100
#define SERIAL_INTERRUPT 0b01000
#define JOYPAD_INTERRUPT 0b10000

class CPU;

class Interrupts {
public:
    Interrupts() {
        // https://gbdev.io/pandocs/Interrupts.html

        interrupt_enable = 0;
        interrupt_flag = 0;
        std::cout << "INTERRUPTS INITIALIZED\n";
    }

    U32 handle_interrupt(CPU &cpu);
    void request_interrupt(BYTE interrupt_mask);

    /* MEMORY FUNCTIONS */
    BYTE read(WORD address);
    void write(WORD address, BYTE value);

private:
    /* MEMORY FUNCTIONS HELPER */
    BYTE *memory_map(WORD address);
    
    /* 0xFFFF
        Bit 4: VBlank 0x40
        Bit 3: LCD STAT 0x48
        Bit 2: Timer 0x50
        Bit 1: Serial 0x58
        Bit 0: Joypad 0x60
    */
    BYTE interrupt_enable;

    /* 0xFF0F
        Same bits as Interrupt enable
    */
    BYTE interrupt_flag;
};


#endif