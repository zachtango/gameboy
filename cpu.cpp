#include <iostream>

typedef unsigned char BYTE; // 1 byte
typedef char SIGNED_BYTE; // 1 byte
typedef unsigned short WORD; // 2 bytes
typedef signed short SIGNED_WORD; // 2 bytes

#define A 0
#define F 1
#define B 2
#define C 3
#define D 4
#define E 5
#define H 6
#define L 7

class CPU {
public:
    void run_fde() {
        
        // fetch
        opcode = fetch();
        
        // decode
        

        // execute
        // will be one instruction
    
    }

private:
    WORD SP;
    WORD PC;
    BYTE opcode;

    /*
        Registers A, B, C, D, E, H, L
    */
    WORD R[7];

    // Fetch helper
    WORD fetch() {return 0; /* FIXME */ }

    /*
        Execute helpers
        reference: https://rgbds.gbdev.io/docs/v0.6.1/gbz80.7/
    */
    void set_r8(BYTE r8, BYTE b);
    void set_r16(BYTE r16, WORD w);
    void carry_helper();

    // 8 bit Arithmetic and Logic Instructions
    
    // void adc_A_r8();
    // void adc_A_mHL();
    // void adc_A_n8();

    void add_A_r8(bool carry);
    void add_A_mHL(bool carry);
    void add_A_n8(bool carry);

    void and_A_r8();
    void and_A_mHL();
    void and_A_n8();

    void cp_A_r8();
    void cp_A_mHL();
    void cp_A_n8();
    
    void dec_r8();
    void dec_mHL();
    void inc_r8();
    void inc_mHL();
    
    void or_A_r8();
    void or_A_mHL();
    void or_A_n8();
    
    void sub_A_r8(bool carry);
    void sub_A_mHL(bool carry);
    void sub_A_n8(bool carry);
    
    void xor_A_r8();
    void xor_A_mHL();
    void xor_A_n8();

    // 16 bit Arithmetic Instructions
    void add_HL_r16();
    void dec_r16();
    void inc_r16();

    // Bit Operations Instructions
    void bit_u3_r8();
    void bit_u3_mHL();

    // Bit Shift Instructions

    // Load Instructions

    // Jump and Subroutines

    // Stack Operations Instructions

    // Miscellaneous Instructions

};

/*
    List of abbreviations used in this document.

    r8
    Any of the 8-bit registers (A, B, C, D, E, H, L).
    r16
    Any of the general-purpose 16-bit registers (BC, DE, HL).
    n8
    8-bit integer constant.
    n16
    16-bit integer constant.
    e8
    8-bit offset (-128 to 127).
    u3
    3-bit unsigned integer constant (0 to 7).
    cc
    Condition codes:
    Z
    Execute if Z is set.
    NZ
    Execute if Z is not set.
    C
    Execute if C is set.
    NC
    Execute if C is not set.
    ! cc
    Negates a condition code.
    vec
    One of the RST vectors (0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, and 0x38).
*/

// Instructions

void _add(BYTE a, BYTE b, bool carry) {

}

void _add(WORD a, WORD b) {

}

void _and(BYTE a, BYTE b) {

}

void _sub(BYTE a, BYTE b, bool carry) {

}

void _sub(WORD a, WORD b) {

}

void _or(BYTE a, BYTE b) {

}

void _xor(BYTE a, BYTE b) {

}

bool get_carry() {

}

BYTE msb(WORD w) {
    return (w & 0xF0) >> 8;
}

BYTE lsb(WORD w) {
    return (w & 0x0F);
}

WORD concat(BYTE a, BYTE b) {
    WORD ab = (a << 8) | b;

    return ab;
}

// 8 bit Arithmetic and Logic Instructions
void CPU::add_A_r8(bool carry) {
    uint32_t cycles = 0;

    BYTE r8; // FIXME, get this from opcode
    
    _add(R[A], R[r8], carry); // FIXME set R[A] to this

}

void CPU::add_A_mHL(bool carry) {
    uint32_t cycles = 0;

    BYTE mHL; // FIXME, get from HL register
    
    _add(R[A], mHL, carry); // FIXME set R[A] to this
    
}

void CPU::add_A_n8(bool carry) {
    uint32_t cycles = 0;

    BYTE n8; // FIXME, get n8 from memory
    
    _add(R[A], n8, carry); // FIXME set R[A] to this
    
}

