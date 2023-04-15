#include "constants.h"
#include "helpers.h"
#include "cpu.h"


/*
    Instructions
*/

UINT CPU::run_fde() {
        
    // fetch
    opcode = mmu.read(PC);    
    
    // decode
    

    // execute
    // will be one instruction
    

}

// 8 bit Arithmetic and Logic Instructions
BYTE CPU::_add_8(BYTE a, BYTE b, bool carry) {
    // Z0HC
    if(carry)
        carry = registers.get_c_flag();
    
    BYTE sum = a + b + (BYTE) carry;

    registers.set_z_flag(sum == 0);

    registers.set_n_flag(0);

    // bit 3 overflow
    registers.set_h_flag( ((a & 0x0F) + (b & 0x0F) + (BYTE) carry) > 0x0F );

    // bit 7 overflow
    registers.set_c_flag( (a + b + (BYTE) carry) > 0xFF );

    return sum;
}

BYTE CPU::_sub_8(BYTE a, BYTE b, bool carry) {
    // Z1HC
    if(carry)
        carry = registers.get_c_flag();

    BYTE diff = a - b - (BYTE) carry;

    registers.set_z_flag(diff == 0);

    registers.set_n_flag(1);

    // bit 4 borrow
    registers.set_h_flag( ((b & 0x0F) + (BYTE) carry) > (a & 0x0F) );

    // bit 8 borrow
    registers.set_c_flag( (b + (BYTE) carry) > a );

    return diff;
}

BYTE CPU::_and(BYTE a, BYTE b) {
    // Z010

    BYTE ab = a & b;

    registers.set_z_flag(ab == 0);

    registers.set_n_flag(0);

    registers.set_h_flag(1);

    registers.set_c_flag(0);
    
    return ab;
}

BYTE CPU::_or(BYTE a, BYTE b) {
    // Z000

    BYTE ab = a | b;

    registers.set_z_flag(ab == 0);

    registers.set_n_flag(0);

    registers.set_h_flag(0);

    registers.set_c_flag(0);

    return ab;
}

BYTE CPU::_xor(BYTE a, BYTE b) {
    // Z000

    BYTE ab = a ^ b;

    registers.set_z_flag(ab == 0);
    
    registers.set_n_flag(0);

    registers.set_h_flag(0);

    registers.set_c_flag(0);

    return ab;
}

BYTE CPU::_dec(BYTE d) {
    // Z1H-

    BYTE dec = d - 1;

    registers.set_z_flag(dec == 0);

    registers.set_n_flag(1);

    registers.set_h_flag( (d & 0x0F) == 0 );

    return dec;
}

BYTE CPU::_inc(BYTE i) {
    // Z0H-

    BYTE inc = i + 1;

    registers.set_z_flag(inc == 0);
    
    registers.set_n_flag(0);
    
    registers.set_h_flag( (i & 0x0F) == 0x0F );

    return inc;
}

void CPU::_add_A_r8(bool carry) {
    // Z0HC

    UINT r8 = _register_8(opcode & 0x07);

    BYTE sum = _add_8(
        registers.read_8(A),
        registers.read_8(r8),
        carry
    );

    registers.write_8(A, sum);
}

void CPU::_add_A_mHL(bool carry) {
    // Z0HC

    WORD hl = registers.read_16(HL);

    BYTE mHL = mmu.read(hl);

    BYTE sum = _add_8(
        registers.read_8(A),
        mHL,
        carry
    );

    registers.write_8(A, sum);
}

void CPU::_add_A_n8(bool carry) {
    // Z0HC

    BYTE n8 = mmu.read(PC);

    BYTE sum = _add_8(
        registers.read_8(A),
        n8,
        carry
    );

    PC += 1;

    registers.write_8(A, sum);
}

void CPU::_sub_A_r8(bool carry) {
    // Z1HC

    UINT r8 = _register_8(opcode & 0x07);

    BYTE diff = _sub_8(
        registers.read_8(A),
        registers.read_8(r8),
        carry
    );

    registers.write_8(A, diff);
}

void CPU::_sub_A_mHL(bool carry) {
    // Z1HC

    WORD hl = registers.read_16(HL);

    BYTE mHL = mmu.read(hl);

    BYTE diff = _sub_8(
        registers.read_8(A),
        mHL,
        carry
    );

    registers.write_8(A, diff);
}

