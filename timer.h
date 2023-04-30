#include "mmu.h"
#include "constants.h"

#define DIV_ADDRESS 0xFF04
#define TIMA_ADDRESS 0xFF05
#define TMA_ADDRESS 0xFF06
#define TAC_ADDRESS 0xFF07

class Timer {
public:
    Timer(MMU &mmu) : mmu(mmu) {
        /*
            https://gbdev.io/pandocs/Timer_and_Divider_Registers.html
        */

        div = 0;
        tima = 0;
        tma = 0;
        tac = 0;
    }
    
    void increment();

    MMU mmu;

    WORD div;
    BYTE tima;
    BYTE tma;
    BYTE tac;
};
