#ifndef CPU_H
#define CPU_H

#include <fstream>
#include <string>
#include <fstream>
#include "constants.h"
#include "helpers.h"

#define A 0
#define F 1
#define B 2
#define C 3
#define D 4
#define E 5
#define H 6
#define L 7

#define AF 0
#define BC 2
#define DE 4
#define HL 6

class MMU;

class CPU {
public:
    CPU(MMU &mmu, bool debug = false) : mmu(mmu) {
        this->debug = debug;
        
        // Map opcode to correct function pointer
        init_instruction_tables();

        halt_mode = false;
        prefix_instruction = false;

        // https://gbdev.io/pandocs/Power_Up_Sequence.html#cpu-registers
        // CPU State after BOOT ROM transfers to CART ROM
        registers.write_8(A, 0x01);
        registers.write_8(F, 0xB0);
        registers.write_8(B, 0x00);
        registers.write_8(C, 0x13);
        registers.write_8(D, 0x00);
        registers.write_8(E, 0xD8);
        registers.write_8(H, 0x01);
        registers.write_8(L, 0x4D);
        
        SP = 0xFFFE;
        PC = 0x0100;
        
        interrupt_master_enable = false;
        enable_interrupt = false;

        if(debug)
            log_file.open("log.txt");

        std::cout << "CPU INITIALIZED\n";
    }

    std::ofstream log_file;

    // CPU halt flag
    bool halt_mode;

    /* MASTER SEQUENCE (returns number of cycles taken) */
    U32 fetch_decode_execute();
    
    /* DEBUG FUNCTIONS */
    void print_registers();

    // Interrupts tightly coupled with CPU
    friend class Interrupts;
private:
    bool debug;

    // For reading and writing to memory
    MMU &mmu;

    // 0 Disable all interupts
    // 1 Enable all interrupts enabled in IE register 0xFFFF
    bool interrupt_master_enable;
    
    // for buffering EI instruction
    bool enable_interrupt;

    /* INSTRUCTION OPCODE MAPPINGS */
    typedef U32 (CPU::*cpu_instruction)();

    // opcode to instruction function pointer map
    cpu_instruction instruction_map[0x100];
    
    // opcode to cb prefixed instruction function pointer map
    cpu_instruction cb_instruction_map[0x100];

    // Map opcode to instruction functions for instruction and cb_instruction table
    void init_instruction_tables();

    // Lets next FDE know that instruction is CB PREFIXED
    bool prefix_instruction;

    // Current instruction opcode
    BYTE opcode;

    /* INTERNAL REGISTERS */
    WORD SP; // stack pointer
    WORD PC; // PC counter pointing to memory address of current instruction

    /* AF BC DE HL REGISTER CLASS */
    class Registers {
    public:
        BYTE read_8(U32 reg) {
            return R[reg];
        }

        void write_8(U32 reg, BYTE b) {
            R[reg] = b;
        }

        WORD read_16(U32 reg) {
            return concat(R[reg], R[reg + 1]);
        }

        void write_16(U32 reg, WORD w) {
            R[reg] = msb(w);
            R[reg + 1] = lsb(w);
        }

        void set_z_flag(bool on) {
            R[F] = set_bit(R[F], 7, on);
        }

        void set_n_flag(bool on) {
            R[F] = set_bit(R[F], 6, on);
        }

        void set_h_flag(bool on) {
            R[F] = set_bit(R[F], 5, on);
        }

        void set_c_flag(bool on) {
            R[F] = set_bit(R[F], 4, on);
        }

        bool get_z_flag() {
            return get_bit(R[F], 7);
        }

        bool get_n_flag() {
            return get_bit(R[F], 6);
        }

        bool get_h_flag() {
            return get_bit(R[F], 5);
        }

        bool get_c_flag() {
            return get_bit(R[F], 4);
        }
        
        /*
            Registers 
                A = 0, 
                B = 1,
                C = 2,
                D = 3,
                E = 4,
                H = 5,
                L = 6,
                F = 7
        */
        BYTE R[7];
    };

    Registers registers; // registers AF, BC, DE, HL


