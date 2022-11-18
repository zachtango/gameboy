#include "cpu.h"

extern MMU mmu;

void CPU::writeFlags(uint8_t z, uint8_t n, uint8_t h, uint8_t c){
    if(z != -1) 
        regs.f = (regs.f & 0b01111111) | (z << 7u);

    if(n != -1) 
        regs.f = (regs.f & 0b10111111) | (n << 6u);

    if(h != -1)
        regs.f = (regs.f & 0b11011111) | (h << 5u);

    if(c != -1)
        regs.f = (regs.f & 0b11101111) | (c << 4u);
}

bool CPU::readZFlag(){
    return (regs.f >> 7u) & 0b1;
}

bool CPU::readNFlag(){
    return (regs.f >> 6u) & 0b1;
}

bool CPU::readHFlag(){
    return (regs.f >> 5u) & 0b1;
}

bool CPU::readCFlag(){
    return (regs.f >> 4u) & 0b1;
}

void CPU::ADD(uint16_t a, uint16_t b){
    writeFlags( -1, 0, (a & 0xFF) > ( 0xFF - (b & 0xFF) ), a > ( UINT16_MAX - b ) );
}

void CPU::ADD(uint8_t a, uint8_t b){
    writeFlags( (a + b) == 0, 0,  (a & 0x0F) > ( 0x0F - (b & 0x0F) ), a > ( UINT8_MAX - b ) );
}

void CPU::SUB(uint16_t a, uint16_t b){
    writeFlags( (a - b) == 0, 1, (a & 0xFF) < (b & 0xFF), a < b );
}

void CPU::SUB(uint8_t a, uint8_t b){
    writeFlags( (a - b) == 0, 1, (a & 0x0F) < (b & 0x0F), a < b);
}

void CPU::AND(uint8_t a, uint8_t b){
    writeFlags( (a & b) == 0, 0, 1, 0 );
}

void CPU::XOR(uint8_t a, uint8_t b){
    writeFlags( (a ^ b) == 0, 0, 0, 0 );
}

void CPU::OR(uint8_t a, uint8_t b){
    writeFlags( (a | b) == 0, 0, 0, 0 );
}

// referencing SM83_decoding-1.pdf in repo

void CPU::noi(){}
// 8 bit loads
void CPU::ld_r_r(){

    writeR(r8Dest, readR(r8Source));

}

void CPU::ld_r_n(){
    uint8_t n;

    writeR(r8Dest, n);

}

void CPU::ld_r_hl(){

    writeR(r8Dest, readR(r8Source));
}

void CPU::ld_hl_r(){

    writeR(r8Dest, readR(r8Source));
}

void CPU::ld_hl_n(){
    uint8_t n;

    writeR(r8Dest, n);
}

void CPU::ld_a_bc(){
    // fix me use enums
    writeR(0, mmu.readByte(readRR(r16, 2)));
}

void CPU::ld_a_de(){
    // fixme replace with ld_a_rr();
}

void CPU::ld_a_nn(){
    uint16_t nn;

    writeR(0, mmu.readByte(nn));
}

void CPU::ld_bc_a(){
    // fixme replace with ld_rr_a();
    mmu.writeByte(readRR(r16, 2), readR(0));
}

void CPU::ld_de_a(){
    // fixme replace with ld_rr_a();
    mmu.writeByte(readRR(r16, 2), readR(0));
}

void CPU::ld_nn_a(){
    uint16_t nn;

    mmu.writeByte(nn, readR(0));
}

void CPU::ld_a_ffn(){
    uint8_t n;

    mmu.writeByte(0xFF00 + n, readR(0));
}

void CPU::ld_ffn_a(){
    uint8_t n;

    writeR(0, mmu.readByte(0xFF00 + n));
}

void CPU::ld_a_ffc(){
    // fixme use enums
    writeR(0, mmu.readByte(0xFF00 + readR(2)));
}

void CPU::ld_ffc_a(){
    // fixme use enums
    mmu.writeByte(0xFF00 + readR(2), readR(0));
}

void CPU::ldi_hl_a(){
    // ld_rr_a();

    regs.hl += 1;
}