void CPU::_sub_A_n8(bool carry) {
    // Z1HC

    BYTE n8 = mmu.read(PC); // FIXME get n8 from opcode

    BYTE diff = _sub_8(
        registers.read_8(A),
        n8,
        carry
    );

    PC += 1;

    registers.write_8(A, diff);
}

UINT CPU::adc_A_r8() {
    _add_A_r8(true);
    
    // 4 T Cycles
    return 4;
}

UINT CPU::adc_A_mHL() {
    _add_A_mHL(true);

    // 8 T Cycles
    return 8;
}

UINT CPU::adc_A_n8() {
    _add_A_n8(true);

    // 8 T Cycles;
    return 8;
}

UINT CPU::add_A_r8() {
    _add_A_r8(false);
    
    // 4 T Cycles
    return 4;
}

UINT CPU::add_A_mHL() {
    _add_A_mHL(false);

    // 8 T Cycles
    return 8;
}

UINT CPU::add_A_n8() {
    _add_A_n8(false);

    // 8 T Cycles;
    return 8;
}

UINT CPU::and_A_r8() {
    // Z010

    UINT r8 = _register_8(opcode & 0x07); // get r8 from opcode fIXME

    BYTE a = _and(
        registers.read_8(A),
        registers.read_8(r8)
    );

    registers.write_8(A, a);

    // 4 T Cycles
    return 4;
}

UINT CPU::and_A_mHL() { 
    // Z010

    WORD hl = registers.read_16(HL);

    BYTE mHL = mmu.read(hl);

    BYTE a = _and(
        registers.read_8(A),
        mHL
    );
    
    registers.write_8(A, a);

    // 8 T Cycles
    return 8;
}

UINT CPU::and_A_n8() {
    // Z010

    BYTE n8 = mmu.read(PC); // FIXME get n8 from opcode

    BYTE a = _and(
        registers.read_8(A),
        n8
    );

    registers.write_8(A, a);

    PC += 1;

    // 8 T Cycles
    return 8;
}

UINT CPU::cp_A_r8() {
    // Z1HC

    UINT r8 = _register_8(opcode & 0x07); // FIXME get r8 from opcode

    _sub_8(
        registers.read_8(A),
        registers.read_8(r8),
        false
    );

    // 4 T Cycles
    return 4;
}

UINT CPU::cp_A_mHL() {
    // Z1HC

    WORD hl = registers.read_16(HL);

    BYTE mHL = mmu.read(hl);

    _sub_8(
        registers.read_8(A),
        mHL,
        false
    );

    // 8 T Cycles
    return 8;
}

UINT CPU::cp_A_n8() {
    // Z1HC

    BYTE n8 = mmu.read(PC); // FIXME get n8 from opcode

    _sub_8(
        registers.read_8(A),
        n8,
        false
    );

    PC += 1;

    // 8 T Cycles
    return 8;
}

UINT CPU::dec_r8() {
    // ZNH-

    UINT r8 = _register_8(opcode & 0x07); // FIXME get r8 from opcode

    BYTE dec = _dec(registers.read_8(r8));

    registers.write_8(r8, dec);

    // 4 T Cycles
    return 4;
}

UINT CPU::dec_mHL() {
    // ZNH-

    WORD hl = registers.read_16(HL);

    BYTE mHL = mmu.read(hl);

    BYTE dec = _dec(mHL);

    mmu.write(hl, dec);

    // 12 T Cycles
    return 12;
}

UINT CPU::inc_r8() {
    // Z0H-

    UINT r8 = _register_8(opcode & 0x07); // FIXME get r8 from opcode

    BYTE inc = _inc(registers.read_8(r8));

    registers.write_8(r8, inc);

    // 4 T Cycles
    return 4;
}

UINT CPU::inc_mHL() {
    // Z0H-

    WORD hl = registers.read_16(HL);

    BYTE mHL = mmu.read(hl);

    BYTE inc = _inc(mHL);

    mmu.write(hl, inc);

    // 12 T Cycles
    return 12;
}

UINT CPU::or_A_r8() {
    // Z000

    UINT r8 = _register_8(opcode & 0x07); // FIXME get r8 from opcode

    BYTE o = _or(
        registers.read_8(A),
        registers.read_8(r8)
    );

    registers.write_8(A, o);

    // 4 T Cycles
    return 4;
}

UINT CPU::or_A_mHL() {
    // Z000

    WORD hl = registers.read_16(HL);

    BYTE mHL = mmu.read(hl);

    BYTE o = _or(
        registers.read_8(A),
        mHL
    );

    registers.write_8(A, o);

    // 8 T Cycles
    return 8;
}