    /* REGISTER ENCODINGS FOR INSTRUCTION SET */
    U32 _register_8(U32 r) {
        switch(r) {
            case 0:
                return B;
            case 1:
                return C;
            case 2:
                return D;
            case 3:
                return E;
            case 4:
                return H;
            case 5:
                return L;
            case 7:
                return A;
            default:
                std::cerr << "Unknown register 8\n";
                throw "Unknown register 8";
        }
    }

    U32 _register_16(U32 r) {
        switch(r) {
            case 0:
                return BC;
            case 1:
                return DE;
            case 2:
                return HL;
            case 3:
                return AF;
            default:
                std::cerr << "Unknown register 16\n";
                throw "Unknown register 16";
        }
    }

    /*
        CPU INSTRUCTIONS
        https://rgbds.gbdev.io/docs/v0.6.1/gbz80.7/
    */
    // Prefix
    U32 cb_prefix();

    // 8 bit Arithmetic and Logic instructions
    BYTE _add_8(BYTE, BYTE, bool);
    BYTE _sub_8(BYTE, BYTE, bool);
    BYTE _and(BYTE, BYTE);
    BYTE _or(BYTE, BYTE);
    BYTE _xor(BYTE, BYTE);
    BYTE _dec(BYTE);
    BYTE _inc(BYTE);

    void _add_A_r8(bool);
    void _add_A_mHL(bool);
    void _add_A_n8(bool);

    void _sub_A_r8(bool);
    void _sub_A_mHL(bool);
    void _sub_A_n8(bool);

    U32 adc_A_r8();
    U32 adc_A_mHL();
    U32 adc_A_n8();

    U32 add_A_r8();
    U32 add_A_mHL();
    U32 add_A_n8();

    U32 and_A_r8();
    U32 and_A_mHL();
    U32 and_A_n8();

    U32 cp_A_r8();
    U32 cp_A_mHL();
    U32 cp_A_n8();
    
    U32 dec_r8();
    U32 dec_mHL();
    U32 inc_r8();
    U32 inc_mHL();
    
    U32 or_A_r8();
    U32 or_A_mHL();
    U32 or_A_n8();
    
    U32 sbc_A_r8();
    U32 sbc_A_mHL();
    U32 sbc_A_n8();

    U32 sub_A_r8();
    U32 sub_A_mHL();
    U32 sub_A_n8();
    
    U32 xor_A_r8();
    U32 xor_A_mHL();
    U32 xor_A_n8();

    // 16 bit Arithmetic instructions
    WORD _add_16(WORD, WORD);

    U32 add_HL_r16();
    U32 dec_r16();
    U32 inc_r16();

    // Bit Operations instructions
    void _bit(BYTE, U32);
    BYTE _res(BYTE, U32);
    BYTE _set(BYTE, U32);
    BYTE _swap(BYTE);

    U32 bit_u3_r8();
    U32 bit_u3_mHL();

    U32 res_u3_r8();
    U32 res_u3_mHL();
    
    U32 set_u3_r8();
    U32 set_u3_mHL();

    U32 swap_r8();
    U32 swap_mHL();

    // Bit Shift instructions
    BYTE _rl(BYTE, bool);
    BYTE _rr(BYTE, bool);
    BYTE _sl(BYTE);
    BYTE _sr(BYTE, bool);

    U32 rl_r8();
    U32 rl_mHL();
    U32 rla();
    U32 rlc_r8();
    U32 rlc_mHL();
    U32 rlca();

    U32 rr_r8();
    U32 rr_mHL();
    U32 rra();
    U32 rrc_r8();
    U32 rrc_mHL();
    U32 rrca();

    U32 sla_r8();
    U32 sla_mHL();

    U32 sra_r8();
    U32 sra_mHL();

    U32 srl_r8();
    U32 srl_mHL();

