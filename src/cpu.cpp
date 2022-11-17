#include "cpu.h"

#include <iostream>

using namespace std;

extern MMU mmu;

CPU::CPU(){
    regs.pc = 0x0100;
}

void CPU::step(){

    // read instr
    printf("PC: %d\n", regs.pc);
    opcode = readByte();

    // decode and exec instr
    exec();

    // handle interrupts


}

uint8_t CPU::readByte(){
    return mmu.readByte(regs.pc++);
}

uint16_t CPU::readWord(){
    uint16_t addr = regs.pc;

    regs.pc += 2;

    return mmu.readWord(addr);
}

void CPU::exec(){

    instruction *instr = &instructions[opcode];
    fp *func = &instr->exec;

    cout << instr->name << endl;

    switch(instr->operandLen){
        case 2:
            readWord();
        case 1:
            readByte();
        case 0:
            // ((void(*)(void))instructions[opcode].exec)();

            break;
        default:
            cout << "operand len ERR\n";
            exit(-1);
    }
}