void CPU::and_A_r8() {
    uint32_t cycles = 0;

    BYTE n8; // FIXME, get n8 from memory
    
    _and(R[A], n8); // FIXME set R[A] to this
}

void CPU::and_A_mHL() { 
    uint32_t cycles = 0;

    BYTE n8; // FIXME, get n8 from memory
    
    _and(R[A], n8);
}

void CPU::and_A_n8() {
    uint32_t cycles = 0;

    BYTE n8; // FIXME, get n8 from memory
    
    _and(R[A], n8);
}

void CPU::cp_A_r8() {
    uint32_t cycles = 0;

    BYTE r8; // FIXME, get r8 from opcode
    
    _sub(R[A], R[r8], false);
}

void CPU::cp_A_mHL() {
    uint32_t cycles = 0;

    BYTE mHL; // FIXME, get mHL
    
    _sub(R[A], mHL, false);
}

void CPU::cp_A_n8() {
    uint32_t cycles = 0;

    BYTE n8; // FIXME, get n8 from memory and memory
    
    _sub(R[A], n8, false);
}

void CPU::dec_r8() {
    uint32_t cycles = 0;

    BYTE r8; // FIXME, get r8 from opcode and memory
    
    _sub(R[r8], 1, false);    
}

void CPU::dec_mHL() {
    uint32_t cycles = 0;

    BYTE mHL; // FIXME, get mHL
    
    _sub(mHL, 1, false);

    // FIXME set mHl to decr   
}

void CPU::inc_r8() {
    uint32_t cycles = 0;

    BYTE r8; // FIXME, get r8
    
    _add(R[r8], 1, false);
}

void CPU::inc_mHL() {
    uint32_t cycles = 0;

    BYTE mHL; // FIXME, get mHL
    
    _add(mHL, 1, false);

    // FIXME set mHl to inc 
}


void CPU::or_A_r8() {
    uint32_t cycles = 0;

    BYTE r8; // FIXME, get r8
    
    _or(R[A], R[r8]);
}

void CPU::or_A_mHL() {
    uint32_t cycles = 0;

    BYTE mHL; // FIXME, get mHL
    
    _or(R[A], mHL);
}

void CPU::or_A_n8() {
    uint32_t cycles = 0;

    BYTE n8; // FIXME, get n8 memory
    
    _or(R[A], n8);
}

void CPU::sub_A_r8(bool carry) {
    uint32_t cycles = 0;

    BYTE r8; // FIXME, get this from opcode
    
    _sub(R[A], R[r8], carry); // FIXME set R[A] to this

}

void CPU::sub_A_mHL(bool carry) {
    uint32_t cycles = 0;

    BYTE mHL; // FIXME, get from HL register
    
    _sub(R[A], mHL, carry); // FIXME set R[A] to this
    
}

void CPU::sub_A_n8(bool carry) {
    uint32_t cycles = 0;

    BYTE n8; // FIXME, get n8 from memory
    
    _sub(R[A], n8, carry); // FIXME set R[A] to this
    
}

void CPU::xor_A_r8() {
    uint32_t cycles = 0;

    BYTE r8; // FIXME, get r8 opcode
    
    _xor(R[A], R[r8]);
}

void CPU::xor_A_mHL() {
    uint32_t cycles = 0;

    BYTE mHL; // FIXME, get mHL memory
    
    _xor(R[A], mHL);
}

void CPU::xor_A_n8() {
    uint32_t cycles = 0;

    BYTE n8; // FIXME, get n8 from memory
    
    _xor(R[A], n8);
}

// 16 bit Arithmetic Instructions
void CPU::add_HL_r16() {
    uint32_t CYCLES = 0;

    WORD HL = concat(R[H], R[L]);
    WORD r16; // FIXME, get r16 from opcode

    _add(HL, r16);
}

void CPU::dec_r16() {
    uint32_t CYCLES = 0;

    WORD r16; // FIXME, get r16 from opcode

    BYTE r1 = msb(r16),
         r2 = lsb(r16);

    WORD mr16 = concat(R[r1], R[r2]);
    
    _sub(mr16, 1);

    set_r16(r16, mr16);
}

void CPU::inc_r16() {
    uint32_t CYCLES = 0;

    WORD r16; // FIXME, get r16 from opcode

    BYTE r1 = msb(r16),
         r2 = lsb(r16);

    WORD mr16 = concat(R[r1], R[r2]);
    
    _add(mr16, 1);

    set_r16(r16, mr16);
}