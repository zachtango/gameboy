#ifndef CPU_H
#define CPU_H

#include <unordered_map>
#include <fstream>
#include <string>
#include "constants.h"
#include "helpers.h"
#include "mmu.h"

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


class CPU {
public:
    CPU(MMU &mmu) : mmu(mmu) {
        // Map opcode to correct function pointer
        init_instr_tables();

        halt_mode = false;
        prefix = false;

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
    }

    MMU mmu;

    // CPU halt flag
    bool halt_mode;

    /* MASTER SEQUENCE (returns number of cycles taken) */
    U32 fetch_decode_execute();
    
    /* DEBUG FUNCTIONS */
    void print_registers();

private:
    /* INSTRUCTION OPCODE MAPPINGS */
    typedef U32 (CPU::*cpu_instr)();

    // opcode to instruction function pointer map
    std::unordered_map<BYTE, cpu_instr> instr;
    
    // opcode to cb prefixed instruction function pointer map
    std::unordered_map<BYTE, cpu_instr> cb_instr;

    // Map opcode to instruction functions for instr and cb_instr table
    void init_instr_tables();

    // Lets next FDE know that instruction is CB PREFIXED
    bool prefix;

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
                throw "Unknown register 16";
        }
    }

    /*
        CPU INSTRUCTIONS
        https://rgbds.gbdev.io/docs/v0.6.1/gbz80.7/
    */
    // Prefix
    U32 cb_prefix();

    // 8 bit Arithmetic and Logic Instructions
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

    // 16 bit Arithmetic Instructions
    WORD _add_16(WORD, WORD);

    U32 add_HL_r16();
    U32 dec_r16();
    U32 inc_r16();

    // Bit Operations Instructions
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

    // Bit Shift Instructions
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

    // Load Instructions
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

    // Stack Operations Instructions
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

    // Miscellaneous Instructions
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

#endif