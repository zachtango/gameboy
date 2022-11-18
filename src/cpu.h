#include "mmu.h"
#include <stdint.h>
#include <string>

typedef struct instruction instruction;
typedef struct registers registers;

// referencing SM83_decoding-1.pdf in repo
#define r8Dest (opcode >> 3u) & 0b111
#define r8Source (opcode) && 0b111
#define r16 (opcode >> 4u) && 0b11

// // flags
// #define testZ(a) (a == 0)
// #define testCAdd(a, b) ( UINT8_MAX  )
// #define testCSub(a, b) (a < b)
// #define testHSub(a, b) ( (a & 0x0F) < (b & 0x0F) )


class CPU {

private:
    struct registers {
        union {
            struct {
                uint8_t f; // lo
                uint8_t a; // hi
            };
            uint16_t af; // hi lo (stored little endian)
        };
        union {
            struct {
                uint8_t c;
                uint8_t b;
            };
            uint16_t bc;
        };
        union {
            struct {
                uint8_t e;
                uint8_t d;
            };
            uint16_t de;
        };
        union {
            struct {
                uint8_t l;
                uint8_t h;
            };
            uint16_t hl;
        };

        uint16_t sp;
        uint16_t pc;
    };

    typedef void(CPU::*fp)();
    struct instruction {
        const char *name;
        uint8_t operandLen;
        fp exec;
        // uint8_t t_cycles;
        int t_cycles;
    };

