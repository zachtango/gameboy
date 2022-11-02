#include <stdint.h>

using namespace std;

#define MEMORY_SIZE 0x10000
#define REGISTERS_SIZE 8
#define TIMA 0xFF05 // timer address
#define TMA 0xFF06 // timer modulo
#define TAC 0xFF07 // time control

enum Register { B, C, D, E, H, L, F, A };

class Gameboy{

public:


private:
    
    uint8_t M[MEMORY_SIZE];
    uint8_t R[REGISTERS_SIZE];
    uint8_t sp; // points to top of stack in memory
    uint16_t pc; // index value of current instruction in memory arr
    uint8_t opcode; // M[pc]
    uint64_t t_cycles; // fixme not sure if this is the right variable
    bool IME;

    void step(); // fetch-decode-execute

    uint8_t readByte();

    uint8_t *register8(uint8_t);

    // LOAD instructions

    // 8 bit
    uint8_t ld_rr(); // Load to the 8-bit register r, the immediate data n
    uint8_t ld_rn(); // Load to the 8-bit register r, data from the absolute address specified by the 16-bit register HL
    uint8_t ld_rhl(); // Load to the absolute address specified by the 16-bit register HL, data from the 8-bit register r
    uint8_t ld_hlr(); // Load to the absolute address specified by the 16-bit register HL, the immediate data n
    uint8_t ld_hln(); // Load to the 8-bit A register, data from the absolute address specified by the 16-bit register BC
    uint8_t ld_abc(); // Load to the 8-bit A register, data from the absolute address specified by the 16-bit register DE
    uint8_t ld_ade(); // Load to the absolute address specified by the 16-bit register BC, data from the 8-bit A register.
    uint8_t ld_bca(); // Load to the absolute address specified by the 16-bit register DE, data from the 8-bit A register.
    uint8_t ld_dea(); // Load to the 8-bit A register, data from the absolute address specified by the 16-bit operand
    uint8_t ld_ann(); // Load to the absolute address specified by the 16-bit operand nn, data from the 8-bit A register.
    uint8_t ld_nna(); // Load to the absolute address specified by the 16-bit operand nn, data from the 8-bit A register
    uint8_t ldh_ac(); // Load to the 8-bit A register, data from the address specified by the 8-bit C register. The full 16-bit absolute address is obtained by setting the most significant byte to 0xFF and the least significant byte to the value of C, so the possible range is 0xFF00-0xFFFF
    uint8_t ldh_ca(); // Load to the address specified by the 8-bit C register, data from the 8-bit A register. The full 16-bit absolute address is obtained by setting the most significant byte to 0xFF and the least significant byte to the value of C, so the possible range is 0xFF00-0xFFFF
    uint8_t ldh_an(); // Load to the 8-bit A register, data from the address specified by the 8-bit immediate data n. The full 16-bit absolute address is obtained by setting the most significant byte to 0xFF and the least significant byte to the value of n, so the possible range is 0xFF00-0xFFFF.
    uint8_t ldh_na(); // Load to the address specified by the 8-bit immediate data n, data from the 8-bit A register. The full 16-bit absolute address is obtained by setting the most significant byte to 0xFF and the least significant byte to the value of n, so the possible range is 0xFF00-0xFFFF.
    uint8_t ldd_ahl();
    uint8_t ldd_hla();
    uint8_t ldi_hla();
    uint8_t ldi_ahl();

    // 16 bit
    uint8_t ld_rrnn();
    uint8_t ld_nnsp();
    uint8_t ld_sphl();
    uint8_t push_rr();
    uint8_t pop_rr();

    // END LOAD instructions

    // ARITHMETIC instructions

    // ADD A, n
    /*
        flags:
        Z - set if result is 0
        N - reset, indicates whether previous instr has been a subtraction
        H - set if carry from bit 3
        C - set if carry from bit 7
    */

    // 8 bit arithmetic
    uint8_t add_ar();
    uint8_t add_an();
    uint8_t add_ahl();
    uint8_t adc_ar();
    uint8_t adc_an();
    uint8_t adc_ahl();
    uint8_t sub_ar();
    uint8_t sub_an();
    uint8_t sub_ahl();
    uint8_t sbc_ar();
    uint8_t sbc_an();
    uint8_t sbc_ahl();
    uint8_t and_ar();
    uint8_t and_an();
    uint8_t and_ahl();
    uint8_t xor_ar();
    uint8_t xor_an();
    uint8_t xor_ahl();
    uint8_t or_ar();
    uint8_t or_an();
    uint8_t or_ahl();
    uint8_t cp_ar();
    uint8_t cp_an();
    uint8_t cp_ahl();
    uint8_t inc_r();
    uint8_t inc_hl();
    uint8_t dec_r();
    uint8_t dec_hl();
    uint8_t daa();
    uint8_t cpl();

    // 16 bit arithmetic
    uint8_t add_hlrr();
    uint8_t inc_rr();
    uint8_t dec_rr();
    uint8_t add_spdd();
    uint8_t ld_hlspdd();

    // END ARITHMETIC instructions

    // CONTROL FLOW instructions

    uint8_t jp_nn();
    uint8_t jp_hl();
    uint8_t jp_ccnn();
    uint8_t jr_e();
    uint8_t jr_cce();
    uint8_t call_nn();
    uint8_t call_ccnn();
    uint8_t ret();
    uint8_t ret_nn();
    uint8_t ret_cc();
    uint8_t reti();
    uint8_t rst_n();

    // END CONTROL FLOW instructions

};