    // Load instructions
    U32 ld_r8_r8();
    U32 ld_r8_n8();
    U32 ld_r16_n16();
    U32 ld_mHL_r8();
    U32 ld_mHL_n8();
    U32 ld_r8_mHL();
    U32 ld_mr16_A();
    U32 ld_mn16_A();
    U32 ldh_mn16_A();
    U32 ldh_mC_A();
    U32 ld_A_mr16();
    U32 ld_A_mn16();
    U32 ldh_A_mn16();
    U32 ldh_A_mC();
    U32 ld_mHLI_A();
    U32 ld_mHLD_A();
    U32 ld_A_mHLI();
    U32 ld_A_mHLD();

    // Jump and Subroutines
    bool _condition(U32);
    void _call(WORD);

    U32 call_n16();
    U32 call_cc_n16();
    U32 jp_HL();
    U32 jp_n16();
    U32 jp_cc_n16();
    U32 jr_n16();
    U32 jr_cc_n16();
    U32 ret_cc();
    U32 ret();
    U32 reti();
    U32 rst();

    // Stack Operations instructions
    U32 add_HL_SP();
    U32 add_SP_e8();
    U32 dec_SP();
    U32 inc_SP();
    U32 ld_SP_n16();
    U32 ld_mn16_SP();
    U32 ld_HL_SPe8();
    U32 ld_SP_HL();
    U32 pop_AF();
    U32 pop_r16();
    U32 push_AF();
    U32 push_r16();

    // Miscellaneous instructions
    U32 ccf();
    U32 cpl();
    U32 daa();
    U32 di();
    U32 ei();
    U32 halt();
    U32 nop();
    U32 scf();
    U32 stop();
};


