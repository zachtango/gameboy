#ifndef HELPERS_H
#define HELPERS_H

#include "constants.h"

BYTE msb(WORD w);
BYTE lsb(WORD w);
WORD concat(BYTE _msb, BYTE _lsb);
bool get_bit(BYTE b, U32 bit);
BYTE set_bit(BYTE b, U32 bit, bool on);

void print_byte(BYTE b);

#endif