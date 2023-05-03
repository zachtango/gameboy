#include "timer.h"
#include "helpers.h"


void Timer::increment() {
    if(*mmu.reset_div) {
        div = 0;
        *mmu.reset_div = false;
    }

    WORD prev_div = div;

    // http://pixelbits.16-b.it/GBEDG/timers/
    /*
        Bit  2   - Timer Enable
        Bits 1-0 - Input Clock Select
           00: CPU Clock / 1024 (DMG:   4096 Hz)
           01: CPU Clock / 16   (DMG: 262144 Hz)
           10: CPU Clock / 64   (DMG:  65536 Hz)
           11: CPU Clock / 256  (DMG:  16384 Hz)
    */

    div += 1;
    mmu.M[DIV_ADDRESS] = div >> 8;

    tac = mmu.read(TAC_ADDRESS);

    if(!get_bit(tac, 2))
        return;

    // Increment at freq of TAC
    static U32 freq_map[] = {
        9, 3, 5, 7
    };
    
    bool timer_update = ( prev_div & (1 << freq_map[tac & 0b11]) ) && !( div & (1 << freq_map[tac & 0b11]) );
    // std::cout << timer_update << '\n';
    if(timer_update) {
        tima += 1;
        
        if(tima == 0x00) {
            tma = mmu.read(TMA_ADDRESS);
            tima = tma;
            // std::cout << "timer interrupt sent\n";
            // send timer interrupt
            mmu.write(
                0xFF0F,
                mmu.read(0xFF0F) | 0b00100 // timer bit
            );
        }

        mmu.write(TIMA_ADDRESS, tima);
    }
}
