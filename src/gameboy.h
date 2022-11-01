#include <stdint.h>

using namespace std;

#define MEMORY_SIZE 0x10000
#define REGISTERS_SIZE 8
#define TIMA 0xFF05 // timer address
#define TMA 0xFF06 // timer modulo
#define TAC 0xFF07 // time control

enum Register { B, C, D, E, H, L, A, F };

class Gameboy{

public:


private:
    
    uint8_t M[MEMORY_SIZE];
    uint8_t R[REGISTERS_SIZE];
    uint8_t sp; // points to top of stack in memory
    uint16_t pc; // index value of current instruction in memory arr
    uint8_t opcode; // M[pc]
    uint64_t t_cycles; // fixme not sure if this is the right variable

    void step(); // fetch-decode-execute

    uint8_t readByte();

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
    void I_add();
    void I_adc();
    void I_sub();
    void I_sbc();
    void I_and();
    void I_or();
    void I_xor();
    void I_cp();
    void I_inc();
    void I_dec();

    // 16 bit arithmetic
    void I_addHL();
    void I_addSP();
    void I_incNN();
    void I_decNN();

    // END ARITHMETIC instructions

    // CONTROL FLOW instructions

    uint8_t jp_nn(uint16_t);
    uint8_t jp_hl(uint16_t);
    uint8_t jp_(uint16_t, uint16_t);
    uint8_t jprE(uint8_t);
    uint8_t jprCCE();
    uint8_t callNN(uint16_t);
    uint8_t callCCNN(uint16_t, uint16_t);
    uint8_t ret();
    uint8_t retNN(uint16_t);
    uint8_t retCC(uint16_t);
    uint8_t reti();
    uint8_t rstN(uint8_t);

    // END CONTROL FLOW instructions

};