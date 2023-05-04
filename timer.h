#ifndef TIMER_H
#define TIMER_H

#include "mmu.h"
#include "constants.h"

#define DIV_ADDRESS 0xFF04
#define TIMA_ADDRESS 0xFF05
#define TMA_ADDRESS 0xFF06
#define TAC_ADDRESS 0xFF07

#include "interrupts.h"

class Interrupts;

class Timer {
public:
    Timer(Interrupts &interrupts) : interrupts(interrupts) {
        // https://gbdev.io/pandocs/Timer_and_Divider_Registers.html
        // http://pixelbits.16-b.it/GBEDG/timers/

        last_cycle = 0;

        div = 0;
        tima = 0;
        tma = 0;
        tac = 0;
    }
    
    /* MASTER TICK LOOP 1 T Cycle / tick */
    void tick();
    
    /* MEMORY FUNCTIONS */
    BYTE read(WORD address);
    void write(WORD address, BYTE value);

private:
    Interrupts &interrupts;

    /* MEMORY FUNCTIONS HELPER */
    BYTE *memory_map(WORD address);

    /* STORE DIV && Timer Enable FROM LAST CYCLE */
    bool last_cycle;

    /* TIMER REGISTERS */
    
    // 0xFF04 Divider register
    WORD div;
    
    // 0xFF05 Timer counter
    BYTE tima;
    
    // 0xFF06 Timer modulo
    BYTE tma;
    
    // 0xFF07 Timer control
    BYTE tac;
};

#endif