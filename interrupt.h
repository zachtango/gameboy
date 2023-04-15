#include "mmu.h"
#include "constants.h"
#include "cpu.h"
#include "helpers.h"

#define IE 0xFFFF
#define IF 0xFF0F

#define VBlank 0
#define LCD 1
#define Timer 2
#define Serial 3
#define Joypad 4


class Interrupt {
public:
    void request_interrupt(BYTE);
    void handle_interrupts();

private:

    /*
        0xFFFF Interrupt Enable flag

        

        0xFF0F Interrupt flag

        

    */
    
    MMU &mmu;
    CPU &cpu;

    UINT _interrupt(BYTE);

    void _handle_interrupt(BYTE);

};

