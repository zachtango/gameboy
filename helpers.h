#include "constants.h"

UINT _add(BYTE a, BYTE b) {

}

UINT _add(WORD a, WORD b) {

}

UINT _and(BYTE a, BYTE b) {

}

UINT _sub(BYTE a, BYTE b) {

}

UINT _sub(WORD a, WORD b) {

}

UINT _or(BYTE a, BYTE b) {

}

UINT _xor(BYTE a, BYTE b) {

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

void set_bit(BYTE &b, UINT bit, bool on) {
    if(on)
        b |= (1u << bit);
    else 
        b &= ~(1u << bit);
}