void CPU::ldi_a_hl(){
    // ld_a_rr();

    regs.hl += 1;
}

void CPU::ldd_hl_a(){
    // ld_rr_a();

    regs.hl -= 1;
}

void CPU::ldd_a_hl(){
    // ld_a_rr();

    regs.hl -= 1;
}

// 16 bit loads
void CPU::ld_rr_nn(){
    uint16_t nn;

    writeRR(r16, 1, nn);
}

void CPU::ld_nn_sp(){
    uint16_t nn;

    mmu.writeWord(nn, regs.sp);
}

void CPU::ld_sp_hl(){
    regs.sp = regs.hl;
}

void CPU::push_rr(){
    regs.sp -= 2;

    mmu.writeByte(regs.sp, readRR(r16, 3));
}

void CPU::pop_rr(){
    writeRR(r16, 3, mmu.readByte(regs.sp));

    regs.sp += 2;
}

// 8 bit ALU
void CPU::add_a_r(){
    uint8_t r = readR(r8Dest);
    
    // set add flags
    ADD(regs.a, r);

    regs.a += r;
}

void CPU::add_a_n(){
    uint8_t n;

    // set add flags
    ADD(regs.a, n);

    regs.a += n;
}

void CPU::add_a_hl(){
    uint8_t mhl = mmu.readByte(regs.hl);

    ADD(regs.a, mhl);

    regs.a += mhl;
}

void CPU::adc_a_r(){
    uint8_t b = readR(r8Dest) + readCFlag();

    ADD(regs.a, b);

    regs.a += b;
}

void CPU::adc_a_n(){
    uint8_t n;

    uint8_t b = n + readCFlag();

    ADD(regs.a, b);

    regs.a += b;
}

void CPU::adc_a_hl(){
    uint8_t b = mmu.readByte(regs.hl) + readCFlag();

    ADD(regs.a, b);

    regs.a += b;
}

void CPU::sub_a_r(){
    uint8_t r = readR(r8Dest);

    SUB(regs.a, r);

    regs.a -= r;
}

void CPU::sub_a_n(){
    uint8_t n;

    SUB(regs.a, n);

    regs.a -= n;
}

void CPU::sub_a_hl(){
    uint8_t mhl = mmu.readByte(regs.hl);

    SUB(regs.a, mhl);

    regs.a -= mhl;
}

void CPU::sbc_a_r(){
    uint8_t b = readR(r8Dest) + readCFlag();

    SUB(regs.a, b);

    regs.a -= b;
}

void CPU::sbc_a_n(){
    uint8_t n;
    uint8_t b = n + readCFlag();

    SUB(regs.a, b);

    regs.a -= b;
}

void CPU::sbc_a_hl(){
    uint8_t mhl = mmu.readByte(regs.hl);

    SUB(regs.a, mhl);

    regs.a -= mhl;
}

void CPU::and_a_r(){
    uint8_t r = readR(r8Dest);

    AND(regs.a, r);

    regs.a &= r;
}

void CPU::and_a_n(){
    uint8_t n;

    AND(regs.a, n);

    regs.a &= n;
}

void CPU::and_a_hl(){
    uint8_t mhl = mmu.readByte(regs.hl);

    AND(regs.a, mhl);

    regs.a &= mhl;
}

void CPU::xor_a_r(){
    uint8_t r = readR(r8Dest);

    XOR(regs.a, r);

    regs.a ^= r;
}

void CPU::xor_a_n(){
    uint8_t n;

    XOR(regs.a, n);

    regs.a ^= n;
}

void CPU::xor_a_hl(){
    uint8_t mhl = mmu.readByte(regs.hl);

    XOR(regs.a, mhl);

    regs.a ^= mhl;
}

void CPU::or_a_r(){
    uint8_t r = readR(r8Dest);

    OR(regs.a, r);

    regs.a |= r;
}

void CPU::or_a_n(){
    uint8_t n;;

    OR(regs.a, n);

    regs.a |= n;
}

void CPU::or_a_hl(){
    uint8_t mhl = mmu.readByte(regs.hl);

    OR(regs.a, mhl);

    regs.a |= mhl;
}