UINT CPU::or_A_n8() {
    // Z000

    BYTE n8 = mmu.read(PC); // FIXME get n8 from opcode

    BYTE o = _or(
        registers.read_8(A),
        n8
    );

    registers.write_8(A, o);

    PC += 1;

    // 8 T Cycles
    return 8;
}

UINT CPU::sbc_A_r8() {
    // Z1HC

    _sub_A_r8(true);

    // 4 T Cycles
    return 4;
}

UINT CPU::sbc_A_mHL() {
     // Z1HC

    _sub_A_mHL(true);

    // 8 T Cycles
    return 8;
}

UINT CPU::sbc_A_n8() {
    // Z1HC

    _sub_A_n8(true);

    // 8 T Cycles
    return 8;
}

UINT CPU::sub_A_r8() {
    // Z1HC

    _sub_A_r8(false);

    // 4 T Cycles
    return 4;
}

UINT CPU::sub_A_mHL() {
     // Z1HC

    _sub_A_mHL(false);

    // 8 T Cycles
    return 8;
}

UINT CPU::sub_A_n8() {
    // Z1HC

    _sub_A_n8(false);

    // 8 T Cycles
    return 8;
}

UINT CPU::xor_A_r8() {
    // Z000
    
    UINT r8 = _register_8(opcode & 0x07); // FIXME r8

    BYTE x = _xor(
        registers.read_8(A),
        registers.read_8(r8)
    );

    registers.write_8(A, x);

    // 4 T Cycles
    return 4;
}

UINT CPU::xor_A_mHL() {
    // Z000

    WORD hl = registers.read_16(HL);

    BYTE mHL = mmu.read(hl);

    BYTE x = _xor(
        registers.read_8(A),
        mHL
    );

    registers.write_8(A, x);
    
    // 8 T Cycles
    return 8;
}

UINT CPU::xor_A_n8() {
    // Z000

    BYTE n8 = mmu.read(PC); // FIXME read n8 from opcode

    BYTE x = _xor(
        registers.read_8(A),
        n8
    );

    registers.write_8(A, x);
    
    PC += 1;

    // 8 T Cycles
    return 8;
}

// 16 bit Arithmetic Instructions
WORD CPU::_add_16(WORD a, WORD b) {
    // -0HC

    registers.set_n_flag(0);

    registers.set_h_flag( ((a & 0x0FFF) + (b & 0x0FFF)) > 0x0FFF );

    registers.set_c_flag( (a + b) > 0xFFFF );

    return a + b;
}

UINT CPU::add_HL_r16() {
    // -0HC

    UINT r16 = _register_16( (opcode >> 3u) & 0x07 ); // FIXME get r16 from opcode

    WORD sum = _add_16(
        registers.read_16(HL),
        registers.read_16(r16)
    );

    registers.write_16(HL, sum);

    // 8 T Cycles
    return 8;
}

UINT CPU::dec_r16() {
    
    UINT r16 = _register_16( (opcode >> 3u) & 0x07 ); // FIXME get r16 from opcode

    WORD dec = registers.read_16(r16) - 1;

    registers.write_16(r16, dec);

    // 8 T Cycles
    return 8;
}

UINT CPU::inc_r16() {

    UINT r16 = _register_16( (opcode >> 3u) & 0x07 ); // FIXME get r16 from opcode

    WORD inc = registers.read_16(r16) + 1;

    registers.write_16(r16, inc);

    // 8 T Cycles
    return 8;
}

// Bit Operations Instructions
void CPU::_bit(BYTE b, UINT n) {
    // Z01-

    registers.set_z_flag( (b >> n) & 0x01 );

    registers.set_n_flag(0);

    registers.set_h_flag(1);

}

BYTE CPU::_res(BYTE b, UINT n) {
    return (b & ~(1u << n));
}

BYTE CPU::_set(BYTE b, UINT n) {
    return (b | (1u << n));
}

BYTE CPU::_swap(BYTE b) {
    // Z000

    b = ((b & 0x0F) << 4u) | ((b & 0xF0) >> 4u);

    registers.set_z_flag(b == 0);
    
    registers.set_n_flag(0);

    registers.set_h_flag(0);

    registers.set_c_flag(0);

    return b;
}

