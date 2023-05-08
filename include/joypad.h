#ifndef JOYPAD_H
#define JOYPAD_H

#include "constants.h"
#include "helpers.h"

#define START_INPUT 7
#define SELECT_INPUT 6
#define B_INPUT 5
#define A_INPUT 4
#define DOWN_INPUT 3
#define UP_INPUT 2
#define LEFT_INPUT 1
#define RIGHT_INPUT 0

class Joypad {
public:
    
    Joypad() {
        // https://gbdev.io/pandocs/Joypad_Input.html#ff00--p1joyp-joypad

        /*
            In the docs, 0 = pressed, 0 = select
                This is due to how the hardware works
                To make it less confusing for me, I use 1 = pressed, 1 = select
                    and return the inverted representation on read / store
                    the inverted representation on write.
        */

        select_action = false;
        select_direction = false;
        
        start = false;
        select = false;
        b = false;
        a = false;

        down = false;
        up = false;
        left = false;
        right = false;
    }

    /* KEY ACTION */
    void key_action(U32 key, bool pressed);

    /* MEMORY FUNCTIONS */
    BYTE read(WORD address);
    void write(WORD address, BYTE value);

private:
    /* 0xFF00 JOYPAD REGISTER */

    /* R/W BITS */
    // bit 5
    bool select_action;
    // bit 4
    bool select_direction;
    
    /* READ ONLY BITS */
    // bit 3
    bool start;
    // bit 2
    bool select;
    // bit 1
    bool b;
    // bit 0
    bool a;

    // bit 3
    bool down;
    // bit 2
    bool up;
    // bit 1
    bool left;
    // bit 0
    bool right;
};


#endif