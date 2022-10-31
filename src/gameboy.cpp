
#include <gameboy.h>




// LOAD Instructions

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