static char* INSTRUCTION_NAME_MAP[0x100] = {
    [0x00] = "nop;",
    [0x01] = "ld_r16_n16;",
    [0x02] = "ld_mr16_A;",
    [0x03] = "inc_r16;",
    [0x04] = "inc_r8;",
    [0x05] = "dec_r8;",
    [0x06] = "ld_r8_n8;",
    [0x07] = "rlca;",
    [0x08] = "ld_mn16_SP;",
    [0x09] = "add_HL_r16;",
    [0x0A] = "ld_A_mr16;",
    [0x0B] = "dec_r16;",
    [0x0C] = "inc_r8;",
    [0x0D] = "dec_r8;",
    [0x0E] = "ld_r8_n8;",
    [0x0F] = "rrca;",

    [0x10] = "stop;",
    [0x11] = "ld_r16_n16;",
    [0x12] = "ld_mr16_A;",
    [0x13] = "inc_r16;",
    [0x14] = "inc_r8;",
    [0x15] = "dec_r8;",
    [0x16] = "ld_r8_n8;",
    [0x17] = "rla;",
    [0x18] = "jr_n16;",
    [0x19] = "add_HL_r16;",
    [0x1A] = "ld_A_mr16;",
    [0x1B] = "dec_r16;",
    [0x1C] = "inc_r8;",
    [0x1D] = "dec_r8;",
    [0x1E] = "ld_r8_n8;",
    [0x1F] = "rra;",

    [0x20] = "jr_cc_n16;",
    [0x21] = "ld_r16_n16;",
    [0x22] = "ld_mHLI_A;",
    [0x23] = "inc_r16;",
    [0x24] = "inc_r8;",
    [0x25] = "dec_r8;",
    [0x26] = "ld_r8_n8;",
    [0x27] = "daa;",
    [0x28] = "jr_cc_n16;",
    [0x29] = "add_HL_r16;",
    [0x2A] = "ld_A_mHLI;",
    [0x2B] = "dec_r16;",
    [0x2C] = "inc_r8;",
    [0x2D] = "dec_r8;",
    [0x2E] = "ld_r8_n8;",
    [0x2F] = "cpl;",
    
    [0x30] = "jr_cc_n16;",
    [0x31] = "ld_SP_n16;",
    [0x32] = "ld_mHLD_A;",
    [0x33] = "inc_SP;",
    [0x34] = "inc_mHL;",
    [0x35] = "dec_mHL;",
    [0x36] = "ld_mHL_n8;",
    [0x37] = "scf;",
    [0x38] = "jr_cc_n16;",
    [0x39] = "add_HL_SP;",
    [0x3A] = "ld_A_mHLD;",
    [0x3B] = "dec_SP;",
    [0x3C] = "inc_r8;",
    [0x3D] = "dec_r8;",
    [0x3E] = "ld_r8_n8;",
    [0x3F] = "ccf;",

    [0x40] = "ld_r8_r8;",
    [0x41] = "ld_r8_r8;",
    [0x42] = "ld_r8_r8;",
    [0x43] = "ld_r8_r8;",
    [0x44] = "ld_r8_r8;",
    [0x45] = "ld_r8_r8;",
    [0x46] = "ld_r8_mHL;",
    [0x47] = "ld_r8_r8;",
    [0x48] = "ld_r8_r8;",
    [0x49] = "ld_r8_r8;",
    [0x4A] = "ld_r8_r8;",
    [0x4B] = "ld_r8_r8;",
    [0x4C] = "ld_r8_r8;",
    [0x4D] = "ld_r8_r8;",
    [0x4E] = "ld_r8_mHL;",
    [0x4F] = "ld_r8_r8;",

    [0x50] = "ld_r8_r8;",
    [0x51] = "ld_r8_r8;",
    [0x52] = "ld_r8_r8;",
    [0x53] = "ld_r8_r8;",
    [0x54] = "ld_r8_r8;",
    [0x55] = "ld_r8_r8;",
    [0x56] = "ld_r8_mHL;",
    [0x57] = "ld_r8_r8;",
    [0x58] = "ld_r8_r8;",
    [0x59] = "ld_r8_r8;",
    [0x5A] = "ld_r8_r8;",
    [0x5B] = "ld_r8_r8;",
    [0x5C] = "ld_r8_r8;",
    [0x5D] = "ld_r8_r8;",
    [0x5E] = "ld_r8_mHL;",
    [0x5F] = "ld_r8_r8;",

    [0x60] = "ld_r8_r8;",
    [0x61] = "ld_r8_r8;",
    [0x62] = "ld_r8_r8;",
    [0x63] = "ld_r8_r8;",
    [0x64] = "ld_r8_r8;",
    [0x65] = "ld_r8_r8;",
    [0x66] = "ld_r8_mHL;",
    [0x67] = "ld_r8_r8;",
    [0x68] = "ld_r8_r8;",
    [0x69] = "ld_r8_r8;",
    [0x6A] = "ld_r8_r8;",
    [0x6B] = "ld_r8_r8;",
    [0x6C] = "ld_r8_r8;",
    [0x6D] = "ld_r8_r8;",
    [0x6E] = "ld_r8_mHL;",
    [0x6F] = "ld_r8_r8;",

    [0x70] = "ld_mHL_r8;",
    [0x71] = "ld_mHL_r8;",
    [0x72] = "ld_mHL_r8;",
    [0x73] = "ld_mHL_r8;",
    [0x74] = "ld_mHL_r8;",
    [0x75] = "ld_mHL_r8;",
    [0x76] = "halt;",
    [0x77] = "ld_mHL_r8;",
    [0x78] = "ld_r8_r8;",
    [0x79] = "ld_r8_r8;",
    [0x7A] = "ld_r8_r8;",
    [0x7B] = "ld_r8_r8;",
    [0x7C] = "ld_r8_r8;",
    [0x7D] = "ld_r8_r8;",
    [0x7E] = "ld_r8_mHL;",
    [0x7F] = "ld_r8_r8;",

    [0x80] = "add_A_r8;",
    [0x81] = "add_A_r8;",
    [0x82] = "add_A_r8;",
    [0x83] = "add_A_r8;",
    [0x84] = "add_A_r8;",
    [0x85] = "add_A_r8;",
    [0x86] = "add_A_mHL;",
    [0x87] = "add_A_r8;",
    [0x88] = "adc_A_r8;",
    [0x89] = "adc_A_r8;",
    [0x8A] = "adc_A_r8;",
    [0x8B] = "adc_A_r8;",
    [0x8C] = "adc_A_r8;",
    [0x8D] = "adc_A_r8;",
    [0x8E] = "adc_A_mHL;",
    [0x8F] = "adc_A_r8;",

    [0x90] = "sub_A_r8;",
    [0x91] = "sub_A_r8;",
    [0x92] = "sub_A_r8;",
    [0x93] = "sub_A_r8;",
    [0x94] = "sub_A_r8;",
    [0x95] = "sub_A_r8;",
    [0x96] = "sub_A_mHL;",
    [0x97] = "sub_A_r8;",
    [0x98] = "sbc_A_r8;",
    [0x99] = "sbc_A_r8;",
    [0x9A] = "sbc_A_r8;",
    [0x9B] = "sbc_A_r8;",
    [0x9C] = "sbc_A_r8;",
    [0x9D] = "sbc_A_r8;",
    [0x9E] = "sbc_A_mHL;",
    [0x9F] = "sbc_A_r8;",

    [0xA0] = "and_A_r8;",
    [0xA1] = "and_A_r8;",
    [0xA2] = "and_A_r8;",
    [0xA3] = "and_A_r8;",
    [0xA4] = "and_A_r8;",
    [0xA5] = "and_A_r8;",
    [0xA6] = "and_A_mHL;",
    [0xA7] = "and_A_r8;",
    [0xA8] = "xor_A_r8;",
    [0xA9] = "xor_A_r8;",
    [0xAA] = "xor_A_r8;",
    [0xAB] = "xor_A_r8;",
    [0xAC] = "xor_A_r8;",
    [0xAD] = "xor_A_r8;",
    [0xAE] = "xor_A_mHL;",
    [0xAF] = "xor_A_r8;",

    [0xB0] = "or_A_r8;",
    [0xB1] = "or_A_r8;",
    [0xB2] = "or_A_r8;",
    [0xB3] = "or_A_r8;",
    [0xB4] = "or_A_r8;",
    [0xB5] = "or_A_r8;",
    [0xB6] = "or_A_mHL;",
    [0xB7] = "or_A_r8;",
    [0xB8] = "cp_A_r8;",
    [0xB9] = "cp_A_r8;",
    [0xBA] = "cp_A_r8;",
    [0xBB] = "cp_A_r8;",
    [0xBC] = "cp_A_r8;",
    [0xBD] = "cp_A_r8;",
    [0xBE] = "cp_A_mHL;",
    [0xBF] = "cp_A_r8;",

    [0xC0] = "ret_cc;",
    [0xC1] = "pop_r16;",
    [0xC2] = "jp_cc_n16;",
    [0xC3] = "jp_n16;",
    [0xC4] = "call_cc_n16;",
    [0xC5] = "push_r16;",
    [0xC6] = "add_A_n8;",
    [0xC7] = "rst;",
    [0xC8] = "ret_cc;",
    [0xC9] = "ret;",
    [0xCA] = "jp_cc_n16;",
    [0xCB] = "cb_prefix;",
    [0xCC] = "call_cc_n16;",
    [0xCD] = "call_n16;",
    [0xCE] = "adc_A_n8;",
    [0xCF] = "rst;",

    [0xD0] = "ret_cc;",
    [0xD1] = "pop_r16;",
    [0xD2] = "jp_cc_n16;",
    [0xD3] = "ILLEGAL",
    [0xD4] = "call_cc_n16;",
    [0xD5] = "push_r16;",
    [0xD6] = "sub_A_n8;",
    [0xD7] = "rst;",
    [0xD8] = "ret_cc;",
    [0xD9] = "reti;",
    [0xDA] = "jp_cc_n16;",
    [0xDB] = "ILLEGAL",
    [0xDC] = "call_cc_n16;",
    [0xDD] = "ILLEGAL",
    [0xDE] = "sbc_A_n8;",
    [0xDF] = "rst;",

    [0xE0] = "ldh_mn16_A;",
    [0xE1] = "pop_r16;",
    [0xE2] = "ldh_mC_A;",
    [0xE3] = "ILLEGAL",
    [0xE4] = "ILLEGAL",
    [0xE5] = "push_r16;",
    [0xE6] = "and_A_n8;",
    [0xE7] = "rst;",
    [0xE8] = "add_SP_e8;",
    [0xE9] = "jp_HL;",
    [0xEA] = "ld_mn16_A;",
    [0xEB] = "ILLEGAL",
    [0xEC] = "ILLEGAL",
    [0xED] = "ILLEGAL",
    [0xEE] = "xor_A_n8;",
    [0xEF] = "rst;",

    [0xF0] = "ldh_A_mn16;",
    [0xF1] = "pop_AF;",
    [0xF2] = "ldh_A_mC;",
    [0xF3] = "di;",
    [0xF4] = "ILLEGAL",
    [0xF5] = "push_r16;",
    [0xF6] = "or_A_n8;",
    [0xF7] = "rst;",
    [0xF8] = "ld_HL_SPe8;",
    [0xF9] = "ld_SP_HL;",
    [0xFA] = "ld_A_mn16;",
    [0xFB] = "ei;",
    [0xFC] = "ILLEGAL",
    [0xFD] = "ILLEGAL",
    [0xFE] = "cp_A_n8;",
    [0xFF] = "rst;",
};