UINT CPU::bit_u3_r8() {
    // Z01-

    UINT r8 = _register_8(opcode & 0x07); // GET r8 from opcode
    UINT u3 = (opcode >> 3u) & 0x07; // GET u3 from opcode

    _bit(
        registers.read_8(r8),
        u3
    );

    // 8 T Cycles
    return 8;
}

UINT CPU::bit_u3_mHL() {
    // Z01-

    WORD hl = registers.read_16(HL);

    BYTE mHL = mmu.read(hl);

    UINT u3 = (opcode >> 3u) & 0x07;

    _bit(
        mHL,
        u3
    );

    // 12 T Cycles
    return 12;
}

UINT CPU::res_u3_r8() {
    
    UINT r8 = _register_8(opcode & 0x07);
    UINT u3 = (opcode >> 3u) & 0x07;

    BYTE res = _res(
        registers.read_8(r8),
        u3
    );

    registers.write_8(r8, res);

    // 8 T Cycles
    return 8;
}

UINT CPU::res_u3_mHL() {

    WORD hl = registers.read_16(HL);

    BYTE mHL = mmu.read(hl);

    UINT u3 = (opcode >> 3u) & 0x07;

    BYTE res = _res(
        mHL,
        u3
    );

    mmu.write(hl, res);

    // 16 T Cycles
    return 16;
}

UINT CPU::set_u3_r8() {

    UINT r8 = _register_8(opcode & 0x07);
    UINT u3 = (opcode >> 3u) & 0x07;

    BYTE set = _set(
        registers.read_8(r8),
        u3
    );

    registers.write_8(r8, u3);

    // 8 T Cycles
    return 8;
}

UINT CPU::set_u3_mHL() {

    WORD hl = registers.read_16(HL);

    BYTE mHL = mmu.read(hl);

    UINT u3 = (opcode >> 3u) & 0x07;

    BYTE set = _set(
        mHL,
        u3
    );

    mmu.write(hl, set);

    // 16 T Cycles
    return 16;
}

UINT CPU::swap_r8() {
    // Z000

    UINT r8 = _register_8(opcode & 0x07);

    BYTE s = _swap(
        registers.read_8(r8)
    );
    
    registers.write_8(r8, s);

    // 8 T Cycles
    return 8;
}

UINT CPU::swap_mHL() {
    // Z000

    WORD hl = registers.read_16(HL);

    BYTE mHL = mmu.read(hl);

    BYTE s = _swap(
        mHL
    );

    mmu.write(hl, s);

    // 16 T Cycles
    return 16;
}

// Bit Shift Instructions
BYTE CPU::_rl(BYTE b, bool through_carry) {
    // Z00C

    bool lsb;

    if(through_carry) {
        lsb = registers.get_c_flag();
    } else {
        lsb = ( (b >> 7u) & 0x01 );
    }

    BYTE rl = (b << 1u) | (BYTE) lsb;

    registers.set_z_flag(rl == 0);

    registers.set_n_flag(0);

    registers.set_h_flag(0);

    registers.set_c_flag( (b >> 7u) & 0x01 );

    return rl;
}

BYTE CPU::_rr(BYTE b, bool through_carry) {
    // Z00C

    bool msb;

    if(through_carry) {
        msb = registers.get_c_flag();
    } else {
        msb = ( b & 0x01 );
    }

    BYTE rr = (BYTE) (msb << 7u) | (b >> 1u);

    registers.set_z_flag(rr == 0);
    
    registers.set_n_flag(0);

    registers.set_h_flag(0);

    registers.set_c_flag( b & 0x01 );

    return rr;
}

BYTE CPU::_sl(BYTE b) {

    BYTE sl = b << 1u;

    registers.set_z_flag(sl == 0);
    
    registers.set_n_flag(0);

    registers.set_h_flag(0);

    registers.set_c_flag( (b >> 7u) & 0x01 );

    return sl;
}

BYTE CPU::_sr(BYTE b, bool arithmetic) {

    bool msb;

    if(arithmetic) {
        msb = (b >> 7u) & 0x01;
    } else {
        msb = 0;
    }

    BYTE sr = ((BYTE) msb << 7u) | (b >> 1u);

    registers.set_z_flag(sr == 0);

    registers.set_n_flag(0);
    
    registers.set_h_flag(0);

    registers.set_c_flag( b & 0x01 );

    return sr;
}

UINT CPU::rl_r8() {
    // Z00C

    UINT r8 = _register_8(opcode & 0x07);

    BYTE rl = _rl(
        registers.read_8(r8),
        true
    );

    registers.write_8(r8, rl);

    // 8 T Cycles
    return 8;
}

