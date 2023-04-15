#include "interrupt.h"


void Interrupt::request_interrupt(BYTE interrupt) {
    interrupt = _interrupt(interrupt);

    BYTE ie = mmu.read(IE);

    ie = set_bit(ie, interrupt, 1);

    mmu.write(IE, ie);

}

void Interrupt::handle_interrupts() {

    BYTE interrupt;

    _handle_interrupt(interrupt);
    

}

void Interrupt::_handle_interrupt(BYTE interrupt) {
    interrupt = _interrupt(interrupt);

    BYTE ie = mmu.read(IE);
    
    ie = set_bit(ie, interrupt, 0);

    mmu.write(IE, ie);

    cpu.IME = 0;

    /*
        1. 2 wait states (8 T Cycles)
        2. PC pushed onto stack (8 T Cycles)
        3. PC register set to address of handler ($40, $48, $50, $58, $60) (4 T Cycles)
    */

    switch(interrupt) {
        case VBlank:
            cpu._call(0x40);
            break;
        case LCD:
            cpu._call(0x48);
            break;
        case Timer:
            cpu._call(0x50);
            break;
        case Serial:
            cpu._call(0x58);
            break;
        case Joypad:
            cpu._call(0x60);
            break;
        default:
            throw "Unknown interrupt";
    }
}


