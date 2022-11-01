
#include <gameboy.h>




// LOAD Instructions

// 8 bit load instructions

void Gameboy::loadRR(uint8_t r1, uint8_t r2){
    // opcode 0x41
    R[r1] = R[r2];
}

void Gameboy::loadRN(uint8_t r, uint8_t n){
    // opcode 0x06
    R[r] = n;
}

void Gameboy::loadRHL(uint8_t r, uint16_t HL){
    // opcode 0x46
    R[r] = M[R[HL]];
}

void Gameboy::loadHLR(uint16_t HL, uint8_t r){
    // opcode 0x70
    M[R[HL]] = R[r];
}

void Gameboy::loadHLN(uint16_t HL, uint8_t n){
    // opcode 0x36
    M[R[HL]] = n;
}

void Gameboy::loadABC(uint8_t A, uint16_t BC){
    // opcode 0x0A
    R[A] = M[R[BC]];
}

void Gameboy::loadADE(uint8_t A, uint16_t DE){
    // opcode 0x1A
    R[A] = M[R[DE]];
}

void Gameboy::loadBCA(uint16_t BC, uint8_t A){
    // opcode 0x02
    R[R[BC]] = R[A];
}

void Gameboy::loadDEA(uint16_t DE, uint8_t A){
    // opcode 0x12
    R[R[DE]] = R[A];
}

void Gameboy::loadANN(uint8_t A, uint16_t NN){
    // opcode 0xFA
    R[A] = M[NN];
}

void Gameboy::loadNNA(uint16_t NN, uint8_t A){
    // opcode 0xEA
    M[NN] = R[A];
}

void Gameboy::loadhAC(uint8_t A, uint8_t C){
    // opcode 0xF2
    R[A] = M[0xFF00 | R[C]];
}

void Gameboy::loadhCA(uint8_t C, uint8_t A){
    // opcode 0xE2
    M[0xFF00 | R[C]] = R[A];
}

void Gameboy::loadhAN(uint8_t A, uint8_t N){
    // opcode 0xF0
    R[A] = M[0xFF00 | N];
}


void Gameboy::loadhNA(uint8_t N, uint8_t A){
    // opcode 0xE0
    M[0xFF00 | N] = R[A];
}

void Gameboy::loaddAHL(uint8_t A, uint16_t HL){
    // opcode 0x3A
    R[A] = R[HL];

    R[HL] -= 1;
}

void Gameboy::loaddHLA(uint16_t HL, uint8_t A){
    // opcode 0x32
    R[HL] = R[A];

    R[HL] -= 1;
}

void Gameboy::loadiAHL(uint8_t A, uint16_t HL){
    // opcode 0x2A
    R[A] = R[HL];

    R[HL] += 1;
}

void Gameboy::loadiHLA(uint16_t HL, uint8_t A){
    // opcode 0x22
    R[HL] = R[A];

    R[HL] += 1;
}

// 16 bit load instructions

void Gameboy::loadRRNN(uint16_t RR, uint16_t NN){
    // opcode 0x01
    R[0xF0 & RR] = 0xF0 & NN;
    R[0x0F & RR] = 0x0F & NN;
}

void Gameboy::loadNNSP(uint16_t NN){
    // opcode 0x08
    M[NN] = 0xF0 & sp;
    M[NN + 1] = 0x0F & sp;
}

void Gameboy::loadSPHL(uint16_t HL){
    // opcode 0xF9
    sp = R[HL];
}

void Gameboy::pushRR(uint16_t RR){
    // opcode 0xC5
    sp -= 1;
    M[sp] = R[0xF0 & RR];
    
    sp -= 1;
    M[sp] = R[0x0F & RR];
}

void Gameboy::popRR(uint16_t RR){
    // opcode 0xC1
    R[0x0F & RR] = M[sp];
    sp += 1;

    R[0xF0 & RR] = M[sp];
    sp += 1;
}

// CONTROL FLOW instructions

void Gameboy::jumpNN(uint16_t NN){
    // opcode 0xC3
    pc = NN;
}

void Gameboy::jumpHL(uint16_t HL){
    // opcode 0xe9
    pc = R[HL];
}

void Gameboy::jumpCCNN(uint16_t CC, uint16_t NN){
    // opcode 0xC2 0xD2 0xCA 0xDA

    bool flag;
    // CC IN THE OPCODE
    switch(CC){
        case 0x00:  // NZ
            // flag =
            break;
        case 0x01:  // Z
            break;
        case 0x02:  // NC
            break;
        case 0x03:  // C
    }

    if(flag){
        pc = NN;
    }
}

void Gameboy::jumprE(uint8_t E){
    // opcode 0x18
    pc += E;
}

void Gameboy::jumprCCE(uint16_t CC, uint8_t E){
    // opcode 0x20 0x30 0x28 0x38
    bool flag;
    // CC IN THE OPCODE
    switch(CC){
        case 0x00:  // NZ
            // flag =
            break;
        case 0x01:  // Z
            break;
        case 0x02:  // NC
            break;
        case 0x03:  // C
    }

    if(flag){
        pc += E;
    }
}

void Gameboy::callNN(uint16_t NN){
    // opcode 0xCD
    sp -= 1;
    M[sp] = 0x0F & NN;
    
    sp -= 1;
    M[sp] = 0xF0 & NN;

    pc = NN;
}

void Gameboy::callCCNN(uint16_t CC, uint16_t NN){
    // opcode 0xC4 0xD4 0xCC 0xDC

    bool flag;

    switch(CC){

    }

    if(flag){
        sp -= 1;
        M[sp] = 0x0F & NN;
        
        sp -= 1;
        M[sp] = 0xF0 & NN;

        pc = NN;
    }
}

void Gameboy::ret(){
    // opcode 0xC9
    uint8_t lsb = M[sp];
    sp += 1;

    pc = (lsb << 8u) | M[sp];
    sp += 1;
}

void Gameboy::retCC(uint16_t CC){
    // opcode 0xC0 0xD0 0xC8 0xD8

    bool flag;

    switch(CC){

    }

    if(flag){
        uint8_t lsb = M[sp];
        sp += 1;

        pc = (lsb << 8u) | M[sp];
        sp += 1;
    }
}

void Gameboy::reti(){
    // opcode 0xD9
    // interrupts by setting IME = 1

    uint8_t lsb = M[sp];
    sp += 1;

    pc = (lsb << 8u) | M[sp];
    sp += 1;

    // IME = 1;
}

void Gameboy::rstN(uint8_t N){
    // opcode 0xC7 0xD7 0xE7 0xF7 0xCF 0xDF 0xEF 0xFF
    
    sp -= 1;
    M[sp] = 0xF0 & pc;

    sp -= 1;
    M[sp] = 0x0F & pc;

    pc = N;
}

// MISC instructions




// END MISC instructions