UINT CPU::rl_mHL() {
    // Z00C

    WORD hl = registers.read_16(HL);

    BYTE mHL = mmu.read(hl);

    BYTE rl = _rl(
        mHL,
        true
    );

    mmu.write(hl, rl);

    // 16 T Cycles
    return 16;
}

UINT CPU::rla() {
    // 000C

    BYTE rl = _rotate_left(
        registers.read_8(A)
    );

    registers.set_z_flag(0);

    registers.write_8(A, rl);

    // 4 T Cycles
    return 4;
}

UINT CPU::rlc_r8() {
    // Z00C

    UINT r8 = _register_8(opcode & 0x07);

    BYTE rl = _rl(
        registers.read_8(r8),
        false
    );

    registers.write_8(r8, rl);

    // 8 T Cycles
    return 8;
}

UINT CPU::rlc_mHL() {
    // Z00C

    WORD hl = registers.read_16(HL);

    BYTE mHL = mmu.read(hl);

    BYTE rl = _rl(
        mHL,
        false
    );

    mmu.write(hl, rl);

    // 16 T Cycles
    return 16;
}

UINT CPU::rlca() {
    // 000C
    
    BYTE rl = _rl(
        registers.read_8(A),
        false
    );

    registers.set_z_flag(0);

    registers.write_8(A, rl);

    // 4 T Cycles
    return 4;
}

UINT CPU::rr_r8() {
    // Z00C

    UINT r8 = _register_8(opcode & 0x07);

    BYTE rr = _rr(
        registers.read_8(r8),
        true
    );

    registers.write_8(r8, rr);

    // 8 T Cycles
    return 8;
}

UINT CPU::rr_mHL() {
    // Z00C

    WORD hl = registers.read_16(HL);

    BYTE mHL = mmu.read(hl);

    BYTE rr = _rr(
        mHL,
        true
    );

    mmu.write(hl, rr);

    // 16 T Cycles
    return 16;
}

UINT CPU::rra() {
    // 000C

    BYTE rr = _rr(
        registers.read_8(A),
        true
    );

    registers.set_z_flag(0);

    registers.write_8(A, rr);

    // 4 T Cycles
    return 4;
}

UINT CPU::rrc_r8() {
    // Z00C

    UINT r8 = _register_8(opcode & 0x07);

    BYTE rr = _rr(
        registers.read_8(r8),
        false
    );

    registers.write_8(r8, rr);

    // 8 T Cycles
    return 8;
}

UINT CPU::rrc_mHL() {
    // Z00C

    WORD hl = registers.read_16(HL);

    BYTE mHL = mmu.read(hl);

    BYTE rr = _rr(
        mHL,
        false
    );

    mmu.write(hl, rr);

    // 16 T Cycles
    return 16;
}

UINT CPU::rrca() {
    // 000C
    
    BYTE rr = _rr(
        registers.read_8(A),
        false
    );

    registers.write_8(A, rr);

    // 4 T Cycles
    return 4;
}

UINT CPU::sla_r8() {
    // Z00C
    
    UINT r8 = _register_8(opcode & 0x07);

    BYTE sl = _sl(
        registers.read_8(r8)
    );

    registers.write_8(
        r8,
        sl
    );

    // 8 T Cycles
    return 8;
}

UINT CPU::sla_mHL() {
    // Z00C

    WORD hl = registers.read_16(HL);

    BYTE mHL = mmu.read(hl);

    BYTE sl = _sl(
        mHL
    );

    mmu.write(hl, sl);

    // 16 T Cycles
    return 16;
}

UINT CPU::sra_r8() {
    // Z00C
    
    UINT r8 = _register_8(opcode & 0x07);

    BYTE sr = _sr(
        registers.read_8(r8),
        true
    );

    registers.write_8(
        r8,
        sr
    );

    // 8 T Cycles
    return 8;
}

UINT CPU::sra_mHL() {
    // Z00C

    WORD hl = registers.read_16(HL);

    BYTE mHL = mmu.read(hl);

    BYTE sr = _sr(
        mHL,
        true
    );

    mmu.write(hl, sr);

    // 16 T Cycles
    return 16;
}

UINT CPU::srl_r8() {
    // Z00C
    
    UINT r8 = _register_8(opcode & 0x07);

    BYTE sr = _sr(
        registers.read_8(r8),
        false
    );

    registers.write_8(
        r8,
        sr
    );

    // 8 T Cycles
    return 8;
}