    registers regs;
    instruction instructions[256] = {
        {"NOP ----", 0, nop, 4}, // 0x00 opcode
        {"LD BC, u16 ----", 2, ld_rr_nn, 12}, // 0x01 opcode ...
        {"LD (BC), A ----", 0, ld_bc_a, 8},
        {"INC BC ----", 0, inc_rr, 8},
        {"INC B Z0H-", 0, inc_r, 4},
        {"DEC B Z1H-", 0, dec_r, 4},
        {"LD B, u8 ----", 1, ld_r_n, 8},
        {"RLCA 000C", 0, rlc_a, 4},
        {"LD (u16), SP ----", 2, ld_nn_sp, 20},
        {"ADD HL, BC -0HC", 0, add_hl_rr, 8},
        {"LD A, (BC) ----", 0, ld_a_bc, 8},
        {"DEC BC ----", 0, dec_rr, 8},
        {"INC C Z0H-", 0, inc_r, 4},
        {"DEC C Z1H-", 0, dec_r, 4},
        {"LD C, u8 ----", 1, ld_r_n, 8},
        {"RRCA 000C", 0, rrc_a, 4},
        
        {"STOP ----", 0, stop, 4},
        {"LD DE, u16 ----", 2, ld_rr_nn, 12},
        {"LD (DE), A ----", 0, ld_de_a, 8},
        {"INC DE ----", 0, inc_rr, 8},
        {"INC D Z0H-", 0, inc_r, 4},
        {"DEC D Z1H-", 0, dec_r, 4},
        {"LD D, u8 ----", 1, ld_r_n, 8},
        {"RLA 000C", 0, rl_a, 4},
        {"JR PC+dd ----", 1, jr_pcdd, 12},
        {"ADD HL, DE -0HC", 0, add_hl_rr, 8},
        {"LD A, (DE) ----", 0, ld_a_de, 8},
        {"DEC DE ----", 0, dec_rr, 8},
        {"INC E Z0H-", 0, inc_r, 4},
        {"DEC E Z1H-", 0, dec_r, 4},
        {"LD E, u8 ----", 1, ld_r_n, 8},
        {"RRA 000C", 0, rr_a, 4},

        {"JR NZ, PC+dd ----", 1, jr_f_pcdd, 8-12},
        {"LD HL, u16 ----", 2, ld_hl_n, 12},
        {"LD (HL+), A ----", 0, ldi_hl_a, 8},
        {"INC HL ----", 0, inc_rr, 8},
        {"INC H Z0H-", 0, inc_r, 4},
        {"DEC H Z1H-", 0, dec_r, 4},
        {"LD H, u8 ----", 1, ld_r_n, 8},
        {"DAA Z-0C", 0, daa, 4},
        {"JR Z, PC+dd ----", 1, jr_f_pcdd, 12},
        {"ADD HL, HL -0HC", 0, add_hl_rr, 8},
        {"LD A, (HL+) ----", 0, ldi_a_hl, 8},
        {"DEC HL ----", 0, dec_rr, 8},
        {"INC L Z0H-", 0, inc_r, 4},
        {"DEC L Z1H-", 0, dec_r, 4},
        {"LD L, u8 ----", 1, ld_r_n, 8},
        {"CPL -11-", 0, cpl, 4},

        {"JR NC, PC+dd ----", 1, jr_f_pcdd, 8-12},
        {"LD SP, u16 ----", 2, ld_nn_sp, 12},
        {"LD (HL-), A ----", 0, ldd_hl_a, 8},
        {"INC SP ----", 0, inc_rr, 8},
        {"INC (HL) Z0H-", 0, inc_r, 12},
        {"DEC (HL) Z1H-", 0, dec_r, 12},
        {"LD (HL), u8 ----", 1, ld_r_n, 12},
        {"SCF -001", 0, scf, 4},
        {"JR C, PC+dd ----", 1, jr_f_pcdd, 8-12},
        {"ADD HL, SP -0HC", 0, add_hl_rr, 8},
        {"LD A, (HL-) ----", 0, ldd_a_hl, 8},
        {"DEC SP ----", 0, dec_rr, 8},
        {"INC A Z0H-", 0, inc_r, 4},
        {"DEC A Z1H-", 0, dec_r, 4},
        {"LD A, u8 ----", 1, ld_r_n, 8},
        {"CCF -00C", 0, ccf, 4},

        {"LD B, B ----", 0, ld_r_r, 4},
        {"LD B, C ----", 0, ld_r_r, 4},
        {"LD B, D ----", 0, ld_r_r, 4},
        {"LD B, E ----", 0, ld_r_r, 4},
        {"LD B, H ----", 0, ld_r_r, 4},
        {"LD B, L ----", 0, ld_r_r, 4},
        {"LD B, (HL) ----", 0, ld_r_hl, 8},
        {"LD B, A ----", 0, ld_r_r, 4},
        {"LD C, B ----", 0, ld_r_r, 4},
        {"LD C, C ----", 0, ld_r_r, 4},
        {"LD C, D ----", 0, ld_r_r, 4},
        {"LD C, E ----", 0, ld_r_r, 4},
        {"LD C, H ----", 0, ld_r_r, 4},
        {"LD C, L ----", 0, ld_r_r, 4},
        {"LD C, (HL) ----", 0, ld_r_hl, 8},
        {"LD C, A ----", 0, ld_r_r, 4},

        {"LD D, B ----", 0, ld_r_r, 4},
        {"LD D, C ----", 0, ld_r_r, 4},
        {"LD D, D ----", 0, ld_r_r, 4},
        {"LD D, E ----", 0, ld_r_r, 4},
        {"LD D, H ----", 0, ld_r_r, 4},
        {"LD D, L ----", 0, ld_r_r, 4},
        {"LD D, (HL) ----", 0, ld_r_hl, 8},
        {"LD D, A ----", 0, ld_r_r, 4},
        {"LD E, B ----", 0, ld_r_r, 4},
        {"LD E, C ----", 0, ld_r_r, 4},
        {"LD E, D ----", 0, ld_r_r, 4},
        {"LD E, E ----", 0, ld_r_r, 4},
        {"LD E, H ----", 0, ld_r_r, 4},
        {"LD E, L ----", 0, ld_r_r, 4},
        {"LD E, (HL) ----", 0, ld_r_hl, 8},
        {"LD E, A ----", 0, ld_r_r, 4},

        {"LD H, B ----", 0, ld_r_r, 4},
        {"LD H, C ----", 0, ld_r_r, 4},
        {"LD H, D ----", 0, ld_r_r, 4},
        {"LD H, E ----", 0, ld_r_r, 4},
        {"LD H, H ----", 0, ld_r_r, 4},
        {"LD H, L ----", 0, ld_r_r, 4},
        {"LD H, (HL) ----", 0, ld_r_hl, 8},
        {"LD H, A ----", 0, ld_r_r, 4},
        {"LD L, B ----", 0, ld_r_r, 4},
        {"LD L, C ----", 0, ld_r_r, 4},
        {"LD L, D ----", 0, ld_r_r, 4},
        {"LD L, E ----", 0, ld_r_r, 4},
        {"LD L, H ----", 0, ld_r_r, 4},
        {"LD L, L ----", 0, ld_r_r, 4},
        {"LD L, (HL) ----", 0, ld_r_hl, 8},
        {"LD L, A ----", 0, ld_r_r, 4},

        {"LD (HL), B ----", 0, ld_hl_r, 8},
        {"LD (HL), C ----", 0, ld_hl_r, 8},
        {"LD (HL), D ----", 0, ld_hl_r, 8},
        {"LD (HL), E ----", 0, ld_hl_r, 8},
        {"LD (HL), H ----", 0, ld_hl_r, 8},
        {"LD (HL), L ----", 0, ld_hl_r, 8},
        {"HALT ----", 0, halt, 4},
        {"LD (HL), A ----", 0, ld_hl_r, 8},
        {"LD A, B ----", 0, ld_r_r, 4},
        {"LD A, C ----", 0, ld_r_r, 4},
        {"LD A, D ----", 0, ld_r_r, 4},
        {"LD A, E ----", 0, ld_r_r, 4},
        {"LD A, H ----", 0, ld_r_r, 4},
        {"LD A, L ----", 0, ld_r_r, 4},
        {"LD A, (HL) ----", 0, ld_r_hl, 8},
        {"LD A, A ----", 0, ld_r_r, 4},

        {"ADD A, B Z0HC", 0, add_a_r, 4},
        {"ADD A, C Z0HC", 0, add_a_r, 4},
        {"ADD A, D Z0HC", 0, add_a_r, 4},
        {"ADD A, E Z0HC", 0, add_a_r, 4},
        {"ADD A, H Z0HC", 0, add_a_r, 4},
        {"ADD A, L Z0HC", 0, add_a_r, 4},
        {"ADD A, (HL) Z0HC", 0, add_a_hl, 8},
        {"ADD A, A Z0HC", 0, add_a_r, 4},
        {"ADC A, B Z0HC", 0, adc_a_r, 4},
        {"ADC A, C Z0HC", 0, adc_a_r, 4},
        {"ADC A, D Z0HC", 0, adc_a_r, 4},
        {"ADC A, E Z0HC", 0, adc_a_r, 4},
        {"ADC A, H Z0HC", 0, adc_a_r, 4},
        {"ADC A, L Z0HC", 0, adc_a_r, 4},
        {"ADC A, (HL) Z0HC", 0, adc_a_hl, 8},
        {"ADC A, A Z0HC", 0, adc_a_r, 4},

        {"SUB A, B Z1HC", 0, sub_a_r, 4},
        {"SUB A, C Z1HC", 0, sub_a_r, 4},
        {"SUB A, D Z1HC", 0, sub_a_r, 4},
        {"SUB A, E Z1HC", 0, sub_a_r, 4},
        {"SUB A, H Z1HC", 0, sub_a_r, 4},
        {"SUB A, L Z1HC", 0, sub_a_r, 4},
        {"SUB A, (HL) Z1HC", 0, sub_a_hl, 8},
        {"SUB A, A Z1HC", 0, sub_a_r, 4},
        {"SBC A, B Z1HC", 0, sbc_a_r, 4},
        {"SBC A, C Z1HC", 0, sbc_a_r, 4},
        {"SBC A, D Z1HC", 0, sbc_a_r, 4},
        {"SBC A, E Z1HC", 0, sbc_a_r, 4},
        {"SBC A, H Z1HC", 0, sbc_a_r, 4},
        {"SBC A, L Z1HC", 0, sbc_a_r, 4},
        {"SBC A, (HL) Z1HC", 0, sbc_a_hl, 8},
        {"SBC A, A Z1HC", 0, sbc_a_r, 4},

        {"AND A, B Z010", 0, and_a_r, 4},
        {"AND A, C Z010", 0, and_a_r, 4},
        {"AND A, D Z010", 0, and_a_r, 4},
        {"AND A, E Z010", 0, and_a_r, 4},
        {"AND A, H Z010", 0, and_a_r, 4},
        {"AND A, L Z010", 0, and_a_r, 4},
        {"AND A, (HL) Z010", 0, and_a_hl, 8},
        {"AND A, A Z010", 0, and_a_r, 4},
        {"XOR A, B Z000", 0, xor_a_r, 4},
        {"XOR A, C Z000", 0, xor_a_r, 4},
        {"XOR A, D Z000", 0, xor_a_r, 4},
        {"XOR A, E Z000", 0, xor_a_r, 4},
        {"XOR A, H Z000", 0, xor_a_r, 4},
        {"XOR A, L Z000", 0, xor_a_r, 4},
        {"XOR A, (HL) Z000", 0, xor_a_hl, 8},
        {"XOR A, A Z000", 0, xor_a_r, 4},

        {"OR A, B Z000", 0, or_a_r, 4},
        {"OR A, C Z000", 0, or_a_r, 4},
        {"OR A, D Z000", 0, or_a_r, 4},
        {"OR A, E Z000", 0, or_a_r, 4},
        {"OR A, H Z000", 0, or_a_r, 4},
        {"OR A, L Z000", 0, or_a_r, 4},
        {"OR A, (HL) Z000", 0, or_a_hl, 8},
        {"OR A, A Z000", 0, or_a_r, 4},
        {"CP A, B Z0HC", 0, cp_a_r, 4},
        {"CP A, C Z0HC", 0, cp_a_r, 4},
        {"CP A, D Z0HC", 0, cp_a_r, 4},
        {"CP A, E Z0HC", 0, cp_a_r, 4},
        {"CP A, H Z0HC", 0, cp_a_r, 4},
        {"CP A, L Z0HC", 0, cp_a_r, 4},
        {"CP A, (HL) Z0HC", 0, cp_a_hl, 8},
        {"CP A, A Z0HC", 0, cp_a_r, 4},

        {"RET NZ ----", 0, ret_f, 8-20},
        {"POP BC ----", 0, pop_rr, 12},
        {"JP NZ, u16 ----", 2, jp_f_nn, 12-16},
        {"JP u16 ----", 2, jp_nn, 16},
        {"CALL NZ, u16 ----", 2, call_f_nn, 12-24},
        {"PUSH BC ----", 0, push_rr, 16},
        {"ADD A, u8 Z0HC", 1, add_a_n, 8},
        {"RST 00h ----", 0, rst, 16},
        {"RET Z ----", 0, ret_f, 8-20},
        {"RET ----", 0, ret, 16},
        {"JP Z, u16 ----", 2, jp_f_nn, 12-16},
        {"PREFIX CB ----", 0, cb, 4},
        {"CALL Z, u16 ----", 2, call_f_nn, 12-24},
        {"CALL u16 ----", 2, call_nn, 24},
        {"ADC A, u8 Z0HC", 1, adc_a_n, 8},
        {"RST 08h ----", 0, rst, 16},

        {"RET NC ----", 0, ret_f, 8-20},
        {"POP DE ----", 0, pop_rr, 12},
        {"JP NC, u16 ----", 2, jp_f_nn, 12-16},
        {"NO INSTR ----", 0, noi, 0},
        {"CALL NC, u16 ----", 2, call_f_nn, 12-24},
        {"PUSH DE ----", 0, push_rr, 16},
        {"SUB A, u8 Z1HC", 1, sub_a_n, 8},
        {"RST 10h ----", 0, rst, 16},
        {"RET C ----", 0, ret_f, 8-20},
        {"RETI ----", 0, reti, 16},
        {"JP C, u16 ----", 2, jp_f_nn, 12-16},
        {"NO INSTR ----", 0, noi, 0},
        {"CALL C, u16 ----", 2, call_f_nn, 12-24},
        {"NO INSTR ----", 0, noi, 0},
        {"SBC A, u8 Z1HC", 1, sbc_a_n, 8},
        {"RST 18h ----", 0, rst, 16},

        {"LD (FF00+u8), A ----", 1, ld_ffn_a, 12},
        {"POP HL ----", 0, pop_rr, 12},
        {"LD (FF00+C), A ----", 0, ld_ffc_a, 8},
        {"NO INSTR ----", 0, noi, 0},
        {"NO INSTR ----", 0, noi, 0},
        {"PUSH HL ----", 0, push_rr, 16},
        {"AND A, u8 Z010", 1, and_a_n, 8},
        {"RST 20h ----", 0, rst, 16},
        {"ADD SP, dd 00HC", 1, add_sp_dd, 16},
        {"JP HL ----", 0, jp_hl, 4},
        {"LD (u16), A ----", 2, ld_nn_a, 16},
        {"NO INSTR ----", 0, noi, 0},
        {"NO INSTR ----", 0, noi, 0},
        {"NO INSTR ----", 0, noi, 0},
        {"XOR A, u8 Z000", 1, xor_a_n, 8},
        {"RST 28h ----", 0, rst, 16},

        {"LD A, (FF00+u8) ----", 1, ld_a_ffn, 12},
        {"POP AF ZNHC 0", 0, pop_rr, 12},
        {"LD (FF00+C), A ----", 0, ld_ffc_a, 8},
        {"DI ----", 0, di, 4},
        {"NO INSTR ----", 0, noi, 0},
        {"PUSH AF ----", 0, push_rr, 16},
        {"OR A, u8 Z000", 1, or_a_n, 8},
        {"RST 30h ----", 0, rst, 16},
        {"LD HL, SP+dd 00HC", 1, ld_hl_spdd, 12},
        {"LD SP, HL ----", 1, ld_sp_hl, 8},
        {"LD A, (u16) ----", 2, ld_a_nn, 16},
        {"EI ----", 0, ei, 4},
        {"NO INSTR ----", 0, noi, 0},
        {"NO INSTR ----", 0, noi, 0},
        {"CP A, u8 Z1HC", 1, cp_a_n, 8},
        {"RST 38h ----", 0, rst, 16},
    };

