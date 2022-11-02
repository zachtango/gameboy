
#include <gameboy.h>

uint8_t *Gameboy::register8(uint8_t r){
    /*
        r8 table is
        0 B
        1 C
        2 D
        3 E
        4 H
        5 L
        6 (HL)
        7 A
    */

    switch(r){
        case 0:
            return &R[B];
        case 1:
            return &R[C];
        case 2:
            return &R[D];
        case 3:
            return &R[E];
        case 4:
            return &R[H];
        case 5:
            return &R[L];
        // case 6:
        //     return &M[ (R[H] << 8u) | R[L] ];
        case 7:
            return &R[A];
    }

    return nullptr;
}

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

// ARITHMETIC instructions

// 8 bit ARITHMETIC
// FIXME CHECK FLAGS

uint8_t Gameboy::add_ar(){

    uint8_t r = opcode & 0x0F;

    R[A] += R[r];

    return 4;
}

uint8_t Gameboy::add_an(){
    
    uint8_t n = readByte();

    R[A] += n;

    return 8;
}

uint8_t Gameboy::add_ahl(){

    uint16_t hl = (R[H] << 8u) | R[L];

    R[A] += M[hl];

    return 8;
}

uint8_t Gameboy::adc_ar(){

    uint8_t r = opcode & 0x0F;

    R[A] += R[r]; // + cy carry flag

    return 4;
}

uint8_t Gameboy::adc_an(){

    uint8_t n = readByte();

    R[A] += n; // + cy carry flag

    return 8;
}

uint8_t Gameboy::adc_ahl(){

    uint16_t hl = (R[H] << 8u) | R[L];

    R[A] += M[hl]; // + cy carry flag

    return 8;
}

uint8_t Gameboy::sub_ar(){

    uint8_t r = opcode & 0x0F;

    R[A] -= r;

    return 4;
}

uint8_t Gameboy::sub_an(){
    
    uint8_t n = readByte();

    R[A] -= n;

    return 8;
}

uint8_t Gameboy::sub_ahl(){

    uint16_t hl = (R[H] << 8u) | R[L];

    R[A] -= M[hl];

    return 8;
}

uint8_t Gameboy::sbc_ar(){

    uint8_t r = opcode & 0x0F;

    R[A] -= (R[r]); // + cy flag

    return 4;
}

uint8_t Gameboy::sbc_an(){

    uint8_t n = readByte();

    R[A] -= n; // + cy flag

    return 8;
}

uint8_t Gameboy::sbc_ahl(){

    uint16_t hl = (R[H] << 8u) | R[L];

    R[A] -= M[hl]; // + cy flag

    return 8;
}

uint8_t Gameboy::and_ar(){

    uint8_t r = opcode & 0x0F;

    R[A] &= R[r];

    return 4;
}

uint8_t Gameboy::and_an(){

    uint8_t n = readByte();

    R[A] &= n;

    return 8;
}

uint8_t Gameboy::and_ahl(){

    uint16_t hl = (R[H] << 8u) | R[L];

    R[A] &= M[hl];

    return 8;
}

uint8_t Gameboy::xor_ar(){

    uint8_t r = opcode & 0x0F;

    R[A] ^= R[r];

    return 4;
}

uint8_t Gameboy::xor_an(){

    uint8_t n = readByte();

    R[A] ^= n;

    return 8;
}

uint8_t Gameboy::xor_ahl(){

    uint16_t hl = (R[H] << 8u) | R[L];

    R[A] ^= M[hl];

    return 8;
}

uint8_t Gameboy::or_ar(){

    uint8_t r = opcode & 0x0F;

    R[A] |= R[r];

    return 4;
}

uint8_t Gameboy::or_an(){

    uint8_t n = readByte();

    R[A] |= n;

    return 8;
}

uint8_t Gameboy::or_ahl(){

    uint16_t hl = (R[H] << 8u) | R[L];

    R[A] |= M[hl];

    return 8;
}

uint8_t Gameboy::or_ahl(){

    uint16_t hl = (R[H] << 8u) | R[L];

    R[A] |= hl;

    return 8;
}

uint8_t Gameboy::cp_ar(){

    uint8_t r = opcode & 0x0F;

    // compare a - r change flags

    return 4;
}

uint8_t Gameboy::cp_an(){

    uint8_t n = readByte();

    // compare a - n change flags

    return 8;
}

