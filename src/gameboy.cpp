
#include <gameboy.h>


void Gameboy::step(){
    // update timer
    // instr

    // first instr at 0 (booting)

    // first instr for cartridges at 0x0100
    
    uint8_t cycles;
    
    // fetch instr PC
    opcode = readByte();

    // decode instr function table


    // execute instr


    t_cycles += cycles;

}

uint8_t Gameboy::readByte(){
    return M[pc++];
}

// LOAD Instructions

// 8 bit load instructions

// assume registers 0 - 7 correspond to B, C, D, E, H, L, A, F
uint8_t Gameboy::ld_rr(){
    uint8_t r1 = (opcode >> 3u) & 0x7;
    uint8_t r2 = opcode & 0x7;

    R[r1] = R[r2];

    return 4; // num t-cycles
}

uint8_t Gameboy::ld_rn(){
    uint8_t r = (opcode >> 3u) & 0x7;
    uint8_t n = readByte();

    R[r] = n;

    return 8; 
}

uint8_t Gameboy::ld_rhl(){
    uint8_t r = (opcode >> 3u) & 0x7;
    uint16_t hl = (R[H] << 8u) | R[L];

    R[r] = M[hl];

    return 8;
}

uint8_t Gameboy::ld_hlr(){
    uint8_t r = (opcode >> 3u) & 0x7;
    uint16_t hl = (R[H] << 8u) | R[L];

    M[hl] = R[r];

    return 8;
}

uint8_t Gameboy::ld_hln(){
    uint16_t hl = (R[H] << 8u) | R[L];
    uint8_t n = readByte();

    M[hl] = M[n];

    return 12;
}

uint8_t Gameboy::ld_abc(){
    uint16_t bc = (R[B] << 8u) | R[C];

    R[A] = M[bc];

    return 8;
}

uint8_t Gameboy::ld_ade(){
    uint16_t de = (R[D] << 8u) | R[E];

    R[A] = M[de];

    return 8;
}

uint8_t Gameboy::ld_bca(){
    uint16_t bc = (R[B] << 8u) | R[C];

    M[bc] = R[A];

    return 8;
}

uint8_t Gameboy::ld_dea(){
    uint16_t de = (R[D] << 8u) | R[E];

    M[de] = R[A];

    return 8;
}

uint8_t Gameboy::ld_ann(){
    uint8_t lsb = readByte();
    uint8_t msb = readByte();

    uint16_t nn = (msb << 8u) | lsb;

    R[A] = M[nn];

    return 16;
}

uint8_t Gameboy::ld_nna(){
    uint8_t lsb = readByte();
    uint8_t msb = readByte();

    uint16_t nn = (msb << 8u) | lsb;

    M[nn] = R[A];

    return 16;
}

uint8_t Gameboy::ldh_ac(){
    uint16_t s = 0xFF00 | R[C];

    R[A] = R[s];

    return 8;
}

uint8_t Gameboy::ldh_ca(){
    uint16_t d = 0xFF00 | R[C];

    M[d] = R[A];

    return 8;
}

uint8_t Gameboy::ldh_an(){
    uint8_t n = readByte();
    uint16_t s = 0xFF00 | n;

    R[A] = M[s];

    return 12;
}

uint8_t Gameboy::ldh_na(){
    uint8_t n = readByte();
    uint16_t d = 0xFF00 | n;

    M[d] = R[A];

    return 12;
}

uint8_t Gameboy::ldd_ahl(){
    uint16_t hl = (R[H] << 8u) | R[L];

    R[A] = M[hl];

    hl -= 1;

    R[H] = hl >> 8u;
    R[L] = (uint8_t) hl; // first byte truncated

    return 8;
}

uint8_t Gameboy::ldd_hla(){
    uint16_t hl = (R[H] << 8u) | R[L];

    M[hl] = R[A];

    hl -= 1;

    R[H] = hl >> 8u;
    R[L] = (uint8_t) hl; // first byte truncated

    return 8;
}

uint8_t Gameboy::ldi_ahl(){
    uint16_t hl = (R[H] << 8u) | R[L];

    R[A] = M[hl];

    hl -= 1;

    R[H] = hl >> 8u;
    R[L] = (uint8_t) hl; // first byte truncated

    return 8;
}

