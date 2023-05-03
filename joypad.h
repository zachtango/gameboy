#ifndef JOYPAD_H
#define JOYPAD_H

#include "constants.h"
#include "helpers.h"

class Joypad {
public:
    
    Joypad() {
        p1 = 0xFF;
    }

    BYTE p1;

    void press_key(U32 bit) { // bit 0 - 3 right, left, up, down, 4 - 7 A, B, Select, Start
        p1 &= ~(1 << bit);
    }

    void release_key(U32 bit) {
        p1 |= (1 << bit);
    }
};


#endif