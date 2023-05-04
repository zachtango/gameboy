#include "constants.h"
#include "helpers.h"
#include "cpu.h"
#include "mmu.h"
#include <iostream>
#include <iomanip>

#include <execinfo.h>

char out_r8(U32 r8) {
    char c[] = {'A', 'F', 'B', 'C', 'D', 'E', 'H', 'L'};
    return c[r8];
}

char* out_r16(U32 r16) {
    char *c[7] = {"AF", "", "BC", "", "DE", "", "HL"};
    
    return c[r16];
}

/* DEBUG FUNCTIONS */
void CPU::print_registers() {
    std::cout << std::hex << std::uppercase <<
        "A:" << std::setw(2) << std::setfill('0') << (int)(registers.read_8(A)) << ' ' <<
        "F:" << std::setw(2) << std::setfill('0') << (int)(registers.read_8(F)) << ' ' <<
        "B:" << std::setw(2) << std::setfill('0') << (int)(registers.read_8(B)) << ' ' <<
        "C:" << std::setw(2) << std::setfill('0') << (int)(registers.read_8(C)) << ' ' <<
        "D:" << std::setw(2) << std::setfill('0') << (int)(registers.read_8(D)) << ' ' <<
        "E:" << std::setw(2) << std::setfill('0') << (int)(registers.read_8(E)) << ' ' <<
        "H:" << std::setw(2) << std::setfill('0') << (int)(registers.read_8(H)) << ' ' <<
        "L:" << std::setw(2) << std::setfill('0') << (int)(registers.read_8(L)) << ' ' <<
        "SP:" << std::setw(4) << std::setfill('0') << (int) (SP) << ' ' <<
        "PC:" << std::setw(4) << std::setfill('0') << (int) (PC) << ' ' <<
        "PCMEM:" << std::setw(2) << std::setfill('0') << (int) mmu.read(PC) << ','
                << std::setw(2) << std::setfill('0') << (int) mmu.read(PC + 1) << ','
                << std::setw(2) << std::setfill('0') << (int) mmu.read(PC + 2) << ','
                << std::setw(2) << std::setfill('0') << (int) mmu.read(PC + 3) << '\n';
}

/* MASTER SEQUENCE (returns number of cycles taken) */
U32 CPU::fetch_decode_execute() {

    // fetch
    opcode = mmu.read(PC);
    
    PC += 1;

    // decode
    cpu_instr f;
    
    if(prefix) {
        f = cb_instr[opcode];
        prefix = false;
    } else f = instr[opcode];

    U32 cycles;

    // execute
    cycles = (this->*f)();

    // backtrace_symbols_fd((void*const*) &f, 1, 1);

    return cycles;
}

/*
    CPU INSTRUCTIONS
    https://rgbds.gbdev.io/docs/v0.6.1/gbz80.7/
*/

