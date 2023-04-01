#include <iostream>

typedef unsigned char BYTE; // 1 byte
typedef char SIGNED_BYTE; // 1 byte
typedef unsigned short WORD; // 2 bytes
typedef signed short SIGNED_WORD; // 2 bytes

#define A 0
#define F 1
#define B 2
#define C 3
#define D 4
#define E 5
#define H 6
#define L 7

class CPU {



private:
    WORD sp;
    WORD pc;
    
    /*
        Registers A, B, C, D, E, H, L
    */
    WORD R[7];

    /*
        reference: https://rgbds.gbdev.io/docs/v0.6.1/gbz80.7/
    */
    // 8 bit Arithmetic and Logic Instructions


    // 16 bit Arithmetic Instructions

    // Bit Operations Instructions

    // Bit Shift Instructions

    // Load Instructions

    // Jump and Subroutines

    // Stack Operations Instructions

    // Miscellaneous Instructions


    
};