UINT CPU::srl_mHL() {
    // Z00C

    WORD hl = registers.read_16(HL);

    BYTE mHL = mmu.read(hl);

    BYTE sr = _sr(
        mHL,
        false
    );

    mmu.write(hl, sr);

    // 16 T Cycles
    return 16;
}

// Load Instructions
UINT CPU::ld_r8_r8(){
    
    UINT a_r8 = (opcode >> 3u) & 0x07;
    UINT b_r8 = opcode & 0x07;

    registers.write_8(
        a_r8,
        registers.read_8(b_r8)
    );

    // 4 T Cycles
    return 4;
}

UINT CPU::ld_r8_n8(){

    UINT r8 = (opcode >> 3u) & 0x07;
    BYTE n8 = mmu.read(PC);

    registers.write_8(
        r8,
        n8
    );

    PC += 1;

    // 8 T Cycles
    return 8;
}

UINT CPU::ld_r16_n16(){

    UINT r16 = (opcode >> 4u) & 0x03;
    WORD n16 = concat(
        mmu.read(PC + 1),
        mmu.read(PC)
    );

    registers.write_16(
        r16,
        n16
    );

    PC += 2;

    // 12 T Cycles
    return 12;
}

UINT CPU::ld_mHL_r8(){

    WORD hl = registers.read_16(HL);

    UINT r8 = _register_8(opcode & 0x07);
    
    mmu.write(
        hl,
        registers.read_8(r8)
    );

    // 8 T Cycles
    return 8;
}

UINT CPU::ld_mHL_n8(){

    WORD hl = registers.read_16(HL);

    BYTE n8 = mmu.read(PC);

    mmu.write(
        hl,
        n8
    );

    PC += 1;

    // 12 T Cycles
    return 12;
}

UINT CPU::ld_r8_mHL(){

    WORD hl = registers.read_16(HL);

    BYTE mHL = mmu.read(hl);

    UINT r8 = _register_8(opcode & 0x07);

    registers.write_8(r8, mHL);

    // 8 T Cycles
    return 8;
}

UINT CPU::ld_mr16_A(){
    
    UINT r16 = (opcode >> 4u) & 0x03;

    mmu.write(
        registers.read_16(r16),
        registers.read_8(A)
    );
    
    // 8 T Cycles
    return 8;
}

UINT CPU::ld_mn16_A(){

    WORD n16 = concat(
        mmu.read(PC + 1),
        mmu.read(PC)
    );

    mmu.write(
        n16,
        registers.read_8(A)
    );

    PC += 2;

    // 16 T Cycles
    return 16;
}

UINT CPU::ldh_mn16_A(){

    BYTE n8 = mmu.read(PC);

    mmu.write(
        0xFF00 + n8,
        registers.read_8(A)
    );

    PC += 1;

    // 12 T Cycles
    return 12;
}

UINT CPU::ldh_mC_A(){

    mmu.write(
        0xFF00 + registers.read_8(C),
        registers.read_8(A)    
    );

    // 8 T Cycles
    return 8;
}

UINT CPU::ld_A_mr16(){

    UINT r16 = (opcode >> 4u) & 0x03;

    registers.write_8(
        A,
        mmu.read(registers.read_16(r16))
    );

    // 8 T Cycles
    return 8;
}

UINT CPU::ld_A_mn16(){

    WORD n16 = concat(
        mmu.read(PC + 1),
        mmu.read(PC)
    );

    registers.write_8(
        A,
        mmu.read(n16)
    );

    PC += 2;

    // 16 T Cycles
    return 16;
}

UINT CPU::ldh_A_mn16(){

    BYTE n8 = mmu.read(PC);

    registers.write_8(
        A,
        mmu.read(0xFF00 + n8)
    );

    PC += 1;

    // 12 T Cycles
    return 12;
}

UINT CPU::ldh_A_mC(){

    registers.write_8(
        A,
        mmu.read(0xFF00 + registers.read_8(C))
    );

    // 8 T Cycles
    return 8;
}

UINT CPU::ld_mHLI_A(){

    WORD hl = registers.read_16(HL);

    mmu.write(
        hl,
        registers.read_8(A)
    );

    registers.write_16(HL, hl + 1);

    // 8 T Cycles
    return 8;
}

UINT CPU::ld_mHLD_A(){

    WORD hl = registers.read_16(HL);

    mmu.write(
        hl,
        registers.read_8(A)
    );

    registers.write_16(HL, hl - 1);

    // 8 T Cycles
    return 8;
}