uint8_t Gameboy::ldi_hla(){
    uint16_t hl = (R[H] << 8u) | R[L];

    M[hl] = R[A];

    hl -= 1;

    R[H] = hl >> 8u;
    R[L] = (uint8_t) hl; // first byte truncated

    return 8;
}

// 16 bit load instructions

uint8_t Gameboy::ld_rrnn(){
    uint8_t rr = (opcode >> 4u);
    // 0, 1, 2, 3 corresponds to BC, DE, HL, SP

    uint8_t lsb = readByte();
    uint8_t msb = readByte();

    switch(rr){
        case 0:
            R[B] = msb;
            R[C] = lsb;
            break;
        case 1:
            R[D] = msb;
            R[E] = lsb;
            break;
        case 2:
            R[H] = msb;
            R[L] = lsb;
            break;
        case 3:
            sp = (msb << 8u) | lsb;
    }

    return 12;
}

uint8_t Gameboy::ld_nnsp(){
    uint8_t lsb = readByte();
    uint8_t msb = readByte();

    uint16_t nn = (msb << 8u) | lsb;

    M[nn] = (uint8_t) sp;
    M[nn + 1] = (sp >> 8u);    

    return 20;
}

uint8_t Gameboy::ld_sphl(){
    sp = (R[H] << 8u) | R[L];

    return 8;
}

uint8_t Gameboy::push_rr(){
    uint8_t rr = (opcode >> 4u) & 0x3;
    // 0, 1, 2, 3 corresponds to bc, de, hl, af
    sp -= 1;

    uint8_t msb, lsb;

    switch(rr){
        case 0:
            msb = R[B];
            lsb = R[C];
            break;
        case 1:
            msb = R[D];
            lsb = R[E];
            break;
        case 2:
            msb = R[H];
            lsb = R[L];
            break;
        case 3:
            msb = R[A];
            lsb = R[F];
    }

    M[sp] = msb;
    sp -= 1;
    M[sp] = lsb;

    return 16;
}

uint8_t Gameboy::pop_rr(){
    uint8_t rr = (opcode >> 4u) & 0x3;

    uint8_t lsb = M[sp];
    sp += 1;

    uint8_t msb = M[sp];
    sp += 1;

    R[B] = msb;
    R[C] = lsb;

    return 12;
}

// CONTROL FLOW instructions

void Gameboy::jp_nn(uint16_t NN){
    // opcode 0xC3
    pc = NN;
}

void Gameboy::jp_hl(uint16_t HL){
    // opcode 0xe9
    pc = R[HL];
}

