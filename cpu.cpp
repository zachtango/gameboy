#include "constants.h"
#include "helpers.h"
#include "cpu.h"


/*
    Instructions
*/

// 8 bit Arithmetic and Logic Instructions
// FIXME IMPLEMENT ALU AND CARRIES
void CPU::_add_A_r8(bool carry) {
    // Z0HC

    UINT r8;

    BYTE sum = _add(
        registers.read_8(A),
        registers.read_8(r8)
    );

    registers.write_8(A, sum);
}

void CPU::_add_A_mHL(bool carry) {
    // Z0HC

    WORD hl = registers.read_16(HL);

    BYTE mHL = mmu.read(hl);

    BYTE sum = _add(
        registers.read_8(A),
        mHL 
    );

    registers.write_8(A, sum);
}

void CPU::_add_A_n8(bool carry) {
    // Z0HC

    BYTE n8; // FIXME get n8 from opcode

    BYTE sum = _add(
        registers.read_8(A),
        mmu.read(n8)
    );

    registers.write_8(A, sum);
}

void CPU::_sub_A_r8(bool carry) {
    // Z0HC

    UINT r8;

    BYTE sum = _sub(
        registers.read_8(A),
        registers.read_8(r8)
    );

    registers.write_8(A, sum);
}

void CPU::_sub_A_mHL(bool carry) {
    // Z0HC

    WORD hl = registers.read_16(HL);

    BYTE mHL = mmu.read(hl);

    BYTE sum = _sub(
        registers.read_8(A),
        mHL 
    );

    registers.write_8(A, sum);
}

void CPU::_sub_A_n8(bool carry) {
    // Z0HC

    BYTE n8; // FIXME get n8 from opcode

    BYTE sum = _sub(
        registers.read_8(A),
        mmu.read(n8)
    );

    registers.write_8(A, sum);
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

    UINT r8; // get r8 from opcode fIXME

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

    BYTE n8; // FIXME get n8 from opcode

    BYTE a = _and(
        registers.read_8(A),
        mmu.read(n8)
    );

    registers.write_8(A, a);

    // 8 T Cycles
    return 8;
}

UINT CPU::cp_A_r8() {
    // Z1HC

    UINT r8; // FIXME get r8 from opcode

    _sub(
        registers.read_8(A),
        registers.read_8(r8)  
    );

    // 4 T Cycles
    return 4;
}

UINT CPU::cp_A_mHL() {
    // Z1HC

    WORD hl = registers.read_16(HL);

    BYTE mHL = mmu.read(hl);

    _sub(
        registers.read_8(A),
        mHL 
    );

    // 8 T Cycles
    return 8;
}

UINT CPU::cp_A_n8() {
    // Z1HC

    BYTE n8; // FIXME get n8 from opcode

    _sub(
        registers.read_8(A),
        mmu.read(n8)
    );

    // 8 T Cycles
    return 8;
}

UINT CPU::dec_r8() {
    // ZNH-

    UINT r8; // FIXME get r8 from opcode

    BYTE diff = _sub(
        registers.read_8(r8),
        1
    );

    registers.write_8(r8, diff);

    // 4 T Cycles
    return 4;
}

UINT CPU::dec_mHL() {
    // ZNH-

    WORD hl = registers.read_16(HL);

    BYTE mHL = mmu.read(hl);

    BYTE diff = _sub(
        mHL,
        1
    );

    mmu.write(hl, diff);

    // 12 T Cycles
    return 12;
}

UINT CPU::inc_r8() {
    // Z0H-

    UINT r8; // FIXME get r8 from opcode

    BYTE sum = _add(
        registers.read_8(r8),
        1
    );

    registers.write_8(r8, sum);

    // 4 T Cycles
    return 4;
}

UINT CPU::inc_mHL() {
    // Z0H-

    WORD hl = registers.read_16(HL);

    BYTE mHL = mmu.read(hl);

    BYTE sum = _add(
        mHL,
        1
    );

    mmu.write(hl, sum);

    // 12 T Cycles
    return 12;
}

UINT CPU::or_A_r8() {
    // Z000

    UINT r8; // FIXME get r8 from opcode

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

    BYTE n8; // FIXME get n8 from opcode

    BYTE o = _or(
        registers.read_8(A),
        mmu.read(n8)
    );

    registers.write_8(A, o);

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
    
    UINT r8; // FIXME r8

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

    BYTE n8; // FIXME read n8 from opcode

    BYTE x = _xor(
        registers.read_8(A),
        mmu.read(n8)
    );

    registers.write_8(A, x);
    
    // 8 T Cycles
    return 8;
}

// 16 bit Arithmetic Instructions
UINT CPU::add_HL_r16() {
    // -0HC

    UINT r16; // FIXME get r16 from opcode

    WORD sum = _add(
        registers.read_16(HL),
        registers.read_16(r16)
    );

    registers.write_16(HL, sum);

    // 8 T Cycles
    return 8;
}

UINT CPU::dec_r16() {
    
    UINT r16; // FIXME get r16 from opcode

    WORD diff = _sub(
        registers.read_16(r16),
        1
    );

    registers.write_16(r16, diff);

    // 8 T Cycles
    return 8;
}

UINT CPU::inc_r16() {

    UINT r16; // FIXME get r16 from opcode

    WORD sum = _add(
        registers.read_16(r16),
        1
    );

    registers.write_16(r16, sum);

    // 8 T Cycles
    return 8;
}