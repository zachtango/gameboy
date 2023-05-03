#ifndef MMU_H
#define MMU_H
#include <iostream>
#include "constants.h"
#include "joypad.h"

#define MEMORY_BYTES 0x10000

class MMU {
public:
    

private:
    // 0xC000 - 0xDFFF
    BYTE wram[0x2000];
};

#endif