static char* CB_INSTRUCTION_NAME_MAP[0x100] = {
    [0x00] = "rlc_r8",
    [0x01] = "rlc_r8",
    [0x02] = "rlc_r8",
    [0x03] = "rlc_r8",
    [0x04] = "rlc_r8",
    [0x05] = "rlc_r8",
    [0x06] = "rlc_mHL",
    [0x07] = "rlc_r8",
    [0x08] = "rrc_r8",
    [0x09] = "rrc_r8",
    [0x0A] = "rrc_r8",
    [0x0B] = "rrc_r8",
    [0x0C] = "rrc_r8",
    [0x0D] = "rrc_r8",
    [0x0E] = "rrc_mHL",
    [0x0F] = "rrc_r8",

    [0x10] = "rl_r8",
    [0x11] = "rl_r8",
    [0x12] = "rl_r8",
    [0x13] = "rl_r8",
    [0x14] = "rl_r8",
    [0x15] = "rl_r8",
    [0x16] = "rl_mHL",
    [0x17] = "rl_r8",
    [0x18] = "rr_r8",
    [0x19] = "rr_r8",
    [0x1A] = "rr_r8",
    [0x1B] = "rr_r8",
    [0x1C] = "rr_r8",
    [0x1D] = "rr_r8",
    [0x1E] = "rr_mHL",
    [0x1F] = "rr_r8",

    [0x20] = "sla_r8",
    [0x21] = "sla_r8",
    [0x22] = "sla_r8",
    [0x23] = "sla_r8",
    [0x24] = "sla_r8",
    [0x25] = "sla_r8",
    [0x26] = "sla_mHL",
    [0x27] = "sla_r8",
    [0x28] = "sra_r8",
    [0x29] = "sra_r8",
    [0x2A] = "sra_r8",
    [0x2B] = "sra_r8",
    [0x2C] = "sra_r8",
    [0x2D] = "sra_r8",
    [0x2E] = "sra_mHL",
    [0x2F] = "sra_r8",

    [0x30] = "swap_r8",
    [0x31] = "swap_r8",
    [0x32] = "swap_r8",
    [0x33] = "swap_r8",
    [0x34] = "swap_r8",
    [0x35] = "swap_r8",
    [0x36] = "swap_mHL",
    [0x37] = "swap_r8",
    [0x38] = "srl_r8",
    [0x39] = "srl_r8",
    [0x3A] = "srl_r8",
    [0x3B] = "srl_r8",
    [0x3C] = "srl_r8",
    [0x3D] = "srl_r8",
    [0x3E] = "srl_mHL",
    [0x3F] = "srl_r8",

    [0x40] = "bit_u3_r8",
    [0x41] = "bit_u3_r8",
    [0x42] = "bit_u3_r8",
    [0x43] = "bit_u3_r8",
    [0x44] = "bit_u3_r8",
    [0x45] = "bit_u3_r8",
    [0x46] = "bit_u3_mHL",
    [0x47] = "bit_u3_r8",
    [0x48] = "bit_u3_r8",
    [0x49] = "bit_u3_r8",
    [0x4A] = "bit_u3_r8",
    [0x4B] = "bit_u3_r8",
    [0x4C] = "bit_u3_r8",
    [0x4D] = "bit_u3_r8",
    [0x4E] = "bit_u3_mHL",
    [0x4F] = "bit_u3_r8",

    [0x50] = "bit_u3_r8",
    [0x51] = "bit_u3_r8",
    [0x52] = "bit_u3_r8",
    [0x53] = "bit_u3_r8",
    [0x54] = "bit_u3_r8",
    [0x55] = "bit_u3_r8",
    [0x56] = "bit_u3_mHL",
    [0x57] = "bit_u3_r8",
    [0x58] = "bit_u3_r8",
    [0x59] = "bit_u3_r8",
    [0x5A] = "bit_u3_r8",
    [0x5B] = "bit_u3_r8",
    [0x5C] = "bit_u3_r8",
    [0x5D] = "bit_u3_r8",
    [0x5E] = "bit_u3_mHL",
    [0x5F] = "bit_u3_r8",

    [0x60] = "bit_u3_r8",
    [0x61] = "bit_u3_r8",
    [0x62] = "bit_u3_r8",
    [0x63] = "bit_u3_r8",
    [0x64] = "bit_u3_r8",
    [0x65] = "bit_u3_r8",
    [0x66] = "bit_u3_mHL",
    [0x67] = "bit_u3_r8",
    [0x68] = "bit_u3_r8",
    [0x69] = "bit_u3_r8",
    [0x6A] = "bit_u3_r8",
    [0x6B] = "bit_u3_r8",
    [0x6C] = "bit_u3_r8",
    [0x6D] = "bit_u3_r8",
    [0x6E] = "bit_u3_mHL",
    [0x6F] = "bit_u3_r8",

    [0x70] = "bit_u3_r8",
    [0x71] = "bit_u3_r8",
    [0x72] = "bit_u3_r8",
    [0x73] = "bit_u3_r8",
    [0x74] = "bit_u3_r8",
    [0x75] = "bit_u3_r8",
    [0x76] = "bit_u3_mHL",
    [0x77] = "bit_u3_r8",
    [0x78] = "bit_u3_r8",
    [0x79] = "bit_u3_r8",
    [0x7A] = "bit_u3_r8",
    [0x7B] = "bit_u3_r8",
    [0x7C] = "bit_u3_r8",
    [0x7D] = "bit_u3_r8",
    [0x7E] = "bit_u3_mHL",
    [0x7F] = "bit_u3_r8",

    [0x80] = "res_u3_r8",
    [0x81] = "res_u3_r8",
    [0x82] = "res_u3_r8",
    [0x83] = "res_u3_r8",
    [0x84] = "res_u3_r8",
    [0x85] = "res_u3_r8",
    [0x86] = "res_u3_mHL",
    [0x87] = "res_u3_r8",
    [0x88] = "res_u3_r8",
    [0x89] = "res_u3_r8",
    [0x8A] = "res_u3_r8",
    [0x8B] = "res_u3_r8",
    [0x8C] = "res_u3_r8",
    [0x8D] = "res_u3_r8",
    [0x8E] = "res_u3_mHL",
    [0x8F] = "res_u3_r8",

    [0x90] = "res_u3_r8",
    [0x91] = "res_u3_r8",
    [0x92] = "res_u3_r8",
    [0x93] = "res_u3_r8",
    [0x94] = "res_u3_r8",
    [0x95] = "res_u3_r8",
    [0x96] = "res_u3_mHL",
    [0x97] = "res_u3_r8",
    [0x98] = "res_u3_r8",
    [0x99] = "res_u3_r8",
    [0x9A] = "res_u3_r8",
    [0x9B] = "res_u3_r8",
    [0x9C] = "res_u3_r8",
    [0x9D] = "res_u3_r8",
    [0x9E] = "res_u3_mHL",
    [0x9F] = "res_u3_r8",

    [0xA0] = "res_u3_r8",
    [0xA1] = "res_u3_r8",
    [0xA2] = "res_u3_r8",
    [0xA3] = "res_u3_r8",
    [0xA4] = "res_u3_r8",
    [0xA5] = "res_u3_r8",
    [0xA6] = "res_u3_mHL",
    [0xA7] = "res_u3_r8",
    [0xA8] = "res_u3_r8",
    [0xA9] = "res_u3_r8",
    [0xAA] = "res_u3_r8",
    [0xAB] = "res_u3_r8",
    [0xAC] = "res_u3_r8",
    [0xAD] = "res_u3_r8",
    [0xAE] = "res_u3_mHL",
    [0xAF] = "res_u3_r8",

    [0xB0] = "res_u3_r8",
    [0xB1] = "res_u3_r8",
    [0xB2] = "res_u3_r8",
    [0xB3] = "res_u3_r8",
    [0xB4] = "res_u3_r8",
    [0xB5] = "res_u3_r8",
    [0xB6] = "res_u3_mHL",
    [0xB7] = "res_u3_r8",
    [0xB8] = "res_u3_r8",
    [0xB9] = "res_u3_r8",
    [0xBA] = "res_u3_r8",
    [0xBB] = "res_u3_r8",
    [0xBC] = "res_u3_r8",
    [0xBD] = "res_u3_r8",
    [0xBE] = "res_u3_mHL",
    [0xBF] = "res_u3_r8",

    [0xC0] = "set_u3_r8",
    [0xC1] = "set_u3_r8",
    [0xC2] = "set_u3_r8",
    [0xC3] = "set_u3_r8",
    [0xC4] = "set_u3_r8",
    [0xC5] = "set_u3_r8",
    [0xC6] = "set_u3_mHL",
    [0xC7] = "set_u3_r8",
    [0xC8] = "set_u3_r8",
    [0xC9] = "set_u3_r8",
    [0xCA] = "set_u3_r8",
    [0xCB] = "set_u3_r8",
    [0xCC] = "set_u3_r8",
    [0xCD] = "set_u3_r8",
    [0xCE] = "set_u3_mHL",
    [0xCF] = "set_u3_r8",

    [0xD0] = "set_u3_r8",
    [0xD1] = "set_u3_r8",
    [0xD2] = "set_u3_r8",
    [0xD3] = "set_u3_r8",
    [0xD4] = "set_u3_r8",
    [0xD5] = "set_u3_r8",
    [0xD6] = "set_u3_mHL",
    [0xD7] = "set_u3_r8",
    [0xD8] = "set_u3_r8",
    [0xD9] = "set_u3_r8",
    [0xDA] = "set_u3_r8",
    [0xDB] = "set_u3_r8",
    [0xDC] = "set_u3_r8",
    [0xDD] = "set_u3_r8",
    [0xDE] = "set_u3_mHL",
    [0xDF] = "set_u3_r8",

    [0xE0] = "set_u3_r8",
    [0xE1] = "set_u3_r8",
    [0xE2] = "set_u3_r8",
    [0xE3] = "set_u3_r8",
    [0xE4] = "set_u3_r8",
    [0xE5] = "set_u3_r8",
    [0xE6] = "set_u3_mHL",
    [0xE7] = "set_u3_r8",
    [0xE8] = "set_u3_r8",
    [0xE9] = "set_u3_r8",
    [0xEA] = "set_u3_r8",
    [0xEB] = "set_u3_r8",
    [0xEC] = "set_u3_r8",
    [0xED] = "set_u3_r8",
    [0xEE] = "set_u3_mHL",
    [0xEF] = "set_u3_r8",

    [0xF0] = "set_u3_r8",
    [0xF1] = "set_u3_r8",
    [0xF2] = "set_u3_r8",
    [0xF3] = "set_u3_r8",
    [0xF4] = "set_u3_r8",
    [0xF5] = "set_u3_r8",
    [0xF6] = "set_u3_mHL",
    [0xF7] = "set_u3_r8",
    [0xF8] = "set_u3_r8",
    [0xF9] = "set_u3_r8",
    [0xFA] = "set_u3_r8",
    [0xFB] = "set_u3_r8",
    [0xFC] = "set_u3_r8",
    [0xFD] = "set_u3_r8",
    [0xFE] = "set_u3_mHL",
    [0xFF] = "set_u3_r8",
};

#endif