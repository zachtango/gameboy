#include "cpu.h"
#include "mmu.h"

using namespace std;

extern MMU mmu;

CPU::CPU(){
    regs.pc = 0x0100;
}

void CPU::step(){

    // read instr
    uint8_t opcode = readPC();

    // decode and exec instr
    exec(opcode);

    // handle interrupts


}

uint8_t CPU::readPC(){
    return mmu.readByte(regs.pc++);
}

void CPU::exec(uint8_t opcode){



}


