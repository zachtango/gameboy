#include "constants.h"

BYTE _add(BYTE a, BYTE b) {

}

WORD _add(WORD a, WORD b) {

}

BYTE _and(BYTE a, BYTE b) {

}

BYTE _sub(BYTE a, BYTE b) {

}

BYTE _sub(WORD a, WORD b) {

}

BYTE _or(BYTE a, BYTE b) {

}

BYTE _xor(BYTE a, BYTE b) {

}

bool get_carry() {

}

BYTE msb(WORD w) {
    return (w & 0xF0) >> 8;
}

BYTE lsb(WORD w) {
    return (w & 0x0F);
}

WORD concat(BYTE msb, BYTE lsb) {
    WORD c = (msb << 8) | lsb;

    return c;
}

bool get_bit(BYTE b, UINT bit) {
    return (b >> bit) & 0x01;
}

BYTE set_bit(BYTE b, UINT bit, bool on) {
    if(on)
        b |= (1u << bit);
    else 
        b &= ~(1u << bit);
}

BYTE _swap(BYTE b) {
    return ((b & 0x0F) << 4u) | ((b & 0xF0) >> 4u);
}

BYTE _rotate_left(BYTE b) {

}

BYTE _rotate_right(BYTE b) {

}

BYTE _shift_left(BYTE b) {

}

BYTE _shift_right(BYTE b) {
    
}