#include <stdint.h>

using namespace std;

#define MEMORY_SIZE 0x10000
#define REGISTERS_SIZE 8

class Gameboy{

public:


private:
    
    uint8_t M[MEMORY_SIZE];
    uint8_t R[REGISTERS_SIZE];
    uint8_t sp; // points to top of stack in memory
    uint16_t pc; // index value of current instruction in memory arr
    uint8_t opcode; // M[pc]

    // LOAD instructions

    // 8 bit
    void loadRR(uint8_t, uint8_t); // Load to the 8-bit register r, the immediate data n
    void loadRN(uint8_t, uint8_t); // Load to the 8-bit register r, data from the absolute address specified by the 16-bit register HL
    void loadRHL(uint8_t, uint16_t); // Load to the absolute address specified by the 16-bit register HL, data from the 8-bit register r
    void loadHLR(uint16_t, uint8_t); // Load to the absolute address specified by the 16-bit register HL, the immediate data n
    void loadHLN(uint16_t, uint8_t); // Load to the 8-bit A register, data from the absolute address specified by the 16-bit register BC
    void loadABC(uint8_t, uint16_t); // Load to the 8-bit A register, data from the absolute address specified by the 16-bit register DE
    void loadADE(uint8_t, uint16_t); // Load to the absolute address specified by the 16-bit register BC, data from the 8-bit A register.
    void loadBCA(uint16_t, uint8_t); // Load to the absolute address specified by the 16-bit register DE, data from the 8-bit A register.
    void loadDEA(uint16_t, uint8_t); // Load to the 8-bit A register, data from the absolute address specified by the 16-bit operand
    void loadANN(uint8_t, uint16_t); // Load to the absolute address specified by the 16-bit operand nn, data from the 8-bit A register.
    void loadNNA(uint16_t, uint8_t); // Load to the absolute address specified by the 16-bit operand nn, data from the 8-bit A register
    void loadhAC(uint8_t, uint8_t); // Load to the 8-bit A register, data from the address specified by the 8-bit C register. The full 16-bit absolute address is obtained by setting the most significant byte to 0xFF and the least significant byte to the value of C, so the possible range is 0xFF00-0xFFFF
    void loadhCA(uint8_t, uint8_t); // Load to the address specified by the 8-bit C register, data from the 8-bit A register. The full 16-bit absolute address is obtained by setting the most significant byte to 0xFF and the least significant byte to the value of C, so the possible range is 0xFF00-0xFFFF
    void loadhAN(uint8_t, uint8_t); // Load to the 8-bit A register, data from the address specified by the 8-bit immediate data n. The full 16-bit absolute address is obtained by setting the most significant byte to 0xFF and the least significant byte to the value of n, so the possible range is 0xFF00-0xFFFF.
    void loadhNA(uint8_t, uint8_t); // Load to the address specified by the 8-bit immediate data n, data from the 8-bit A register. The full 16-bit absolute address is obtained by setting the most significant byte to 0xFF and the least significant byte to the value of n, so the possible range is 0xFF00-0xFFFF.
    void loaddAHL(uint8_t, uint16_t);
    void loaddHLA(uint16_t, uint8_t);
    void loadiHLA(uint16_t, uint8_t);
    void loadiAHL(uint8_t, uint16_t);

    // 16 bit
    void loadRRNN(uint16_t, uint16_t);
    void loadNNSP(uint16_t);
    void loadSPHL(uint16_t);
    void pushRR(uint16_t);
    void popRR(uint16_t);

    // END LOAD instructions

    // CONTROL FLOW instructions

    void jumpNN(uint16_t);
    void jumpHL(uint16_t);
    void jumpCCNN(uint16_t, uint16_t);
    void jumprE(uint8_t);
    void jumprCCE(uint16_t, uint8_t);
    void callNN(uint16_t);
    void callCCNN(uint16_t, uint16_t);
    void ret();
    void retNN(uint16_t);
    void retCC(uint16_t);
    void reti();
    void rstN(uint8_t);

    // END CONTROL FLOW instructions

};