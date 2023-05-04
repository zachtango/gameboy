#include "timer.h"
#include "helpers.h"

void Timer::tick() {
    // https://gbdev.io/pandocs/Timer_and_Divider_Registers.html
    // http://pixelbits.16-b.it/GBEDG/timers/

    // increment DIV every T cycle
    div += 1;
    
    U32 d;

    // Clock Select (bit 1 - 0 of TAC) determines frequency of increment
    switch(tac & 0b11) {
        case 0b00:
            // every 1024 T cycles
            d = 9;
            break;
        case 0b01:
            // every 16 T cycles
            d = 3;
            break;
        case 0b10:
            // every 64 T cycles
            d = 5;
            break;
        case 0b11:
            // every 256 T cycles
            d = 7;
    }

    // bit d of DIV is ANDed with Timer Enable bit (bit 2 of TAC)
    bool this_cycle = get_bit(div, d) && get_bit(tac, 2);

    // increment only on the falling edge (1 in the last cycle and 0 in this cycle)
    if(last_cycle && !this_cycle) {
        tima += 1;

        if(tima == 0) {
            // set TIMA = TMA after overflow
            tima = tma;

            // send TIMER interrupt
            interrupts.request_interrupt(TIMER_INTERRUPT);
        }
    }
}


/* MEMORY FUNCTIONS */
BYTE Timer::read(WORD address) {
    // 0xFF04 Divider register
    if(address == 0xFF04) {
        // Return high byte of DIV
        return div >> 8;
    }

    BYTE *p = memory_map(address);
    return *p;
}

void Timer::write(WORD address, BYTE value) {
    // 0xFF04 Divider register
    if(address == 0xFF04) {
        // write to DIV --> reset register to 0
        div = 0;
        return;
    }

    BYTE *p = memory_map(address);
    *p = value;
}

/* Given 16 bit address
    Return pointer to the BYTE in memory the address refers to
*/
BYTE* Timer::memory_map(WORD address) {
    switch(address) {
        // 0xFF05 Timer counter
        case 0xFF05:
            return &tima;
        
        // 0xFF06 Timer modulo
        case 0xFF06:
            return &tma;

        // 0xFF07 Timer control
        case 0xFF07:
            return &tac;
    }

    throw "Address not in Timer range";
}