UINT CPU::ld_A_mHLI(){

    WORD hl = registers.read_16(HL);

    registers.write_8(
        A,
        mmu.read(hl)
    );

    registers.write_16(HL, hl + 1);

    // 8 T Cycles
    return 8;
}

UINT CPU::ld_A_mHLD(){
    
    WORD hl = registers.read_16(HL);

    registers.write_8(
        A,
        mmu.read(hl)
    );

    registers.write_16(HL, hl - 1);

    // 8 T Cycles
    return 8;
}

// Jump and Subroutines
bool CPU::_condition(UINT c) {
    switch(c) {
        case 0:
            return !registers.get_z_flag();
        case 1:
            return registers.get_z_flag();
        case 2:
            return !registers.get_c_flag();
        case 3:
            return registers.get_c_flag();
    }
    throw "Unknown condition";
}

void CPU::_call(WORD address) {
    mmu.write(SP - 1, lsb(PC));

    mmu.write(SP - 2, msb(PC));

    SP -= 2;

    PC = address;
}

UINT CPU::call_n16() {

    WORD n16 = concat(
        mmu.read(PC + 1),
        mmu.read(PC)
    );

    _call(n16);

    // 24 T Cycles
    return 24;
}

UINT CPU::call_cc_n16() {
    WORD n16 = concat(
        mmu.read(PC + 1),
        mmu.read(PC)
    );

    bool cc = _condition( (opcode >> 3u) & 0x03 );

    if(cc) {
        _call(n16);

        // 24 T Cycles
        return 24;
    }

    PC += 2;

    // 12 T Cycles
    return 12;
}

UINT CPU::jp_HL() {

    PC = registers.read_16(HL);

    // 4 T Cycles
    return 4;
}

UINT CPU::jp_n16() {

    WORD n16 = concat(
        mmu.read(PC + 1),
        mmu.read(PC)
    );

    PC = n16;

    // 16 T Cycles
    return 16;
}

UINT CPU::jp_cc_n16() {

    WORD n16 = concat(
        mmu.read(PC + 1),
        mmu.read(PC)
    );

    bool cc = _condition( (opcode >> 3u) & 0x03 );

    if(cc) {
        PC = n16;

        // 16 T Cycles
        return 16;
    }

    PC += 2;

    // 12 T Cycles
    return 12;
}

UINT CPU::jr_n16() {

    SIGNED_BYTE n8 = mmu.read(PC);

    // FIXME signed and unsigend arithmetic?
    PC += n8 + 2;

    // 12 T Cycles
    return 12;
}

UINT CPU::jr_cc_n16() {

    SIGNED_BYTE n8 = mmu.read(PC);

    bool c;
    // FIXME handle c flag
    if(c) {
        // FIXME signed and unsigend arithmetic?
        PC += n8 + 2;

        // 8 T Cycles
        return 8;
    }

    PC += 1;

    // 12 T Cycles
    return 12;
}

UINT CPU::ret() {
    
    WORD p = concat(
        mmu.read(SP + 1),
        mmu.read(SP)
    );

    SP += 2;

    PC = p;

    // 16 T Cycles
    return 16;
}

UINT CPU::ret_cc() {

    bool cc = _condition( (opcode >> 3u) & 0x03 );

    if(cc) {
        ret();

        // 20 T Cycles
        return 20;
    }

    // 8 T Cycles
    return 8;
}

UINT CPU::reti() {
    // FIXME set IME

    ret();

    // 16 T Cycles
    return 16;
}

UINT CPU::rst() {

    WORD vec = opcode & 0b00111000;

    mmu.write(SP - 1, lsb(PC + 1));

    mmu.write(SP - 2, msb(PC + 1));

    SP -= 2;

    PC = vec;

    // 16 T Cycles
    return 16;
}

// Stack Operations Instructions
UINT CPU::add_HL_sp() {
    // -0HC

    WORD sum = _add_16(
        registers.read_16(HL),
        SP
    );

    registers.write_16(HL, sum);

    // 8 T Cycles
    return 8;
}

UINT CPU::add_SP_e8() {

    SIGNED_BYTE e8 = mmu.read(PC);

    // Set flags
    _add_8((BYTE) SP, e8, false);

    SP += e8;

    PC += 1;

    // 16 T Cycles
    return 16;
}

UINT CPU::dec_SP() {

    SP -= 1;

    // 8 T Cycles
    return 8;
}

