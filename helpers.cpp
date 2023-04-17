#include "helpers.h"

BYTE msb(WORD w) {
    return (w & 0xF0) >> 8;
}

BYTE lsb(WORD w) {
    return (w & 0x0F);
}

WORD concat(BYTE _msb, BYTE _lsb) {
    WORD c = (_msb << 8) | _lsb;

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
    
    return b;
}