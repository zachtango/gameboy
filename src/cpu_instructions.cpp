#include "cpu.h"

extern MMU mmu;

// referencing SM83_decoding-1.pdf in repo

void CPU::noi(){}
// 8 bit loads
void CPU::ld_r_r(){

    writeR(r8Dest, readR(r8Source));

}

void CPU::ld_r_n(){
    uint8_t n;

    writeR(r8Dest, n);

}

void CPU::ld_r_hl(){

    writeR(r8Dest, readR(r8Source));
}

void CPU::ld_hl_r(){

    writeR(r8Dest, readR(r8Source));
}

void CPU::ld_hl_n(){
    uint8_t n;

    writeR(r8Dest, n);
}

void CPU::ld_a_bc(){
    // fix me use enums
    writeR(0, mmu.readByte(readRR(r16, 2)));
}

void CPU::ld_a_de(){
    // fixme replace with ld_a_rr();
}

void CPU::ld_a_nn(){
    uint16_t nn;

    writeR(0, mmu.readByte(nn));
}

void CPU::ld_bc_a(){
    // fixme replace with ld_rr_a();
    mmu.writeByte(readRR(r16, 2), readR(0));
}

void CPU::ld_de_a(){
    // fixme replace with ld_rr_a();
    mmu.writeByte(readRR(r16, 2), readR(0));
}

void CPU::ld_nn_a(){
    uint16_t nn;

    mmu.writeByte(nn, readR(0));
}

void CPU::ld_a_ffn(){
    uint8_t n;

    mmu.writeByte(0xFF00 + n, readR(0));
}

void CPU::ld_ffn_a(){
    uint8_t n;

    writeR(0, mmu.readByte(0xFF00 + n));
}

void CPU::ld_a_ffc(){
    // fixme use enums
    writeR(0, mmu.readByte(0xFF00 + readR(2)));
}

void CPU::ld_ffc_a(){
    // fixme use enums
    mmu.writeByte(0xFF00 + readR(2), readR(0));
}

void CPU::ldi_hl_a(){
    // ld_rr_a();

    regs.hl += 1;
}

void CPU::ldi_a_hl(){
    // ld_a_rr();

    regs.hl += 1;
}

void CPU::ldd_hl_a(){
    // ld_rr_a();

    regs.hl -= 1;
}

void CPU::ldd_a_hl(){
    // ld_a_rr();

    regs.hl -= 1;
}

// 16 bit loads
void CPU::ld_rr_nn(){
    uint16_t nn;

    writeRR(r16, 1, nn);
}

void CPU::ld_nn_sp(){
    uint16_t nn;

    mmu.writeWord(nn, regs.sp);
}

void CPU::ld_sp_hl(){
    regs.sp = regs.hl;
}

void CPU::push_rr(){
    regs.sp -= 2;

    mmu.writeByte(regs.sp, readRR(r16, 3));
}

void CPU::pop_rr(){
    writeRR(r16, 3, mmu.readByte(regs.sp));

    regs.sp += 2;
}

// 8 bit ALU
void CPU::add_a_r(){}
void CPU::add_a_n(){}
void CPU::add_a_hl(){}
void CPU::adc_a_r(){}
void CPU::adc_a_n(){}
void CPU::adc_a_hl(){}
void CPU::sub_a_r(){}
void CPU::sub_a_n(){}
void CPU::sub_a_hl(){}
void CPU::sbc_a_r(){}
void CPU::sbc_a_n(){}
void CPU::sbc_a_hl(){}
void CPU::and_a_r(){}
void CPU::and_a_n(){}
void CPU::and_a_hl(){}
void CPU::xor_a_r(){}
void CPU::xor_a_n(){}
void CPU::xor_a_hl(){}
void CPU::or_a_r(){}
void CPU::or_a_n(){}
void CPU::or_a_hl(){}
void CPU::cp_a_r(){}
void CPU::cp_a_n(){}
void CPU::cp_a_hl(){}
void CPU::inc_r(){}
void CPU::inc_hl(){}
void CPU::dec_r(){}
void CPU::dec_hl(){}
void CPU::daa(){}
void CPU::cpl(){}

// 16 bit ALU
void CPU::add_hl_rr(){}
void CPU::inc_rr(){}
void CPU::dec_rr(){}
void CPU::add_sp_dd(){}
void CPU::ld_hl_spdd(){}

// rotate and shift
void CPU::rlc_a(){}
void CPU::rl_a(){}
void CPU::rrc_a(){}
void CPU::rr_a(){}
void CPU::rlc_r(){}
void CPU::rlc_hl(){}
void CPU::rl_r(){}
void CPU::rl_hl(){}
void CPU::rrc_r(){}
void CPU::rr_r(){}
void CPU::rr_hl(){}
void CPU::sl_a_r(){}
void CPU::sl_a_hl(){}
void CPU::swap_r(){}
void CPU::swap_hl(){}
void CPU::sr_a_r(){}
void CPU::sr_a_hl(){}
void CPU::srl_r(){}
void CPU::srl_hl(){}

// single bit operation
void CPU::bit_n_r(){}
void CPU::bit_n_hl(){}
void CPU::set_n_r(){}
void CPU::set_n_hl(){}
void CPU::res_n_r(){}
void CPU::res_n_hl(){}

// cpu control
void CPU::ccf(){}
void CPU::scf(){}
void CPU::nop(){}
void CPU::halt(){}
void CPU::stop(){}
void CPU::di(){}
void CPU::ei(){}

// jump
void CPU::jp_nn(){}
void CPU::jp_hl(){}
void CPU::jp_f_nn(){}
void CPU::jr_pcdd(){}
void CPU::jr_f_pcdd(){}
void CPU::call_nn(){}
void CPU::call_f_nn(){}
void CPU::ret(){}
void CPU::ret_f(){}
void CPU::reti(){}
void CPU::rst(){}

// cb
void CPU::cb(){}