uint8_t Gameboy::cp_ahl(){

    uint16_t hl = (R[H] << 8u) | R[L];

    // compare a - hl

    return 8;
}

uint8_t Gameboy::inc_r(){

    uint8_t r = opcode >> 3u;

    R[r] += 1;

    return 4;
}

uint8_t Gameboy::inc_hl(){

    uint16_t hl = (R[H] << 8u) | R[L];

    M[hl] += 1;

    return 12;
}

uint8_t Gameboy::dec_r(){

    uint8_t r = opcode >> 3u;

    R[r] -= 1;

    return 4;
}

uint8_t Gameboy::dec_hl(){

    uint16_t hl = (R[H] << 8u) | R[L];

    M[hl] -= 1;

    return 12;
}

uint8_t Gameboy::daa(){
    return 4;
}

uint8_t Gameboy::cpl(){

    return 4;
}

// END ARITHMETIC instructions

// CONTROL FLOW instructions

uint8_t Gameboy::jp_nn(){
    uint8_t lsb = readByte();
    uint8_t msb = readByte();

    pc = (msb << 8u) | lsb;

    return 16;
}

uint8_t Gameboy::jp_hl(){
    pc = (R[H] << 8u) | R[L];

    return 4;
}

uint8_t Gameboy::jp_ccnn(){
    uint8_t cc = (opcode >> 3u) & 0x3;

    bool flag;

    switch(cc){
        case 0x0:  // NZ
            // flag =
            break;
        case 0x1:  // Z
            break;
        case 0x2:  // NC
            break;
        case 0x3:  // C
    }

    uint8_t lsb = readByte();
    uint8_t msb = readByte();

    if(flag){
        pc = (msb << 8u) | lsb;

        return 16;
    }

    return 12;
}

uint8_t Gameboy::jr_e(){
    pc += readByte();

    return 12;
}

uint8_t Gameboy::jr_cce(){
    uint8_t cc = (opcode >> 3u) & 0x3;

    bool flag;
    
    switch(cc){
        case 0x0:  // NZ
            // flag =
            break;
        case 0x1:  // Z
            break;
        case 0x2:  // NC
            break;
        case 0x3:  // C
    }

    uint8_t e = readByte();

    if(flag){
        pc += e;

        return 12;
    }

    return 8;
}


uint8_t Gameboy::call_nn(){
    uint8_t lsb = readByte();
    uint8_t msb = readByte();

    sp -= 1;
    M[sp] = pc >> 8u;
    
    sp -= 1;
    M[sp] = (uint8_t) pc;

    pc = (msb << 8u) | lsb;

    return 24;
}

uint8_t Gameboy::call_ccnn(){
    uint8_t cc = (opcode >> 3u) & 0x3;

    uint8_t lsb = readByte();
    uint8_t msb = readByte();

    bool flag;

    switch(cc){
        case 0x0:
            break;
        case 0x1:
            break;
        case 0x2:
            break;
        case 0x3:
        
    }

    if(flag){
        sp -= 1;
        M[sp] = pc >> 8u;
        
        sp -= 1;
        M[sp] = (uint8_t) pc;

        pc = (msb << 8u) | lsb;

        return 24;
    }

    return 12;
}

uint8_t Gameboy::ret(){
    uint8_t lsb = readByte();
    uint8_t msb = readByte();

    pc = (msb << 8u) | lsb;

    return 16;
}

uint8_t Gameboy::ret_cc(){
    uint8_t cc = (opcode >> 3u) & 0x3;

    bool flag;

    switch(cc){
        case 0x0:
            break;
        case 0x1:
            break;
        case 0x2:
            break;
        case 0x3:
            
    }

    if(flag){
        uint8_t lsb = M[sp];
        sp += 1;

        pc = (lsb << 8u) | M[sp];
        sp += 1;

        return 20;
    }

    return 8;
}

uint8_t Gameboy::reti(){
    uint8_t lsb = readByte();
    uint8_t msb = readByte();

    pc = (msb << 8u) | lsb;

    IME = 1;

    return 16;
}

uint8_t Gameboy::rst_n(){
    // call to 00, 08, 10, 18, 20, 28, 30, 38
    uint8_t call[8] = {0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38};

    uint8_t xxx = (opcode >> 3u) & 0x3;

    pc = call[xxx];

    return 16;
}

// END CONTROL FLOW instructions