// Prefix
U32 CPU::cb_prefix() {
    prefix = true;
    return 4;
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

    U32 r8 = _register_8(opcode & 0x07);
    // std::cout << out_r8(r8) << '\n';

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
    // std::cout << std::hex << (int) n8 << '\n';

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

    U32 r8 = _register_8(opcode & 0x07);

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

U32 CPU::adc_A_r8() {

    _add_A_r8(true);
    
    // 4 T Cycles
    return 4;
}

U32 CPU::adc_A_mHL() {

    _add_A_mHL(true);

    // 8 T Cycles
    return 8;
}

U32 CPU::adc_A_n8() {

    _add_A_n8(true);

    // 8 T Cycles;
    return 8;
}

U32 CPU::add_A_r8() {

    _add_A_r8(false);
    
    // 4 T Cycles
    return 4;
}

U32 CPU::add_A_mHL() {

    _add_A_mHL(false);

    // 8 T Cycles
    return 8;
}

U32 CPU::add_A_n8() {

    _add_A_n8(false);

    // 8 T Cycles;
    return 8;
}

U32 CPU::and_A_r8() {

    // Z010

    U32 r8 = _register_8(opcode & 0x07); // get r8 from opcode fIXME

    BYTE a = _and(
        registers.read_8(A),
        registers.read_8(r8)
    );

    registers.write_8(A, a);

    // 4 T Cycles
    return 4;
}

U32 CPU::and_A_mHL() {
 
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

U32 CPU::and_A_n8() {

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

U32 CPU::cp_A_r8() {

    // Z1HC

    U32 r8 = _register_8(opcode & 0x07); // FIXME get r8 from opcode

    _sub_8(
        registers.read_8(A),
        registers.read_8(r8),
        false
    );

    // 4 T Cycles
    return 4;
}

U32 CPU::cp_A_mHL() {

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

U32 CPU::cp_A_n8() {

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

U32 CPU::dec_r8() {

    // ZNH-

    U32 r8 = _register_8((opcode >> 3u) & 0x07); // FIXME get r8 from opcode

    BYTE dec = _dec(registers.read_8(r8));

    registers.write_8(r8, dec);

    // 4 T Cycles
    return 4;
}

U32 CPU::dec_mHL() {

    // ZNH-

    WORD hl = registers.read_16(HL);

    BYTE mHL = mmu.read(hl);

    BYTE dec = _dec(mHL);

    mmu.write(hl, dec);

    // 12 T Cycles
    return 12;
}

U32 CPU::inc_r8() {

    // Z0H-

    U32 r8 = _register_8((opcode >> 3u) & 0x07); // FIXME get r8 from opcode

    BYTE inc = _inc(registers.read_8(r8));

    // std::cout << out_r8(r8) << " val: " << std::hex << (int) inc << '\n';

    registers.write_8(r8, inc);

    // 4 T Cycles
    return 4;
}

U32 CPU::inc_mHL() {

    // Z0H-

    WORD hl = registers.read_16(HL);

    BYTE mHL = mmu.read(hl);

    BYTE inc = _inc(mHL);

    mmu.write(hl, inc);

    // 12 T Cycles
    return 12;
}

U32 CPU::or_A_r8() {

    // Z000

    U32 r8 = _register_8(opcode & 0x07); // FIXME get r8 from opcode

    BYTE o = _or(
        registers.read_8(A),
        registers.read_8(r8)
    );

    registers.write_8(A, o);

    // 4 T Cycles
    return 4;
}

U32 CPU::or_A_mHL() {

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

U32 CPU::or_A_n8() {

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

U32 CPU::sbc_A_r8() {

    // Z1HC

    _sub_A_r8(true);

    // 4 T Cycles
    return 4;
}

U32 CPU::sbc_A_mHL() {

     // Z1HC

    _sub_A_mHL(true);

    // 8 T Cycles
    return 8;
}

U32 CPU::sbc_A_n8() {

    // Z1HC

    _sub_A_n8(true);

    // 8 T Cycles
    return 8;
}

U32 CPU::sub_A_r8() {

    // Z1HC

    _sub_A_r8(false);

    // 4 T Cycles
    return 4;
}

U32 CPU::sub_A_mHL() {

     // Z1HC

    _sub_A_mHL(false);

    // 8 T Cycles
    return 8;
}

U32 CPU::sub_A_n8() {

    // Z1HC

    _sub_A_n8(false);

    // 8 T Cycles
    return 8;
}

U32 CPU::xor_A_r8() {

    // Z000
    
    U32 r8 = _register_8(opcode & 0x07); // FIXME r8

    BYTE x = _xor(
        registers.read_8(A),
        registers.read_8(r8)
    );

    registers.write_8(A, x);

    // 4 T Cycles
    return 4;
}

U32 CPU::xor_A_mHL() {

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

U32 CPU::xor_A_n8() {

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

U32 CPU::add_HL_r16() {

    // -0HC

    U32 r16 = _register_16( (opcode >> 4u) & 0x03 ); // FIXME get r16 from opcode

    WORD sum = _add_16(
        registers.read_16(HL),
        registers.read_16(r16)
    );

    registers.write_16(HL, sum);

    // 8 T Cycles
    return 8;
}

U32 CPU::dec_r16() {

    
    U32 r16 = _register_16( (opcode >> 4u) & 0x03 ); // FIXME get r16 from opcode

    WORD dec = registers.read_16(r16) - 1;

    registers.write_16(r16, dec);

    // 8 T Cycles
    return 8;
}

U32 CPU::inc_r16() {


    U32 r16 = _register_16( (opcode >> 4u) & 0x03 ); // FIXME get r16 from opcode

    WORD inc = registers.read_16(r16) + 1;

    registers.write_16(r16, inc);

    // 8 T Cycles
    return 8;
}

// Bit Operations Instructions
void CPU::_bit(BYTE b, U32 n) {
    // Z01-

    registers.set_z_flag( ((b >> n) & 0x01) == 0 );

    registers.set_n_flag(0);

    registers.set_h_flag(1);

}

BYTE CPU::_res(BYTE b, U32 n) {
    return (b & ~(1u << n));
}

BYTE CPU::_set(BYTE b, U32 n) {
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

U32 CPU::bit_u3_r8() {

    // Z01-

    U32 r8 = _register_8(opcode & 0x07); // GET r8 from opcode
    U32 u3 = (opcode >> 3u) & 0x07; // GET u3 from opcode

    // std::cout << "r8: " << r8 << '\n' <<
                // "u3: " << u3 << '\n';
    
    _bit(
        registers.read_8(r8),
        u3
    );

    // 8 T Cycles
    return 8;
}

U32 CPU::bit_u3_mHL() {

    // Z01-

    WORD hl = registers.read_16(HL);

    BYTE mHL = mmu.read(hl);

    U32 u3 = (opcode >> 3u) & 0x07;

    _bit(
        mHL,
        u3
    );

    // 12 T Cycles
    return 12;
}

U32 CPU::res_u3_r8() {

    
    U32 r8 = _register_8(opcode & 0x07);
    U32 u3 = (opcode >> 3u) & 0x07;

    BYTE res = _res(
        registers.read_8(r8),
        u3
    );

    registers.write_8(r8, res);

    // 8 T Cycles
    return 8;
}

U32 CPU::res_u3_mHL() {


    WORD hl = registers.read_16(HL);

    BYTE mHL = mmu.read(hl);

    U32 u3 = (opcode >> 3u) & 0x07;

    BYTE res = _res(
        mHL,
        u3
    );

    mmu.write(hl, res);

    // 16 T Cycles
    return 16;
}

U32 CPU::set_u3_r8() {


    U32 r8 = _register_8(opcode & 0x07);
    U32 u3 = (opcode >> 3u) & 0x07;

    BYTE set = _set(
        registers.read_8(r8),
        u3
    );

    registers.write_8(r8, set);

    // 8 T Cycles
    return 8;
}

U32 CPU::set_u3_mHL() {


    WORD hl = registers.read_16(HL);

    BYTE mHL = mmu.read(hl);

    U32 u3 = (opcode >> 3u) & 0x07;

    BYTE set = _set(
        mHL,
        u3
    );

    mmu.write(hl, set);

    // 16 T Cycles
    return 16;
}

U32 CPU::swap_r8() {

    // Z000

    U32 r8 = _register_8(opcode & 0x07);

    BYTE s = _swap(
        registers.read_8(r8)
    );
    
    registers.write_8(r8, s);

    // 8 T Cycles
    return 8;
}

U32 CPU::swap_mHL() {

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

    bool _lsb;

    if(through_carry) {
        _lsb = registers.get_c_flag();
    } else {
        _lsb = ( (b >> 7u) & 0x01 );
    }

    BYTE rl = (b << 1u) | (BYTE) _lsb;

    registers.set_z_flag(rl == 0);

    registers.set_n_flag(0);

    registers.set_h_flag(0);

    registers.set_c_flag( (b >> 7u) & 0x01 );

    return rl;
}

BYTE CPU::_rr(BYTE b, bool through_carry) {
    // Z00C

    bool _msb;

    if(through_carry) {
        _msb = registers.get_c_flag();
    } else {
        _msb = ( b & 0x01 );
    }

    BYTE rr = (BYTE) (_msb << 7u) | (b >> 1u);

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

    bool _msb;

    if(arithmetic) {
        _msb = (b >> 7u) & 0x01;
    } else {
        _msb = 0;
    }

    BYTE sr = ((BYTE) _msb << 7u) | (b >> 1u);

    registers.set_z_flag(sr == 0);

    registers.set_n_flag(0);
    
    registers.set_h_flag(0);

    registers.set_c_flag( b & 0x01 );

    return sr;
}

U32 CPU::rl_r8() {

    // Z00C

    U32 r8 = _register_8(opcode & 0x07);

    BYTE rl = _rl(
        registers.read_8(r8),
        true
    );

    registers.write_8(r8, rl);

    // 8 T Cycles
    return 8;
}

U32 CPU::rl_mHL() {

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

U32 CPU::rla() {

    // 000C

    BYTE rl = _rl(
        registers.read_8(A),
        true
    );

    registers.set_z_flag(0);

    registers.write_8(A, rl);

    // 4 T Cycles
    return 4;
}

U32 CPU::rlc_r8() {

    // Z00C

    U32 r8 = _register_8(opcode & 0x07);

    BYTE rl = _rl(
        registers.read_8(r8),
        false
    );

    registers.write_8(r8, rl);

    // 8 T Cycles
    return 8;
}

U32 CPU::rlc_mHL() {

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

U32 CPU::rlca() {

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

U32 CPU::rr_r8() {

    // Z00C

    U32 r8 = _register_8(opcode & 0x07);

    BYTE rr = _rr(
        registers.read_8(r8),
        true
    );

    registers.write_8(r8, rr);

    // 8 T Cycles
    return 8;
}

U32 CPU::rr_mHL() {

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

U32 CPU::rra() {

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

U32 CPU::rrc_r8() {

    // Z00C

    U32 r8 = _register_8(opcode & 0x07);

    BYTE rr = _rr(
        registers.read_8(r8),
        false
    );

    registers.write_8(r8, rr);

    // 8 T Cycles
    return 8;
}

U32 CPU::rrc_mHL() {

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

U32 CPU::rrca() {

    // 000C
    
    BYTE rr = _rr(
        registers.read_8(A),
        false
    );
    
    registers.set_z_flag(0);

    registers.write_8(A, rr);

    // 4 T Cycles
    return 4;
}

U32 CPU::sla_r8() {

    // Z00C
    
    U32 r8 = _register_8(opcode & 0x07);

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

U32 CPU::sla_mHL() {

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

U32 CPU::sra_r8() {

    // Z00C
    
    U32 r8 = _register_8(opcode & 0x07);

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

U32 CPU::sra_mHL() {

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

U32 CPU::srl_r8() {

    // Z00C
    
    U32 r8 = _register_8(opcode & 0x07);

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

U32 CPU::srl_mHL() {

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
U32 CPU::ld_r8_r8(){
    
    U32 a_r8 = _register_8((opcode >> 3u) & 0x07);
    U32 b_r8 = _register_8(opcode & 0x07);

    // std::cout << "r8_1: " << out_r8(a_r8) << '\n'
        // << "r8_2: " << out_r8(b_r8)
        // << " val: " << (int) registers.read_8(b_r8) << '\n';

    registers.write_8(
        a_r8,
        registers.read_8(b_r8)
    );

    // 4 T Cycles
    return 4;
}

U32 CPU::ld_r8_n8(){

    U32 r8 = _register_8((opcode >> 3u) & 0x07);
    BYTE n8 = mmu.read(PC);

    // std::cout << "r8: " << out_r8(r8) << '\n';
    // std::cout << "n8: " << std::hex << (int) n8 << '\n';

    registers.write_8(
        r8,
        n8
    );

    PC += 1;

    // 8 T Cycles
    return 8;
}

U32 CPU::ld_r16_n16(){

    U32 r16 = _register_16( (opcode >> 4u) & 0x03 );
    WORD n16 = concat(
        mmu.read(PC + 1),
        mmu.read(PC)
    );

    // std::cout << "r16: " << out_r16(r16) << '\n';
    // std::cout << "n16: " << std::hex << (int) n16 << '\n';

    registers.write_16(
        r16,
        n16
    );

    PC += 2;

    // 12 T Cycles
    return 12;
}

U32 CPU::ld_mHL_r8(){

    WORD hl = registers.read_16(HL);
    
    // std::cout << "hl: " << std::hex << (int) hl << '\n';
    U32 r8 = _register_8(opcode & 0x07);
    // std::cout << "r8: " << out_r8(r8) << '\n';
    mmu.write(
        hl,
        registers.read_8(r8)
    );

    // 8 T Cycles
    return 8;
}

U32 CPU::ld_mHL_n8(){

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

U32 CPU::ld_r8_mHL(){

    WORD hl = registers.read_16(HL);

    BYTE mHL = mmu.read(hl);

    U32 r8 = _register_8((opcode >> 3u) & 0x07);

    // std::cout << std::hex << out_r8(r8) << " HL: " << (int) hl <<
    //     " mHL: " << (int) mHL << '\n';

    registers.write_8(r8, mHL);

    // 8 T Cycles
    return 8;
}

U32 CPU::ld_mr16_A(){
    
    U32 r16 = _register_16( (opcode >> 4u) & 0x03 );
    // std::cout << "r16: " << out_r16(r16) << '\n'
        // << "val: " << std::hex << (int) registers.read_16(r16) << '\n';
    mmu.write(
        registers.read_16(r16),
        registers.read_8(A)
    );
    // std::cout << "val: " << std::hex << (int) mmu.read(registers.read_16(r16)) << '\n';

    // 8 T Cycles
    return 8;
}

U32 CPU::ld_mn16_A(){

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

U32 CPU::ldh_mn16_A(){

    // std::cout << "A: " << std::hex << (int) registers.read_8(A)
    //     << '\n' << (int) 0xFF00 + mmu.read(PC) << '\n';

    BYTE n8 = mmu.read(PC);

    mmu.write(
        0xFF00 + n8,
        registers.read_8(A)
    );

    PC += 1;

    // 12 T Cycles
    return 12;
}

U32 CPU::ldh_mC_A(){
    // std::cout << std::hex <<
        // "Address: " <<
        // (int) (0xFF00 + registers.read_8(C)) << '\n';

    mmu.write(
        0xFF00 + registers.read_8(C),
        registers.read_8(A)    
    );

    // 8 T Cycles
    return 8;
}

U32 CPU::ld_A_mr16(){

    U32 r16 = _register_16( (opcode >> 4u) & 0x03 );

    // std::cout << out_r16(r16) << '\n';

    registers.write_8(
        A,
        mmu.read(registers.read_16(r16))
    );

    // 8 T Cycles
    return 8;
}

U32 CPU::ld_A_mn16(){

    WORD n16 = concat(
        mmu.read(PC + 1),
        mmu.read(PC)
    );

    // std::cout << "n16: " << std::hex << (int) n16 << '\n';
    // std::cout << "val: " << std::hex << (int) mmu.read(n16) << '\n';

    registers.write_8(
        A,
        mmu.read(n16)
    );

    PC += 2;

    // 16 T Cycles
    return 16;
}

U32 CPU::ldh_A_mn16(){

    BYTE n8 = mmu.read(PC);

    registers.write_8(
        A,
        mmu.read(0xFF00 + n8)
    );

    PC += 1;

    // 12 T Cycles
    return 12;
}

U32 CPU::ldh_A_mC(){

    registers.write_8(
        A,
        mmu.read(0xFF00 + registers.read_8(C))
    );

    // 8 T Cycles
    return 8;
}

U32 CPU::ld_mHLI_A(){

    WORD hl = registers.read_16(HL);

    mmu.write(
        hl,
        registers.read_8(A)
    );

    registers.write_16(HL, hl + 1);

    // 8 T Cycles
    return 8;
}

U32 CPU::ld_mHLD_A(){

    WORD hl = registers.read_16(HL);
    // std::cout << std::hex << "hl: " << (int) hl << '\n';
    mmu.write(
        hl,
        registers.read_8(A)
    );

    registers.write_16(HL, hl - 1);

    // 8 T Cycles
    return 8;
}

U32 CPU::ld_A_mHLI(){

    WORD hl = registers.read_16(HL);
    // std::cout << "hl: " << std::hex << (int) hl << '\n'
    //     << "val: " << (int) mmu.read(hl) << '\n';
    registers.write_8(
        A,
        mmu.read(hl)
    );

    registers.write_16(HL, hl + 1);

    // 8 T Cycles
    return 8;
}

U32 CPU::ld_A_mHLD(){
    
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
bool CPU::_condition(U32 c) {
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
    // std::cout << "call: " << std::hex << (int) address << '\n';
    mmu.write(SP - 1, msb(PC));

    mmu.write(SP - 2, lsb(PC));

    SP -= 2;

    PC = address;
}

U32 CPU::call_n16() {


    WORD n16 = concat(
        mmu.read(PC + 1),
        mmu.read(PC)
    );

    PC += 2;

    _call(n16);

    // 24 T Cycles
    return 24;
}

U32 CPU::call_cc_n16() {

    WORD n16 = concat(
        mmu.read(PC + 1),
        mmu.read(PC)
    );

    PC += 2;

    bool cc = _condition( (opcode >> 3u) & 0x03 );

    if(cc) {
        _call(n16);

        // 24 T Cycles
        return 24;
    }

    // 12 T Cycles
    return 12;
}

U32 CPU::jp_HL() {


    PC = registers.read_16(HL);

    // 4 T Cycles
    return 4;
}

U32 CPU::jp_n16() {


    WORD n16 = concat(
        mmu.read(PC + 1),
        mmu.read(PC)
    );

    PC = n16;

    // 16 T Cycles
    return 16;
}

U32 CPU::jp_cc_n16() {


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

U32 CPU::jr_n16() {


    SIGNED_BYTE n8 = mmu.read(PC);

    // FIXME signed and unsigend arithmetic?
    PC += n8 + 1;

    // 12 T Cycles
    return 12;
}

U32 CPU::jr_cc_n16() {


    SIGNED_BYTE n8 = mmu.read(PC);

    bool cc = _condition( (opcode >> 3u) & 0x03 );
    
    // std::cout << "opcode: " << std::hex << (int) opcode << '\n';
    // std::cout << "n8: " << std::hex << (int) n8 << '\n';
    // std::cout << "PC current: " << std::hex << (int) PC << '\n';
    // std::cout << "PC relative: " << (int) (PC + n8 + 1) << '\n';

    if(cc) {
        // FIXME signed and unsigend arithmetic?
        PC += n8 + 1;

        // 12 T Cycles
        return 12;
    }

    PC += 1;

    // 8 T Cycles
    return 8;
}

U32 CPU::ret() {

    WORD p = concat(
        mmu.read(SP + 1),
        mmu.read(SP)
    );

    SP += 2;

    PC = p;

    // 16 T Cycles
    return 16;
}

U32 CPU::ret_cc() {


    bool cc = _condition( (opcode >> 3u) & 0x03 );

    if(cc) {
        ret();

        // 20 T Cycles
        return 20;
    }

    // 8 T Cycles
    return 8;
}

U32 CPU::reti() {

    // FIXME set IME

    ret();

    // IME = 1;

    // 16 T Cycles
    return 16;
}

U32 CPU::rst() {


    WORD vec = opcode & 0b00111000;

    _call(vec);

    // 16 T Cycles
    return 16;
}

// Stack Operations Instructions
U32 CPU::add_HL_SP() {

    // -0HC

    WORD sum = _add_16(
        registers.read_16(HL),
        SP
    );

    registers.write_16(HL, sum);

    // 8 T Cycles
    return 8;
}

U32 CPU::add_SP_e8() {


    SIGNED_BYTE e8 = mmu.read(PC);

    // Set flags
    _add_8((BYTE) SP, e8, false);
    registers.set_z_flag(0);

    SP += e8;

    PC += 1;

    // 16 T Cycles
    return 16;
}

U32 CPU::dec_SP() {


    SP -= 1;

    // 8 T Cycles
    return 8;
}

U32 CPU::inc_SP() {


    SP += 1;

    // 8 T Cycles
    return 8;
}

U32 CPU::ld_SP_n16() {
    
    WORD n16 = concat(
        mmu.read(PC + 1),
        mmu.read(PC)
    );

    SP = n16;

    PC += 2;

    // 12 T Cycles
    return 12;
}

U32 CPU::ld_mn16_SP() {


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

U32 CPU::ld_HL_SPe8() {

    // 00HC

    SIGNED_BYTE e8 = mmu.read(PC);
    
    // set right flags
    _add_8((BYTE) SP, e8, false);
    
    registers.set_z_flag(0);

    registers.write_16(
        HL,
        SP + e8
    );
    
    PC += 1;

    // 12 T Cycles
    return 12;
}

U32 CPU::ld_SP_HL() {

    SP = registers.read_16(HL);

    // 8 T Cycles
    return 8;
}

U32 CPU::pop_AF() {

    // ZNHC

    BYTE a = mmu.read(SP + 1),
         f = mmu.read(SP) & 0xF0;
        
    

    registers.write_16(AF, concat(a, f));

    SP += 2;

    // 12 T Cycles
    return 12;
}

U32 CPU::pop_r16() {


    U32 r16 = _register_16( (opcode >> 4u) & 0x03 );

    BYTE hi = mmu.read(SP + 1),
         lo = mmu.read(SP);

    registers.write_16(
        r16,
        concat(hi, lo)
    );

    SP += 2;

    // 12 T Cycles
    return 12;
}

U32 CPU::push_AF() {


    mmu.write(
        SP - 1,
        registers.read_8(A)
    );

    mmu.write(
        SP - 2,
        registers.read_8(F) & 0xF0    
    );
    
    SP -= 2;

    // 16 T Cycles
    return 16;
}

U32 CPU::push_r16() {


    U32 r16 = _register_16( (opcode >> 4u) & 0x03 );

    WORD r = registers.read_16(r16);

    // std::cout << std::hex << out_r16(r16) << ": " << (int) r << '\n';

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
U32 CPU::ccf() {

    // -00C

    registers.set_n_flag(0);

    registers.set_h_flag(0);

    registers.set_c_flag(
        !registers.get_c_flag()
    );

    // 4 T Cycles
    return 4;
}

U32 CPU::cpl() {

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

U32 CPU::daa() {

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
        if( c || (a > 0x99) ) {
            a += 0x60;
            registers.set_c_flag(1);
        }
        if( h || ((a & 0x0F) > 0x09) ) {
            a += 0x06;
        }
    }

    registers.set_z_flag(a == 0);

    registers.set_h_flag(0);

    registers.write_8(A, a);

    // 4 T Cycles
    return 4;
}

U32 CPU::di() {


    // IME = 0;

    // 4 T Cycles
    return 4;
}

U32 CPU::ei() {
    // std::cout << "Enable Interrupts\n";
    // Set IME on next instr
    // EI = true;

    // 4 T Cycles FIXME add next instr cycles
    return 4;
}

U32 CPU::halt() {
    // std::cout << "halted\n";
    halt_mode = true;
    
    // Wait for interrupt
    return 0;
}

U32 CPU::nop() {

    // no operation

    // 4 T Cycles
    return 4;
}

U32 CPU::scf() {

    // -001

    registers.set_n_flag(0);

    registers.set_h_flag(0);

    registers.set_c_flag(1);

    // 4 T Cycles
    return 4;
}

U32 CPU::stop() {
    
    halt_mode = true;

    // Wait for interrupt or joypad
    return 0;
}


void CPU::init_instr_tables() {
    instr[0x00] = &CPU::nop;
    instr[0x01] = &CPU::ld_r16_n16;
    instr[0x02] = &CPU::ld_mr16_A;
    instr[0x03] = &CPU::inc_r16;
    instr[0x04] = &CPU::inc_r8;
    instr[0x05] = &CPU::dec_r8;
    instr[0x06] = &CPU::ld_r8_n8;
    instr[0x07] = &CPU::rlca;
    instr[0x08] = &CPU::ld_mn16_SP;
    instr[0x09] = &CPU::add_HL_r16;
    instr[0x0A] = &CPU::ld_A_mr16;
    instr[0x0B] = &CPU::dec_r16;
    instr[0x0C] = &CPU::inc_r8;
    instr[0x0D] = &CPU::dec_r8;
    instr[0x0E] = &CPU::ld_r8_n8;
    instr[0x0F] = &CPU::rrca;

    instr[0x10] = &CPU::stop;
    instr[0x11] = &CPU::ld_r16_n16;
    instr[0x12] = &CPU::ld_mr16_A;
    instr[0x13] = &CPU::inc_r16;
    instr[0x14] = &CPU::inc_r8;
    instr[0x15] = &CPU::dec_r8;
    instr[0x16] = &CPU::ld_r8_n8;
    instr[0x17] = &CPU::rla;
    instr[0x18] = &CPU::jr_n16;
    instr[0x19] = &CPU::add_HL_r16;
    instr[0x1A] = &CPU::ld_A_mr16;
    instr[0x1B] = &CPU::dec_r16;
    instr[0x1C] = &CPU::inc_r8;
    instr[0x1D] = &CPU::dec_r8;
    instr[0x1E] = &CPU::ld_r8_n8;
    instr[0x1F] = &CPU::rra;

    instr[0x20] = &CPU::jr_cc_n16;
    instr[0x21] = &CPU::ld_r16_n16;
    instr[0x22] = &CPU::ld_mHLI_A;
    instr[0x23] = &CPU::inc_r16;
    instr[0x24] = &CPU::inc_r8;
    instr[0x25] = &CPU::dec_r8;
    instr[0x26] = &CPU::ld_r8_n8;
    instr[0x27] = &CPU::daa;
    instr[0x28] = &CPU::jr_cc_n16;
    instr[0x29] = &CPU::add_HL_r16;
    instr[0x2A] = &CPU::ld_A_mHLI;
    instr[0x2B] = &CPU::dec_r16;
    instr[0x2C] = &CPU::inc_r8;
    instr[0x2D] = &CPU::dec_r8;
    instr[0x2E] = &CPU::ld_r8_n8;
    instr[0x2F] = &CPU::cpl;
    
    instr[0x30] = &CPU::jr_cc_n16;
    instr[0x31] = &CPU::ld_SP_n16;
    instr[0x32] = &CPU::ld_mHLD_A;
    instr[0x33] = &CPU::inc_SP;
    instr[0x34] = &CPU::inc_mHL;
    instr[0x35] = &CPU::dec_mHL;
    instr[0x36] = &CPU::ld_mHL_n8;
    instr[0x37] = &CPU::scf;
    instr[0x38] = &CPU::jr_cc_n16;
    instr[0x39] = &CPU::add_HL_SP;
    instr[0x3A] = &CPU::ld_A_mHLD;
    instr[0x3B] = &CPU::dec_SP;
    instr[0x3C] = &CPU::inc_r8;
    instr[0x3D] = &CPU::dec_r8;
    instr[0x3E] = &CPU::ld_r8_n8;
    instr[0x3F] = &CPU::ccf;

    instr[0x40] = &CPU::ld_r8_r8;
    instr[0x41] = &CPU::ld_r8_r8;
    instr[0x42] = &CPU::ld_r8_r8;
    instr[0x43] = &CPU::ld_r8_r8;
    instr[0x44] = &CPU::ld_r8_r8;
    instr[0x45] = &CPU::ld_r8_r8;
    instr[0x46] = &CPU::ld_r8_mHL;
    instr[0x47] = &CPU::ld_r8_r8;
    instr[0x48] = &CPU::ld_r8_r8;
    instr[0x49] = &CPU::ld_r8_r8;
    instr[0x4A] = &CPU::ld_r8_r8;
    instr[0x4B] = &CPU::ld_r8_r8;
    instr[0x4C] = &CPU::ld_r8_r8;
    instr[0x4D] = &CPU::ld_r8_r8;
    instr[0x4E] = &CPU::ld_r8_mHL;
    instr[0x4F] = &CPU::ld_r8_r8;

    instr[0x50] = &CPU::ld_r8_r8;
    instr[0x51] = &CPU::ld_r8_r8;
    instr[0x52] = &CPU::ld_r8_r8;
    instr[0x53] = &CPU::ld_r8_r8;
    instr[0x54] = &CPU::ld_r8_r8;
    instr[0x55] = &CPU::ld_r8_r8;
    instr[0x56] = &CPU::ld_r8_mHL;
    instr[0x57] = &CPU::ld_r8_r8;
    instr[0x58] = &CPU::ld_r8_r8;
    instr[0x59] = &CPU::ld_r8_r8;
    instr[0x5A] = &CPU::ld_r8_r8;
    instr[0x5B] = &CPU::ld_r8_r8;
    instr[0x5C] = &CPU::ld_r8_r8;
    instr[0x5D] = &CPU::ld_r8_r8;
    instr[0x5E] = &CPU::ld_r8_mHL;
    instr[0x5F] = &CPU::ld_r8_r8;

    instr[0x60] = &CPU::ld_r8_r8;
    instr[0x61] = &CPU::ld_r8_r8;
    instr[0x62] = &CPU::ld_r8_r8;
    instr[0x63] = &CPU::ld_r8_r8;
    instr[0x64] = &CPU::ld_r8_r8;
    instr[0x65] = &CPU::ld_r8_r8;
    instr[0x66] = &CPU::ld_r8_mHL;
    instr[0x67] = &CPU::ld_r8_r8;
    instr[0x68] = &CPU::ld_r8_r8;
    instr[0x69] = &CPU::ld_r8_r8;
    instr[0x6A] = &CPU::ld_r8_r8;
    instr[0x6B] = &CPU::ld_r8_r8;
    instr[0x6C] = &CPU::ld_r8_r8;
    instr[0x6D] = &CPU::ld_r8_r8;
    instr[0x6E] = &CPU::ld_r8_mHL;
    instr[0x6F] = &CPU::ld_r8_r8;

    instr[0x70] = &CPU::ld_mHL_r8;
    instr[0x71] = &CPU::ld_mHL_r8;
    instr[0x72] = &CPU::ld_mHL_r8;
    instr[0x73] = &CPU::ld_mHL_r8;
    instr[0x74] = &CPU::ld_mHL_r8;
    instr[0x75] = &CPU::ld_mHL_r8;
    instr[0x76] = &CPU::halt;
    instr[0x77] = &CPU::ld_mHL_r8;
    instr[0x78] = &CPU::ld_r8_r8;
    instr[0x79] = &CPU::ld_r8_r8;
    instr[0x7A] = &CPU::ld_r8_r8;
    instr[0x7B] = &CPU::ld_r8_r8;
    instr[0x7C] = &CPU::ld_r8_r8;
    instr[0x7D] = &CPU::ld_r8_r8;
    instr[0x7E] = &CPU::ld_r8_mHL;
    instr[0x7F] = &CPU::ld_r8_r8;

    instr[0x80] = &CPU::add_A_r8;
    instr[0x81] = &CPU::add_A_r8;
    instr[0x82] = &CPU::add_A_r8;
    instr[0x83] = &CPU::add_A_r8;
    instr[0x84] = &CPU::add_A_r8;
    instr[0x85] = &CPU::add_A_r8;
    instr[0x86] = &CPU::add_A_mHL;
    instr[0x87] = &CPU::add_A_r8;
    instr[0x88] = &CPU::adc_A_r8;
    instr[0x89] = &CPU::adc_A_r8;
    instr[0x8A] = &CPU::adc_A_r8;
    instr[0x8B] = &CPU::adc_A_r8;
    instr[0x8C] = &CPU::adc_A_r8;
    instr[0x8D] = &CPU::adc_A_r8;
    instr[0x8E] = &CPU::adc_A_mHL;
    instr[0x8F] = &CPU::adc_A_r8;

    instr[0x90] = &CPU::sub_A_r8;
    instr[0x91] = &CPU::sub_A_r8;
    instr[0x92] = &CPU::sub_A_r8;
    instr[0x93] = &CPU::sub_A_r8;
    instr[0x94] = &CPU::sub_A_r8;
    instr[0x95] = &CPU::sub_A_r8;
    instr[0x96] = &CPU::sub_A_mHL;
    instr[0x97] = &CPU::sub_A_r8;
    instr[0x98] = &CPU::sbc_A_r8;
    instr[0x99] = &CPU::sbc_A_r8;
    instr[0x9A] = &CPU::sbc_A_r8;
    instr[0x9B] = &CPU::sbc_A_r8;
    instr[0x9C] = &CPU::sbc_A_r8;
    instr[0x9D] = &CPU::sbc_A_r8;
    instr[0x9E] = &CPU::sbc_A_mHL;
    instr[0x9F] = &CPU::sbc_A_r8;

    instr[0xA0] = &CPU::and_A_r8;
    instr[0xA1] = &CPU::and_A_r8;
    instr[0xA2] = &CPU::and_A_r8;
    instr[0xA3] = &CPU::and_A_r8;
    instr[0xA4] = &CPU::and_A_r8;
    instr[0xA5] = &CPU::and_A_r8;
    instr[0xA6] = &CPU::and_A_mHL;
    instr[0xA7] = &CPU::and_A_r8;
    instr[0xA8] = &CPU::xor_A_r8;
    instr[0xA9] = &CPU::xor_A_r8;
    instr[0xAA] = &CPU::xor_A_r8;
    instr[0xAB] = &CPU::xor_A_r8;
    instr[0xAC] = &CPU::xor_A_r8;
    instr[0xAD] = &CPU::xor_A_r8;
    instr[0xAE] = &CPU::xor_A_mHL;
    instr[0xAF] = &CPU::xor_A_r8;

    instr[0xB0] = &CPU::or_A_r8;
    instr[0xB1] = &CPU::or_A_r8;
    instr[0xB2] = &CPU::or_A_r8;
    instr[0xB3] = &CPU::or_A_r8;
    instr[0xB4] = &CPU::or_A_r8;
    instr[0xB5] = &CPU::or_A_r8;
    instr[0xB6] = &CPU::or_A_mHL;
    instr[0xB7] = &CPU::or_A_r8;
    instr[0xB8] = &CPU::cp_A_r8;
    instr[0xB9] = &CPU::cp_A_r8;
    instr[0xBA] = &CPU::cp_A_r8;
    instr[0xBB] = &CPU::cp_A_r8;
    instr[0xBC] = &CPU::cp_A_r8;
    instr[0xBD] = &CPU::cp_A_r8;
    instr[0xBE] = &CPU::cp_A_mHL;
    instr[0xBF] = &CPU::cp_A_r8;

    instr[0xC0] = &CPU::ret_cc;
    instr[0xC1] = &CPU::pop_r16;
    instr[0xC2] = &CPU::jp_cc_n16;
    instr[0xC3] = &CPU::jp_n16;
    instr[0xC4] = &CPU::call_cc_n16;
    instr[0xC5] = &CPU::push_r16;
    instr[0xC6] = &CPU::add_A_n8;
    instr[0xC7] = &CPU::rst;
    instr[0xC8] = &CPU::ret_cc;
    instr[0xC9] = &CPU::ret;
    instr[0xCA] = &CPU::jp_cc_n16;
    instr[0xCB] = &CPU::cb_prefix;
    instr[0xCC] = &CPU::call_cc_n16;
    instr[0xCD] = &CPU::call_n16;
    instr[0xCE] = &CPU::adc_A_n8;
    instr[0xCF] = &CPU::rst;

    instr[0xD0] = &CPU::ret_cc;
    instr[0xD1] = &CPU::pop_r16;
    instr[0xD2] = &CPU::jp_cc_n16;
    // instr[0xD3] ILLEGAL
    instr[0xD4] = &CPU::call_cc_n16;
    instr[0xD5] = &CPU::push_r16;
    instr[0xD6] = &CPU::sub_A_n8;
    instr[0xD7] = &CPU::rst;
    instr[0xD8] = &CPU::ret_cc;
    instr[0xD9] = &CPU::reti;
    instr[0xDA] = &CPU::jp_cc_n16;
    // instr[0xDB] ILLEGAL
    instr[0xDC] = &CPU::call_cc_n16;
    // instr[0xDD] ILLEGAL
    instr[0xDE] = &CPU::sbc_A_n8;
    instr[0xDF] = &CPU::rst;

    instr[0xE0] = &CPU::ldh_mn16_A;
    instr[0xE1] = &CPU::pop_r16;
    instr[0xE2] = &CPU::ldh_mC_A;
    // instr[0xE3] ILLEGAL
    // instr[0xE4] ILLEGAL
    instr[0xE5] = &CPU::push_r16;
    instr[0xE6] = &CPU::and_A_n8;
    instr[0xE7] = &CPU::rst;
    instr[0xE8] = &CPU::add_SP_e8;
    instr[0xE9] = &CPU::jp_HL;
    instr[0xEA] = &CPU::ld_mn16_A;
    // instr[0xEB] ILLEGAL
    // instr[0xEC] ILLEGAL
    // instr[0xED] ILLEGAL
    instr[0xEE] = &CPU::xor_A_n8;
    instr[0xEF] = &CPU::rst;

    instr[0xF0] = &CPU::ldh_A_mn16;
    instr[0xF1] = &CPU::pop_AF;
    instr[0xF2] = &CPU::ldh_A_mC;
    instr[0xF3] = &CPU::di;
    // instr[0xF4] ILLEGAL
    instr[0xF5] = &CPU::push_r16;
    instr[0xF6] = &CPU::or_A_n8;
    instr[0xF7] = &CPU::rst;
    instr[0xF8] = &CPU::ld_HL_SPe8;
    instr[0xF9] = &CPU::ld_SP_HL;
    instr[0xFA] = &CPU::ld_A_mn16;
    instr[0xFB] = &CPU::ei;
    // instr[0xFC] ILLEGAL
    // instr[0xFD] ILLEGAL
    instr[0xFE] = &CPU::cp_A_n8;
    instr[0xFF] = &CPU::rst;

    cb_instr[0x00] = &CPU::rlc_r8;
    cb_instr[0x01] = &CPU::rlc_r8;
    cb_instr[0x02] = &CPU::rlc_r8;
    cb_instr[0x03] = &CPU::rlc_r8;
    cb_instr[0x04] = &CPU::rlc_r8;
    cb_instr[0x05] = &CPU::rlc_r8;
    cb_instr[0x06] = &CPU::rlc_mHL;
    cb_instr[0x07] = &CPU::rlc_r8;
    cb_instr[0x08] = &CPU::rrc_r8;
    cb_instr[0x09] = &CPU::rrc_r8;
    cb_instr[0x0A] = &CPU::rrc_r8;
    cb_instr[0x0B] = &CPU::rrc_r8;
    cb_instr[0x0C] = &CPU::rrc_r8;
    cb_instr[0x0D] = &CPU::rrc_r8;
    cb_instr[0x0E] = &CPU::rrc_mHL;
    cb_instr[0x0F] = &CPU::rrc_r8;

    cb_instr[0x10] = &CPU::rl_r8;
    cb_instr[0x11] = &CPU::rl_r8;
    cb_instr[0x12] = &CPU::rl_r8;
    cb_instr[0x13] = &CPU::rl_r8;
    cb_instr[0x14] = &CPU::rl_r8;
    cb_instr[0x15] = &CPU::rl_r8;
    cb_instr[0x16] = &CPU::rl_mHL;
    cb_instr[0x17] = &CPU::rl_r8;
    cb_instr[0x18] = &CPU::rr_r8;
    cb_instr[0x19] = &CPU::rr_r8;
    cb_instr[0x1A] = &CPU::rr_r8;
    cb_instr[0x1B] = &CPU::rr_r8;
    cb_instr[0x1C] = &CPU::rr_r8;
    cb_instr[0x1D] = &CPU::rr_r8;
    cb_instr[0x1E] = &CPU::rr_mHL;
    cb_instr[0x1F] = &CPU::rr_r8;

    cb_instr[0x20] = &CPU::sla_r8;
    cb_instr[0x21] = &CPU::sla_r8;
    cb_instr[0x22] = &CPU::sla_r8;
    cb_instr[0x23] = &CPU::sla_r8;
    cb_instr[0x24] = &CPU::sla_r8;
    cb_instr[0x25] = &CPU::sla_r8;
    cb_instr[0x26] = &CPU::sla_mHL;
    cb_instr[0x27] = &CPU::sla_r8;
    cb_instr[0x28] = &CPU::sra_r8;
    cb_instr[0x29] = &CPU::sra_r8;
    cb_instr[0x2A] = &CPU::sra_r8;
    cb_instr[0x2B] = &CPU::sra_r8;
    cb_instr[0x2C] = &CPU::sra_r8;
    cb_instr[0x2D] = &CPU::sra_r8;
    cb_instr[0x2E] = &CPU::sra_mHL;
    cb_instr[0x2F] = &CPU::sra_r8;

    cb_instr[0x30] = &CPU::swap_r8;
    cb_instr[0x31] = &CPU::swap_r8;
    cb_instr[0x32] = &CPU::swap_r8;
    cb_instr[0x33] = &CPU::swap_r8;
    cb_instr[0x34] = &CPU::swap_r8;
    cb_instr[0x35] = &CPU::swap_r8;
    cb_instr[0x36] = &CPU::swap_mHL;
    cb_instr[0x37] = &CPU::swap_r8;
    cb_instr[0x38] = &CPU::srl_r8;
    cb_instr[0x39] = &CPU::srl_r8;
    cb_instr[0x3A] = &CPU::srl_r8;
    cb_instr[0x3B] = &CPU::srl_r8;
    cb_instr[0x3C] = &CPU::srl_r8;
    cb_instr[0x3D] = &CPU::srl_r8;
    cb_instr[0x3E] = &CPU::srl_mHL;
    cb_instr[0x3F] = &CPU::srl_r8;

    cb_instr[0x40] = &CPU::bit_u3_r8;
    cb_instr[0x41] = &CPU::bit_u3_r8;
    cb_instr[0x42] = &CPU::bit_u3_r8;
    cb_instr[0x43] = &CPU::bit_u3_r8;
    cb_instr[0x44] = &CPU::bit_u3_r8;
    cb_instr[0x45] = &CPU::bit_u3_r8;
    cb_instr[0x46] = &CPU::bit_u3_mHL;
    cb_instr[0x47] = &CPU::bit_u3_r8;
    cb_instr[0x48] = &CPU::bit_u3_r8;
    cb_instr[0x49] = &CPU::bit_u3_r8;
    cb_instr[0x4A] = &CPU::bit_u3_r8;
    cb_instr[0x4B] = &CPU::bit_u3_r8;
    cb_instr[0x4C] = &CPU::bit_u3_r8;
    cb_instr[0x4D] = &CPU::bit_u3_r8;
    cb_instr[0x4E] = &CPU::bit_u3_mHL;
    cb_instr[0x4F] = &CPU::bit_u3_r8;

    cb_instr[0x50] = &CPU::bit_u3_r8;
    cb_instr[0x51] = &CPU::bit_u3_r8;
    cb_instr[0x52] = &CPU::bit_u3_r8;
    cb_instr[0x53] = &CPU::bit_u3_r8;
    cb_instr[0x54] = &CPU::bit_u3_r8;
    cb_instr[0x55] = &CPU::bit_u3_r8;
    cb_instr[0x56] = &CPU::bit_u3_mHL;
    cb_instr[0x57] = &CPU::bit_u3_r8;
    cb_instr[0x58] = &CPU::bit_u3_r8;
    cb_instr[0x59] = &CPU::bit_u3_r8;
    cb_instr[0x5A] = &CPU::bit_u3_r8;
    cb_instr[0x5B] = &CPU::bit_u3_r8;
    cb_instr[0x5C] = &CPU::bit_u3_r8;
    cb_instr[0x5D] = &CPU::bit_u3_r8;
    cb_instr[0x5E] = &CPU::bit_u3_mHL;
    cb_instr[0x5F] = &CPU::bit_u3_r8;

    cb_instr[0x60] = &CPU::bit_u3_r8;
    cb_instr[0x61] = &CPU::bit_u3_r8;
    cb_instr[0x62] = &CPU::bit_u3_r8;
    cb_instr[0x63] = &CPU::bit_u3_r8;
    cb_instr[0x64] = &CPU::bit_u3_r8;
    cb_instr[0x65] = &CPU::bit_u3_r8;
    cb_instr[0x66] = &CPU::bit_u3_mHL;
    cb_instr[0x67] = &CPU::bit_u3_r8;
    cb_instr[0x68] = &CPU::bit_u3_r8;
    cb_instr[0x69] = &CPU::bit_u3_r8;
    cb_instr[0x6A] = &CPU::bit_u3_r8;
    cb_instr[0x6B] = &CPU::bit_u3_r8;
    cb_instr[0x6C] = &CPU::bit_u3_r8;
    cb_instr[0x6D] = &CPU::bit_u3_r8;
    cb_instr[0x6E] = &CPU::bit_u3_mHL;
    cb_instr[0x6F] = &CPU::bit_u3_r8;

    cb_instr[0x70] = &CPU::bit_u3_r8;
    cb_instr[0x71] = &CPU::bit_u3_r8;
    cb_instr[0x72] = &CPU::bit_u3_r8;
    cb_instr[0x73] = &CPU::bit_u3_r8;
    cb_instr[0x74] = &CPU::bit_u3_r8;
    cb_instr[0x75] = &CPU::bit_u3_r8;
    cb_instr[0x76] = &CPU::bit_u3_mHL;
    cb_instr[0x77] = &CPU::bit_u3_r8;
    cb_instr[0x78] = &CPU::bit_u3_r8;
    cb_instr[0x79] = &CPU::bit_u3_r8;
    cb_instr[0x7A] = &CPU::bit_u3_r8;
    cb_instr[0x7B] = &CPU::bit_u3_r8;
    cb_instr[0x7C] = &CPU::bit_u3_r8;
    cb_instr[0x7D] = &CPU::bit_u3_r8;
    cb_instr[0x7E] = &CPU::bit_u3_mHL;
    cb_instr[0x7F] = &CPU::bit_u3_r8;

    cb_instr[0x80] = &CPU::res_u3_r8;
    cb_instr[0x81] = &CPU::res_u3_r8;
    cb_instr[0x82] = &CPU::res_u3_r8;
    cb_instr[0x83] = &CPU::res_u3_r8;
    cb_instr[0x84] = &CPU::res_u3_r8;
    cb_instr[0x85] = &CPU::res_u3_r8;
    cb_instr[0x86] = &CPU::res_u3_mHL;
    cb_instr[0x87] = &CPU::res_u3_r8;
    cb_instr[0x88] = &CPU::res_u3_r8;
    cb_instr[0x89] = &CPU::res_u3_r8;
    cb_instr[0x8A] = &CPU::res_u3_r8;
    cb_instr[0x8B] = &CPU::res_u3_r8;
    cb_instr[0x8C] = &CPU::res_u3_r8;
    cb_instr[0x8D] = &CPU::res_u3_r8;
    cb_instr[0x8E] = &CPU::res_u3_mHL;
    cb_instr[0x8F] = &CPU::res_u3_r8;

    cb_instr[0x90] = &CPU::res_u3_r8;
    cb_instr[0x91] = &CPU::res_u3_r8;
    cb_instr[0x92] = &CPU::res_u3_r8;
    cb_instr[0x93] = &CPU::res_u3_r8;
    cb_instr[0x94] = &CPU::res_u3_r8;
    cb_instr[0x95] = &CPU::res_u3_r8;
    cb_instr[0x96] = &CPU::res_u3_mHL;
    cb_instr[0x97] = &CPU::res_u3_r8;
    cb_instr[0x98] = &CPU::res_u3_r8;
    cb_instr[0x99] = &CPU::res_u3_r8;
    cb_instr[0x9A] = &CPU::res_u3_r8;
    cb_instr[0x9B] = &CPU::res_u3_r8;
    cb_instr[0x9C] = &CPU::res_u3_r8;
    cb_instr[0x9D] = &CPU::res_u3_r8;
    cb_instr[0x9E] = &CPU::res_u3_mHL;
    cb_instr[0x9F] = &CPU::res_u3_r8;

    cb_instr[0xA0] = &CPU::res_u3_r8;
    cb_instr[0xA1] = &CPU::res_u3_r8;
    cb_instr[0xA2] = &CPU::res_u3_r8;
    cb_instr[0xA3] = &CPU::res_u3_r8;
    cb_instr[0xA4] = &CPU::res_u3_r8;
    cb_instr[0xA5] = &CPU::res_u3_r8;
    cb_instr[0xA6] = &CPU::res_u3_mHL;
    cb_instr[0xA7] = &CPU::res_u3_r8;
    cb_instr[0xA8] = &CPU::res_u3_r8;
    cb_instr[0xA9] = &CPU::res_u3_r8;
    cb_instr[0xAA] = &CPU::res_u3_r8;
    cb_instr[0xAB] = &CPU::res_u3_r8;
    cb_instr[0xAC] = &CPU::res_u3_r8;
    cb_instr[0xAD] = &CPU::res_u3_r8;
    cb_instr[0xAE] = &CPU::res_u3_mHL;
    cb_instr[0xAF] = &CPU::res_u3_r8;

    cb_instr[0xB0] = &CPU::res_u3_r8;
    cb_instr[0xB1] = &CPU::res_u3_r8;
    cb_instr[0xB2] = &CPU::res_u3_r8;
    cb_instr[0xB3] = &CPU::res_u3_r8;
    cb_instr[0xB4] = &CPU::res_u3_r8;
    cb_instr[0xB5] = &CPU::res_u3_r8;
    cb_instr[0xB6] = &CPU::res_u3_mHL;
    cb_instr[0xB7] = &CPU::res_u3_r8;
    cb_instr[0xB8] = &CPU::res_u3_r8;
    cb_instr[0xB9] = &CPU::res_u3_r8;
    cb_instr[0xBA] = &CPU::res_u3_r8;
    cb_instr[0xBB] = &CPU::res_u3_r8;
    cb_instr[0xBC] = &CPU::res_u3_r8;
    cb_instr[0xBD] = &CPU::res_u3_r8;
    cb_instr[0xBE] = &CPU::res_u3_mHL;
    cb_instr[0xBF] = &CPU::res_u3_r8;

    cb_instr[0xC0] = &CPU::set_u3_r8;
    cb_instr[0xC1] = &CPU::set_u3_r8;
    cb_instr[0xC2] = &CPU::set_u3_r8;
    cb_instr[0xC3] = &CPU::set_u3_r8;
    cb_instr[0xC4] = &CPU::set_u3_r8;
    cb_instr[0xC5] = &CPU::set_u3_r8;
    cb_instr[0xC6] = &CPU::set_u3_mHL;
    cb_instr[0xC7] = &CPU::set_u3_r8;
    cb_instr[0xC8] = &CPU::set_u3_r8;
    cb_instr[0xC9] = &CPU::set_u3_r8;
    cb_instr[0xCA] = &CPU::set_u3_r8;
    cb_instr[0xCB] = &CPU::set_u3_r8;
    cb_instr[0xCC] = &CPU::set_u3_r8;
    cb_instr[0xCD] = &CPU::set_u3_r8;
    cb_instr[0xCE] = &CPU::set_u3_mHL;
    cb_instr[0xCF] = &CPU::set_u3_r8;

    cb_instr[0xD0] = &CPU::set_u3_r8;
    cb_instr[0xD1] = &CPU::set_u3_r8;
    cb_instr[0xD2] = &CPU::set_u3_r8;
    cb_instr[0xD3] = &CPU::set_u3_r8;
    cb_instr[0xD4] = &CPU::set_u3_r8;
    cb_instr[0xD5] = &CPU::set_u3_r8;
    cb_instr[0xD6] = &CPU::set_u3_mHL;
    cb_instr[0xD7] = &CPU::set_u3_r8;
    cb_instr[0xD8] = &CPU::set_u3_r8;
    cb_instr[0xD9] = &CPU::set_u3_r8;
    cb_instr[0xDA] = &CPU::set_u3_r8;
    cb_instr[0xDB] = &CPU::set_u3_r8;
    cb_instr[0xDC] = &CPU::set_u3_r8;
    cb_instr[0xDD] = &CPU::set_u3_r8;
    cb_instr[0xDE] = &CPU::set_u3_mHL;
    cb_instr[0xDF] = &CPU::set_u3_r8;

    cb_instr[0xE0] = &CPU::set_u3_r8;
    cb_instr[0xE1] = &CPU::set_u3_r8;
    cb_instr[0xE2] = &CPU::set_u3_r8;
    cb_instr[0xE3] = &CPU::set_u3_r8;
    cb_instr[0xE4] = &CPU::set_u3_r8;
    cb_instr[0xE5] = &CPU::set_u3_r8;
    cb_instr[0xE6] = &CPU::set_u3_mHL;
    cb_instr[0xE7] = &CPU::set_u3_r8;
    cb_instr[0xE8] = &CPU::set_u3_r8;
    cb_instr[0xE9] = &CPU::set_u3_r8;
    cb_instr[0xEA] = &CPU::set_u3_r8;
    cb_instr[0xEB] = &CPU::set_u3_r8;
    cb_instr[0xEC] = &CPU::set_u3_r8;
    cb_instr[0xED] = &CPU::set_u3_r8;
    cb_instr[0xEE] = &CPU::set_u3_mHL;
    cb_instr[0xEF] = &CPU::set_u3_r8;

    cb_instr[0xF0] = &CPU::set_u3_r8;
    cb_instr[0xF1] = &CPU::set_u3_r8;
    cb_instr[0xF2] = &CPU::set_u3_r8;
    cb_instr[0xF3] = &CPU::set_u3_r8;
    cb_instr[0xF4] = &CPU::set_u3_r8;
    cb_instr[0xF5] = &CPU::set_u3_r8;
    cb_instr[0xF6] = &CPU::set_u3_mHL;
    cb_instr[0xF7] = &CPU::set_u3_r8;
    cb_instr[0xF8] = &CPU::set_u3_r8;
    cb_instr[0xF9] = &CPU::set_u3_r8;
    cb_instr[0xFA] = &CPU::set_u3_r8;
    cb_instr[0xFB] = &CPU::set_u3_r8;
    cb_instr[0xFC] = &CPU::set_u3_r8;
    cb_instr[0xFD] = &CPU::set_u3_r8;
    cb_instr[0xFE] = &CPU::set_u3_mHL;
    cb_instr[0xFF] = &CPU::set_u3_r8;
}