void Gameboy::I_jumpCCNN(uint16_t CC, uint16_t NN){
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

void Gameboy::I_jumprE(uint8_t E){
    // opcode 0x18
    pc += E;
}

void Gameboy::I_jumprCCE(uint16_t CC, uint8_t E){
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

void Gameboy::I_callNN(uint16_t NN){
    // opcode 0xCD
    sp -= 1;
    M[sp] = 0x0F & NN;
    
    sp -= 1;
    M[sp] = 0xF0 & NN;

    pc = NN;
}

void Gameboy::I_callCCNN(uint16_t CC, uint16_t NN){
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

void Gameboy::I_ret(){
    // opcode 0xC9
    uint8_t lsb = M[sp];
    sp += 1;

    pc = (lsb << 8u) | M[sp];
    sp += 1;
}

void Gameboy::I_retCC(uint16_t CC){
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

void Gameboy::I_reti(){
    // opcode 0xD9
    // interrupts by setting IME = 1

    uint8_t lsb = M[sp];
    sp += 1;

    pc = (lsb << 8u) | M[sp];
    sp += 1;

    // IME = 1;
}

void Gameboy::I_rstN(uint8_t N){
    // opcode 0xC7 0xD7 0xE7 0xF7 0xCF 0xDF 0xEF 0xFF
    
    sp -= 1;
    M[sp] = 0xF0 & pc;

    sp -= 1;
    M[sp] = 0x0F & pc;

    pc = N;
}

// ARITHMETIC instructions

// 8 bit ARITHMETIC
void Gameboy::I_add(){  
    uint8_t operand;

    switch(opcode){
        case 0x87:
            operand = R[A];
            break;
        case 0x80:
            operand = R[B];
            break;
        case 0x81:
            operand = R[C];
            break;
        case 0x82:
            operand = R[D];
            break;
        case 0x83:
            operand = R[E];
            break;
        case 0x84:
            operand = R[H];
            break;
        case 0x85:
            operand = R[L];
            break;
        case 0x86:
            operand = M[ (R[H] << 8u) | R[L] ];
            break;
        case 0xC6:
            // operand = n
            // get n from M[pc + 1] (encoding)
    }

    // FIXME check flags

    R[0xA] += operand;

}

void Gameboy::I_adc(){
    uint8_t operand;

    switch(opcode){
        case 0x8F:
            operand = R[A];
            break;
        case 0x88:
            operand = R[B];
            break;
        case 0x89:
            operand = R[C];
            break;
        case 0x8A:
            operand = R[D];
            break;
        case 0x8B:
            operand = R[E];
            break;
        case 0x8C:
            operand = R[H];
            break;
        case 0x8D:
            operand = R[L];
            break;
        case 0x8E:
            operand = M[ (R[H] << 8u) | R[L] ];
            break;
        case 0xCE:
            // operand = n
            // get n from M[pc + 1] (encoding)
    }

    //operand += carry flag;

    // FIXME check flags

    R[0xA] += operand;

}

void Gameboy::I_sub(){
    uint8_t operand;

    switch(opcode){
        case 0x97:
            operand = R[A];
            break;
        case 0x90:
            operand = R[B];
            break;
        case 0x91:
            operand = R[C];
            break;
        case 0x92:
            operand = R[D];
            break;
        case 0x93:
            operand = R[E];
            break;
        case 0x94:
            operand = R[H];
            break;
        case 0x95:
            operand = R[L];
            break;
        case 0x96:
            operand = M[ (R[H] << 8u) | R[L] ];
            break;
        case 0xD6:
            // operand = n
            // get n from M[pc + 1] (encoding)
    }

    // FIXME check flags affected

    R[A] -= operand;
}

void Gameboy::I_sbc(){

    uint8_t operand;

    switch(opcode){
        case 0x9F:
            operand = R[A];
            break;
        case 0x98:
            operand = R[B];
            break;
        case 0x99:
            operand = R[C];
            break;
        case 0x9A:
            operand = R[D];
            break;
        case 0x9B:
            operand = R[E];
            break;
        case 0x9C:
            operand = R[H];
            break;
        case 0x9D:
            operand = R[L];
            break;
        case 0x9E:
            operand = M[ (R[H] << 8u) | R[L] ];
            break;
        case 0xDE:
            // operand = n
            // get n from M[pc + 1] (encoding)
    }
    
    // operand += carry flag

    // FIXME check flags affected

    R[A] -= operand;

}

void Gameboy::I_and(){

    uint8_t operand;

    switch(opcode){
        case 0xA7:
            operand = R[A];
            break;
        case 0xA0:
            operand = R[B];
            break;
        case 0xA1:
            operand = R[C];
            break;
        case 0xA2:
            operand = R[D];
            break;
        case 0xA3:
            operand = R[E];
            break;
        case 0xA4:
            operand = R[H];
            break;
        case 0xA5:
            operand = R[L];
            break;
        case 0xA6:
            operand = M[ (R[H] << 8u) | R[L] ];
            break;
        case 0xE6:
            // operand = n
            // get n from M[pc + 1] (encoding)
    }

    // FIXME check flags affected

    R[A] &= operand; // not sure if it's supposed to be bitwise or not

}

void Gameboy::I_or(){

    uint8_t operand;

    switch(opcode){
        case 0xB7:
            operand = R[A];
            break;
        case 0xB0:
            operand = R[B];
            break;
        case 0xB1:
            operand = R[C];
            break;
        case 0xB2:
            operand = R[D];
            break;
        case 0xB3:
            operand = R[E];
            break;
        case 0xB4:
            operand = R[H];
            break;
        case 0xB5:
            operand = R[L];
            break;
        case 0xB6:
            operand = M[ (R[H] << 8u) | R[L] ];
            break;
        case 0xF6:
            // operand = n
            // get n from M[pc + 1] (encoding)
    }

    // FIXME check flags affected

    R[A] |= operand; // not sure if it's supposed to be bitwise or not

}

void Gameboy::I_xor(){

    uint8_t operand;

    switch(opcode){
        case 0xAF:
            operand = R[A];
            break;
        case 0xA8:
            operand = R[B];
            break;
        case 0xA9:
            operand = R[C];
            break;
        case 0xAA:
            operand = R[D];
            break;
        case 0xAB:
            operand = R[E];
            break;
        case 0xAC:
            operand = R[H];
            break;
        case 0xAD:
            operand = R[L];
            break;
        case 0xAE:
            operand = M[ (R[H] << 8u) | R[L] ];
            break;
        case 0xEE:
            // operand = n
            // get n from M[pc + 1] (encoding)
    }

    // FIXME check flags affected

    R[A] ^= operand; // not sure if it's supposed to be bitwise or not

}

void Gameboy::I_cp(){
    uint8_t operand;

    switch(opcode){
        case 0xBF:
            operand = R[A];
            break;
        case 0xB8:
            operand = R[B];
            break;
        case 0xB9:
            operand = R[C];
            break;
        case 0xBA:
            operand = R[D];
            break;
        case 0xBB:
            operand = R[E];
            break;
        case 0xBC:
            operand = R[H];
            break;
        case 0xBD:
            operand = R[L];
            break;
        case 0xBE:
            operand = M[ (R[H] << 8u) | R[L] ];
            break;
        case 0xFE:
            // operand = n
            // get n from M[pc + 1] (encoding)
    }

    // FIXME check flags affected

    // compare A with n (A - n and set flags but throw away result)

}

void Gameboy::I_inc(){

    uint8_t *reg;

    switch(opcode){
        case 0x3C:
            reg = &R[A];
            break;
        case 0x04:
            reg = &R[B];
            break;
        case 0x0C:
            reg = &R[C];
            break;
        case 0x14:
            reg = &R[D];
            break;
        case 0x1C:
            reg = &R[E];
            break;
        case 0x24:
            reg = &R[H];
            break;
        case 0x2C:
            reg = &R[L];
            break;
        case 0x34:
            reg = &M[ (R[H] << 8u) | R[L] ];
    }

    // FIXME check flags affected
    // c not affected

    *reg += 1;

}

void Gameboy::I_dec(){

    uint8_t *reg;

    switch(opcode){
        case 0x3D:
            reg = &R[A];
            break;
        case 0x05:
            reg = &R[B];
            break;
        case 0x0D:
            reg = &R[C];
            break;
        case 0x15:
            reg = &R[D];
            break;
        case 0x1D:
            reg = &R[E];
            break;
        case 0x25:
            reg = &R[H];
            break;
        case 0x2D:
            reg = &R[L];
            break;
        case 0x35:
            reg = &M[ (R[H] << 8u) | R[L] ];
    }

    // FIXME check flags affected
    // c not affected

    *reg -= 1;
}

// 16 bit ARITHMETIC
void Gameboy::I_addHL(){
    uint16_t hl = (R[H] << 8u) | R[L]; 
    uint16_t operand;

    switch(opcode){
        case 0x09:
            operand = (R[B] << 8u) | R[C];
            break;
        case 0x19:
            operand = (R[D] << 8u) | R[E];
            break;
        case 29:
            operand = hl;
            break;
        case 39:
            operand = sp;
    }

    // set flags

    R[H] = (hl >> 8u);
    R[L] = (uint8_t) hl;
}

void Gameboy::I_addSP(){
    // opcode 0xE8
    uint8_t n;

    // get n from byte after opcode E8 n
    sp += n;
}

void Gameboy::I_incNN(){
    // increment double registers, stored as little endian
}

void Gameboy::I_dec(){
    // decrement double registers, stored as little endian
}

// END ARITHMETIC instructions


// MISC instructions




// END MISC instructions