    uint8_t opcode;

public:
    CPU();

    // ALU
    void writeFlags(uint8_t z, uint8_t n, uint8_t h, uint8_t c);
    bool readZFlag();
    bool readNFlag();
    bool readHFlag();
    bool readCFlag();
    
    void ADD(uint16_t a, uint16_t b);
    void ADD(uint8_t a, uint8_t b);
    void SUB(uint16_t a, uint16_t b);
    void SUB(uint8_t a, uint8_t b);
    void AND(uint8_t a, uint8_t b);
    void XOR(uint8_t a, uint8_t b);
    void OR(uint8_t a, uint8_t b);
    void CP(uint8_t a, uint8_t b);

    void step();

    uint8_t readByte(); // reads byte from M[pc] and incr pc by 1
    uint16_t readWord(); // reads word from M[pc] and incr pc by 2

    uint8_t readR(uint8_t r);
    uint16_t readRR(uint8_t rr, uint8_t group);
    void writeR(uint8_t r, uint8_t val);
    void writeRR(uint8_t rr, uint8_t group, uint16_t val);

    void exec();

    void noi();
    // 8 bit loads
    void ld_r_r();
    void ld_r_n();
    void ld_r_hl();
    void ld_hl_r();
    void ld_hl_n();
    void ld_a_bc();
    void ld_a_de();
    void ld_a_nn();
    void ld_bc_a();
    void ld_de_a();
    void ld_nn_a();
    void ld_a_ffn();
    void ld_ffn_a();
    void ld_a_ffc();
    void ld_ffc_a();
    void ldi_hl_a();
    void ldi_a_hl();
    void ldd_hl_a();
    void ldd_a_hl();