UINT CPU::inc_SP() {

    SP += 1;

    // 8 T Cycles
    return 8;
}

UINT CPU::ld_SP_n16() {

    WORD n16 = concat(
        mmu.read(PC + 1),
        mmu.read(PC)
    );

    SP = n16;

    PC += 2;

    // 12 T Cycles
    return 12;
}

UINT CPU::ld_mn16_SP() {

    WORD n16 = concat(
        mmu.read(PC + 1),
        mmu.read(PC)
    );

    mmu.write(n16, (BYTE) (SP & 0xFF));
    mmu.write(n16 + 1, (BYTE) (SP >> 8u));

    PC += 2;

    // 20 T Cycles
    return 20;
}

UINT CPU::ld_HL_SPe8() {
    // 00HC

    SIGNED_BYTE e8 = mmu.read(PC);
    
    // set right flags
    _add_8((BYTE) SP, e8, false);

    registers.write_16(
        HL,
        SP + e8
    );
    
    PC += 1;

    // 12 T Cycles
    return 12;
}

UINT CPU::ld_SP_HL() {
    
    SP = registers.read_16(HL);

    // 8 T Cycles
    return 8;
}

UINT CPU::pop_AF() {
    // ZNHC

    BYTE f = mmu.read(SP),
         a = mmu.read(SP + 1);
        
    SP += 2;

    registers.write_16(AF, concat(a, f));

    // 12 T Cycles
    return 12;
}

UINT CPU::pop_r16() {

    UINT r16 = (opcode >> 4u) & 0x03;

    BYTE lo = mmu.read(SP),
         hi = mmu.read(SP + 1);

    SP += 2;

    registers.write_16(
        r16,
        concat(hi, lo)
    );

    // 12 T Cycles
    return 12;
}

UINT CPU::push_AF() {

    mmu.write(
        SP - 1, 
        registers.read_8(A)
    );

    mmu.write(
        SP - 2,
        registers.read_8(F) & 0x0F
    );
    
    SP -= 2;

    // 16 T Cycles
    return 16;
}

UINT CPU::push_r16() {

    UINT r16 = (opcode >> 4u) & 0x03;

    WORD r = registers.read_16(r16);

    mmu.write(
        SP - 1, 
        msb(r)
    );

    mmu.write(
        SP - 2,
        lsb(r)
    );
    
    SP -= 2;

    // 16 T Cycles
    return 16;
}


// Miscellaneous Instructions
UINT CPU::ccf() {
    // -00C

    registers.set_n_flag(0);

    registers.set_h_flag(0);

    registers.set_c_flag(
        !registers.get_c_flag()
    );

    // 4 T Cycles
    return 4;
}

UINT CPU::cpl() {
    // -11-
    
    registers.write_8(
        A,
        ~registers.read_8(A)
    );

    registers.set_n_flag(1);

    registers.set_h_flag(1);

    // 4 T Cycles
    return 4;
}

UINT CPU::daa() {
    // Z-0C

    // FIXME decimal adjust accumulator ??

    BYTE a = registers.read_8(A);

    bool n = registers.get_n_flag(),
         h = registers.get_h_flag(),
         c = registers.get_c_flag();

    if(n) {
        if(c) {
            a -= 0x60;
        } 
        if(h) {
            a -= 0x06;
        }
    } else {
        if(c || a > 0x99) {
            a += 0x60;
            registers.set_c_flag(1);
        }
        if(h || (a & 0x0F) > 0x09) {
            a += 0x06;
        }
    }

    registers.set_z_flag(a == 0);

    registers.set_h_flag(0);

    // 4 T Cycles
    return 4;
}

UINT CPU::di() {

    IME = 0;

    // 4 T Cycles
    return 4;
}

UINT CPU::ei() {

    // Exec next instr    // FIXME decode and exec

    IME = 1;

    // 4 T Cycles FIXME add next instr cycles
    return 4;
}

UINT CPU::halt() {
    halt_mode = true;
    
    // Wait for interrupt
    return 0;
}

UINT CPU::nop() {
    // no operation

    // 4 T Cycles
    return 4;
}

UINT CPU::scf() {
    // -001

    registers.set_n_flag(0);

    registers.set_h_flag(0);

    registers.set_c_flag(1);

    // 4 T Cycles
    return 4;
}

UINT CPU::stop() {
    sleep_mode = true;
    
    // Wait for interrupt or joypad
    return 0;
}
