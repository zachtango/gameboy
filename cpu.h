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
    UINT run_fde();

    // CPU states
    bool halt_mode;
    bool sleep_mode;
    bool IME;

// private:
    class Registers {
    public:
        BYTE read_8(UINT reg) {
            return R[reg];
        }

        void write_8(UINT reg, BYTE b) {
            R[reg] = b;
        }

        WORD read_16(UINT reg) {
            return concat(R[reg], R[reg + 1]);
        }

        void write_16(UINT reg, WORD w) {
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
        
    private:
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

    // register encodings
    UINT _register_8(UINT r) {
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

    UINT _register_16(UINT r) {
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

    WORD SP;
    WORD PC;
    BYTE opcode;
    Registers registers;
    MMU &mmu;

    /*
        Instruction Reference: 
            https://rgbds.gbdev.io/docs/v0.6.1/gbz80.7/
    */

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

    UINT adc_A_r8();
    UINT adc_A_mHL();
    UINT adc_A_n8();

    UINT add_A_r8();
    UINT add_A_mHL();
    UINT add_A_n8();

    UINT and_A_r8();
    UINT and_A_mHL();
    UINT and_A_n8();

    UINT cp_A_r8();
    UINT cp_A_mHL();
    UINT cp_A_n8();
    
    UINT dec_r8();
    UINT dec_mHL();
    UINT inc_r8();
    UINT inc_mHL();
    
    UINT or_A_r8();
    UINT or_A_mHL();
    UINT or_A_n8();
    
    UINT sbc_A_r8();
    UINT sbc_A_mHL();
    UINT sbc_A_n8();

    UINT sub_A_r8();
    UINT sub_A_mHL();
    UINT sub_A_n8();
    
    UINT xor_A_r8();
    UINT xor_A_mHL();
    UINT xor_A_n8();

    // 16 bit Arithmetic Instructions
    WORD _add_16(WORD, WORD);

    UINT add_HL_r16();
    UINT dec_r16();
    UINT inc_r16();

    // Bit Operations Instructions
    void _bit(BYTE, UINT);
    BYTE _res(BYTE, UINT);
    BYTE _set(BYTE, UINT);
    BYTE _swap(BYTE);

    UINT bit_u3_r8();
    UINT bit_u3_mHL();

    UINT res_u3_r8();
    UINT res_u3_mHL();
    
    UINT set_u3_r8();
    UINT set_u3_mHL();

    UINT swap_r8();
    UINT swap_mHL();

    // Bit Shift Instructions
    BYTE _rl(BYTE, bool);
    BYTE _rr(BYTE, bool);
    BYTE _sl(BYTE);
    BYTE _sr(BYTE, bool);

    UINT rl_r8();
    UINT rl_mHL();
    UINT rla();
    UINT rlc_r8();
    UINT rlc_mHL();
    UINT rlca();

    UINT rr_r8();
    UINT rr_mHL();
    UINT rra();
    UINT rrc_r8();
    UINT rrc_mHL();
    UINT rrca();

    UINT sla_r8();
    UINT sla_mHL();

    UINT sra_r8();
    UINT sra_mHL();

    UINT srl_r8();
    UINT srl_mHL();

    // Load Instructions
    UINT ld_r8_r8();
    UINT ld_r8_n8();
    UINT ld_r16_n16();
    UINT ld_mHL_r8();
    UINT ld_mHL_n8();
    UINT ld_r8_mHL();
    UINT ld_mr16_A();
    UINT ld_mn16_A();
    UINT ldh_mn16_A();
    UINT ldh_mC_A();
    UINT ld_A_mr16();
    UINT ld_A_mn16();
    UINT ldh_A_mn16();
    UINT ldh_A_mC();
    UINT ld_mHLI_A();
    UINT ld_mHLD_A();
    UINT ld_A_mHLI();
    UINT ld_A_mHLD();

    // Jump and Subroutines
    bool _condition(UINT);
    void _call(WORD);

    UINT call_n16();
    UINT call_cc_n16();
    UINT jp_HL();
    UINT jp_n16();
    UINT jp_cc_n16();
    UINT jr_n16();
    UINT jr_cc_n16();
    UINT ret_cc();
    UINT ret();
    UINT reti();
    UINT rst();

    // Stack Operations Instructions
    UINT add_HL_sp();
    UINT add_SP_e8();
    UINT dec_SP();
    UINT inc_SP();
    UINT ld_SP_n16();
    UINT ld_mn16_SP();
    UINT ld_HL_SPe8();
    UINT ld_SP_HL();
    UINT pop_AF();
    UINT pop_r16();
    UINT push_AF();
    UINT push_r16();

    // Miscellaneous Instructions
    UINT ccf();
    UINT cpl();
    UINT daa();
    UINT di();
    UINT ei();
    UINT halt();
    UINT nop();
    UINT scf();
    UINT stop();
};