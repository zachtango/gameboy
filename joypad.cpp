#include "joypad.h"


// https://gbdev.io/pandocs/Joypad_Input.html#ff00--p1joyp-joypad

/* KEY ACTION */
void Joypad::key_action(U32 key, bool pressed) {
    switch(key) {
        case START_INPUT:
            std::cout << "start pressed\n";
            start = pressed;
            break;
        case SELECT_INPUT:
            std::cout << "select pressed\n";
            select = pressed;
            break;
        case B_INPUT:
            std::cout << "b pressed\n";
            b = pressed;
            break;
        case A_INPUT:
            std::cout << "a pressed\n";
            a = pressed;
            break;
        case DOWN_INPUT:
            std::cout << "down pressed\n";
            down = pressed;
            break;
        case UP_INPUT:
            std::cout << "up pressed\n";
            up = pressed;
            break;
        case LEFT_INPUT:
            std::cout << "left pressed\n";
            left = pressed;
            break;
        case RIGHT_INPUT:
            std::cout << "right pressed\n";
            right = pressed;
            break;
        default:
            std::cerr << "Action on unknown key\n";
            throw "Action on unknown key";
    }
}

/* MEMORY FUNCTIONS */
BYTE Joypad::read(WORD address) {
    // lower 4 bits of register
    BYTE lo = 0;

    if(select_action) {
        lo = (
            (start << 3) |
            (select << 2) |
            (b << 1) |
            (a)
        );
    } else if(select_direction) {
        lo = (
            (down << 3) |
            (up << 2) |
            (left << 1) |
            (right)
        );
    }

    // 0xFF00 Joypad register
    if(address == 0xFF00) {
        // return bits inverted
        return ~(
            (0 << 7) |
            (0 << 6) |
            (select_action << 5) |
            (select_direction << 4) |
            lo
        );
    }
    
    std::cerr << std::hex << address << " Address not in Joypad range\n";
    throw "Address not in Joypad range";
}

void Joypad::write(WORD address, BYTE value) {
    // 0xFF00 Joypad register
    if(address == 0xFF00) {
        // save bits inverted
        // bit 5
        select_action = !get_bit(value, 5);
        // bit 4
        select_direction = !get_bit(value, 4);
        
        return;
    }

    std::cerr << std::hex << address << " Address not in Joypad range\n";
    throw "Address not in Joypad range";
}