void CPU::cp_a_r(){
    uint8_t r = readR(r8Dest);

    SUB(regs.a, r);
}

void CPU::cp_a_n(){
    uint8_t n;

    SUB(regs.a, n);
}

void CPU::cp_a_hl(){
    uint8_t mhl = mmu.readByte(regs.hl);

    SUB(regs.a, mhl);
}

void CPU::inc_r(){
    uint8_t r = readR(r8Source);

    bool c = readCFlag();

    ADD(r, 1);

    writeFlags(-1, -1, -1, c);

    writeR(r8Source, r + 1);
}

void CPU::inc_hl(){
    uint8_t mhl = mmu.readByte(regs.hl);

    bool c = readCFlag();

    ADD(mhl, 1);
    writeFlags(-1, -1, -1, c);

    mmu.writeByte(regs.hl, mhl + 1);
}

void CPU::dec_r(){
    uint8_t r = readR(r8Source);

    bool c = readCFlag();

    SUB(r, 1);

    writeFlags(-1, -1, -1, c);

    writeR(r8Source, r - 1);
}

void CPU::dec_hl(){
    uint8_t mhl = mmu.readByte(regs.hl);

    bool c = readCFlag();

    SUB(mhl, 1);
    writeFlags(-1, -1, -1, c);

    mmu.writeByte(regs.hl, mhl - 1);
}

void CPU::daa(){

}

void CPU::cpl(){
    regs.a ^= 0xFF;

    writeFlags(-1, 1, 1, -1);
}

// 16 bit ALU
void CPU::add_hl_rr(){
    uint16_t rr = readRR(r16, 1);

    ADD(regs.hl, rr);

    regs.hl += rr;
}

void CPU::inc_rr(){
    uint16_t rr = readRR(r16, 1);

    writeRR(r16, 1, rr + 1);
}

void CPU::dec_rr(){
    uint16_t rr = readRR(r16, 1);

    writeRR(r16, 1, rr - 1);
}

void CPU::add_sp_dd(){
    int8_t dd;

    // if dd is negative subtract
    if(dd < 0) SUB(regs.sp, (uint16_t) (~dd + 1));
    else ADD(regs.sp, (uint16_t) dd);

    writeFlags(0, 0, -1, -1);

    regs.sp += dd;
}

void CPU::ld_hl_spdd(){
    int8_t dd;

    // if dd is negative subtract
    if(dd < 0) SUB(regs.sp, (uint16_t) (~dd + 1));
    else ADD(regs.sp, (uint16_t) dd);

    writeFlags(0, 0, -1, -1);

    regs.hl = regs.sp + dd;
}

// rotate and shift
void CPU::rlc_a(){}
void CPU::rl_a(){}
void CPU::rrc_a(){}
void CPU::rr_a(){}
void CPU::rlc_r(){}
void CPU::rlc_hl(){}
void CPU::rl_r(){}
void CPU::rl_hl(){}
void CPU::rrc_r(){}
void CPU::rr_r(){}
void CPU::rr_hl(){}
void CPU::sl_a_r(){}
void CPU::sl_a_hl(){}
void CPU::swap_r(){}
void CPU::swap_hl(){}
void CPU::sr_a_r(){}
void CPU::sr_a_hl(){}
void CPU::srl_r(){}
void CPU::srl_hl(){}

// single bit operation
void CPU::bit_n_r(){}
void CPU::bit_n_hl(){}
void CPU::set_n_r(){}
void CPU::set_n_hl(){}
void CPU::res_n_r(){}
void CPU::res_n_hl(){}

// cpu control
void CPU::ccf(){}
void CPU::scf(){}
void CPU::nop(){}
void CPU::halt(){}
void CPU::stop(){}
void CPU::di(){}
void CPU::ei(){}

// jump
void CPU::jp_nn(){}
void CPU::jp_hl(){}
void CPU::jp_f_nn(){}
void CPU::jr_pcdd(){}
void CPU::jr_f_pcdd(){}
void CPU::call_nn(){}
void CPU::call_f_nn(){}
void CPU::ret(){}
void CPU::ret_f(){}
void CPU::reti(){}
void CPU::rst(){}

// cb
void CPU::cb(){}