    // 16 bit loads
    void ld_rr_nn();
    void ld_nn_sp();
    void ld_sp_hl();
    void push_rr();
    void pop_rr();

    // 8 bit ALU
    void add_a_r();
    void add_a_n();
    void add_a_hl();
    void adc_a_r();
    void adc_a_n();
    void adc_a_hl();
    void sub_a_r();
    void sub_a_n();
    void sub_a_hl();
    void sbc_a_r();
    void sbc_a_n();
    void sbc_a_hl();
    void and_a_r();
    void and_a_n();
    void and_a_hl();
    void xor_a_r();
    void xor_a_n();
    void xor_a_hl();
    void or_a_r();
    void or_a_n();
    void or_a_hl();
    void cp_a_r();
    void cp_a_n();
    void cp_a_hl();
    void inc_r();
    void inc_hl();
    void dec_r();
    void dec_hl();
    void daa();
    void cpl();

    // 16 bit ALU
    void add_hl_rr();
    void inc_rr();
    void dec_rr();
    void add_sp_dd();
    void ld_hl_spdd();

    // rotate and shift
    void rlc_a();
    void rl_a();
    void rrc_a();
    void rr_a();
    void rlc_r();
    void rlc_hl();
    void rl_r();
    void rl_hl();
    void rrc_r();
    void rr_r();
    void rr_hl();
    void sl_a_r();
    void sl_a_hl();
    void swap_r();
    void swap_hl();
    void sr_a_r();
    void sr_a_hl();
    void srl_r();
    void srl_hl();

    // single bit operation
    void bit_n_r();
    void bit_n_hl();
    void set_n_r();
    void set_n_hl();
    void res_n_r();
    void res_n_hl();

    // cpu control
    void ccf();
    void scf();
    void nop();
    void halt();
    void stop();
    void di();
    void ei();

    // jump
    void jp_nn();
    void jp_hl();
    void jp_f_nn();
    void jr_pcdd();
    void jr_f_pcdd();
    void call_nn();
    void call_f_nn();
    void ret();
    void ret_f();
    void reti();
    void rst();

    // cb
    void cb();

};

