#include <stdint.h>
#include <utility>
#include <unordered_map>

using namespace std;

#define BACKGROUND_TILE_SIZE 1024
#define BACKGROUND_TILEMAP0_ADDRESS 0x9800
#define BACKGROUND_TILEMAP1_ADDRESS 0x9C00
#define BACKGROUND_TILEDATA0_ADDRESS 0x8800
#define BACKGROUND_TILEDATA1_ADDRESS 0x8000
#define MEMORY_SIZE 0x10000
#define REGISTERS_SIZE 8
#define TIMA 0xFF05 // timer address
#define TMA 0xFF06 // timer modulo
#define TAC 0xFF07 // time control
#define LCD 0xFF40 // LCD control

enum Register { B, C, D, E, H, L, F, A };

class Gameboy{

public:
    Gameboy();
    void step(); // fetch-decode-execute
    uint8_t *backgroundTileMap[BACKGROUND_TILE_SIZE]; // tile size is 16 bytes, this points to first byte of tile
    uint8_t *windowTileMap[BACKGROUND_TILE_SIZE];
    void printVRAM();

private:

    uint8_t M[MEMORY_SIZE];
    uint8_t R[REGISTERS_SIZE];
    uint8_t sp; // points to top of stack in memory
    uint16_t pc; // index value of current instruction in memory arr
    uint8_t opcode; // M[pc]
    uint64_t t_cycles; // fixme not sure if this is the right variable type

    bool IME; // master flag for interrupts
    /*
        interrupt enable at 0xFFFF
            when bits are set, corresponding interrupt can be triggered
            0   Vblank (on/off)
            1   LCD stat (on/off)
            2   Timer (on/off)
            3   Serial (on/off)
            4   Joypad (on/off)
        interrupt flags at 0xFF0F
            when bits are set, an interrupt has happened
            corresponding bits same as IE
    */


    typedef uint8_t (Gameboy::*GameboyFunc)();
    unordered_map<uint8_t, GameboyFunc> instruction_noprefix; // opcode returns function pointer
    unordered_map<uint8_t, GameboyFunc> instruction_cbprefix;


    void mapBackgroundTiles();
    void mapWindowTiles();

    void loadROM(string);

    uint8_t readByte();

    void execInstruction();
    void mapInstructions();

    bool checkCondition(uint8_t);
    // flags bit 7, 6, 5, 4 of R[F]
    bool checkZero();
    bool checkCarry();
    bool checkH();
    bool checkN();

    void setZero(bool);
    void setCarry(bool);
    void setH(bool);
    void setN(bool);

    void handleInterrupt();

    // null instr
    void op_null();

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

    // ROTATE SHIFT instructions
    void rlc(uint8_t*);
    void rl(uint8_t*);
    void rrc(uint8_t*);
    void rr(uint8_t*);
    void sla(uint8_t*);
    void swap(uint8_t*);
    void sra(uint8_t*);
    void srl(uint8_t*);

    uint8_t rlc_a();
    uint8_t rl_a();
    uint8_t rrc_a();
    uint8_t rr_a();
    uint8_t rlc_r();
    uint8_t rlc_hl();
    uint8_t rl_r();
    uint8_t rl_hl();
    uint8_t rrc_r();
    uint8_t rrc_hl();
    uint8_t rr_r();
    uint8_t rr_hl();
    uint8_t sla_r();
    uint8_t sla_hl();
    uint8_t swap_r();
    uint8_t swap_hl();
    uint8_t sra_r();
    uint8_t sra_hl();
    uint8_t srl_r();
    uint8_t srl_hl();
    // END ROTATE SHIFT instructions

    // SINGLE-BIT OPERATION instructions
    uint8_t bit_nr();
    uint8_t bit_nhl();
    uint8_t set_nr();
    uint8_t set_nhl();
    uint8_t res_nr();
    uint8_t res_nhl();
    // END SINGLE-BIT OPERATION instructions

    // CPU CONTROL instructions
    uint8_t ccf();
    uint8_t scf();
    uint8_t nop();
    uint8_t halt();
    uint8_t stop();
    uint8_t di();
    uint8_t ei();
    // END CPU CONTROL instructions

    // JUMP instructions
    uint8_t jp_nn();
    uint8_t jp_hl();
    uint8_t jp_fnn();
    uint8_t jr_pcdd();
    uint8_t jr_fpcdd();
    uint8_t call_nn();
    uint8_t call_fnn();
    uint8_t ret();
    uint8_t ret_f();
    uint8_t reti();
    uint8_t rst_n();
    // END JUMP instructions

};