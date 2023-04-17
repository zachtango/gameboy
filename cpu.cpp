#include "constants.h"
#include "helpers.h"
#include "cpu.h"
#include <iostream>
#include <iomanip>

void CPU::init_instr_tables() {
    std::cout << __func__ << '\n';
    
    instr[0x00] = &CPU::nop;
    instr[0x01] = &CPU::ld_r16_n16;
    instr[0x02] = &CPU::ld_mr16_A;
    instr[0x03] = &CPU::inc_r16;
    instr[0x04] = &CPU::inc_r8;
    instr[0x05] = &CPU::dec_r8;
    instr[0x06] = &CPU::ld_r8_n8;
    instr[0x07] = &CPU::rlca;
    instr[0x08] = &CPU::ld_mn16_SP;
    instr[0x09] = &CPU::add_HL_r16;
    instr[0x0A] = &CPU::ld_A_mr16;
    instr[0x0B] = &CPU::dec_r16;
    instr[0x0C] = &CPU::inc_r8;
    instr[0x0D] = &CPU::dec_r8;
    instr[0x0E] = &CPU::ld_r8_n8;
    instr[0x0F] = &CPU::rrca;

    instr[0x10] = &CPU::stop;
    instr[0x11] = &CPU::ld_r16_n16;
    instr[0x12] = &CPU::ld_mr16_A;
    instr[0x13] = &CPU::inc_r16;
    instr[0x14] = &CPU::inc_r8;
    instr[0x15] = &CPU::dec_r8;
    instr[0x16] = &CPU::ld_r8_n8;
    instr[0x17] = &CPU::rla;
    instr[0x18] = &CPU::jr_n16;
    instr[0x19] = &CPU::add_HL_r16;
    instr[0x1A] = &CPU::ld_A_mr16;
    instr[0x1B] = &CPU::dec_r16;
    instr[0x1C] = &CPU::inc_r8;
    instr[0x1D] = &CPU::dec_r8;
    instr[0x1E] = &CPU::ld_r8_n8;
    instr[0x1F] = &CPU::rra;

    instr[0x20] = &CPU::jr_cc_n16;
    instr[0x21] = &CPU::ld_r16_n16;
    instr[0x22] = &CPU::ld_mHLI_A;
    instr[0x23] = &CPU::inc_r16;
    instr[0x24] = &CPU::inc_r8;
    instr[0x25] = &CPU::dec_r8;
    instr[0x26] = &CPU::ld_r8_n8;
    instr[0x27] = &CPU::daa;
    instr[0x28] = &CPU::jr_n16;
    instr[0x29] = &CPU::add_HL_r16;
    instr[0x2A] = &CPU::ld_A_mHLI;
    instr[0x2B] = &CPU::dec_r16;
    instr[0x2C] = &CPU::inc_r8;
    instr[0x2D] = &CPU::dec_r8;
    instr[0x2E] = &CPU::ld_r8_n8;
    instr[0x2F] = &CPU::cpl;
    
    instr[0x30] = &CPU::jr_cc_n16;
    instr[0x31] = &CPU::ld_SP_n16;
    instr[0x32] = &CPU::ld_mHLD_A;
    instr[0x33] = &CPU::inc_SP;
    instr[0x34] = &CPU::inc_mHL;
    instr[0x35] = &CPU::dec_mHL;
    instr[0x36] = &CPU::ld_mHL_n8;
    instr[0x37] = &CPU::scf;
    instr[0x38] = &CPU::jr_n16;
    instr[0x39] = &CPU::add_HL_SP;
    instr[0x3A] = &CPU::ld_A_mHLD;
    instr[0x3B] = &CPU::dec_SP;
    instr[0x3C] = &CPU::inc_r8;
    instr[0x3D] = &CPU::dec_r8;
    instr[0x3E] = &CPU::ld_r8_n8;
    instr[0x3F] = &CPU::ccf;

    instr[0x40] = &CPU::ld_r8_r8;
    instr[0x41] = &CPU::ld_r8_r8;
    instr[0x42] = &CPU::ld_r8_r8;
    instr[0x43] = &CPU::ld_r8_r8;
    instr[0x44] = &CPU::ld_r8_r8;
    instr[0x45] = &CPU::ld_r8_r8;
    instr[0x46] = &CPU::ld_r8_mHL;
    instr[0x47] = &CPU::ld_r8_r8;
    instr[0x48] = &CPU::ld_r8_r8;
    instr[0x49] = &CPU::ld_r8_r8;
    instr[0x4A] = &CPU::ld_r8_r8;
    instr[0x4B] = &CPU::ld_r8_r8;
    instr[0x4C] = &CPU::ld_r8_r8;
    instr[0x4D] = &CPU::ld_r8_r8;
    instr[0x4E] = &CPU::ld_r8_mHL;
    instr[0x4F] = &CPU::ld_r8_r8;

    instr[0x50] = &CPU::ld_r8_r8;
    instr[0x51] = &CPU::ld_r8_r8;
    instr[0x52] = &CPU::ld_r8_r8;
    instr[0x53] = &CPU::ld_r8_r8;
    instr[0x54] = &CPU::ld_r8_r8;
    instr[0x55] = &CPU::ld_r8_r8;
    instr[0x56] = &CPU::ld_r8_mHL;
    instr[0x57] = &CPU::ld_r8_r8;
    instr[0x58] = &CPU::ld_r8_r8;
    instr[0x59] = &CPU::ld_r8_r8;
    instr[0x5A] = &CPU::ld_r8_r8;
    instr[0x5B] = &CPU::ld_r8_r8;
    instr[0x5C] = &CPU::ld_r8_r8;
    instr[0x5D] = &CPU::ld_r8_r8;
    instr[0x5E] = &CPU::ld_r8_mHL;
    instr[0x5F] = &CPU::ld_r8_r8;

    instr[0x60] = &CPU::ld_r8_r8;
    instr[0x61] = &CPU::ld_r8_r8;
    instr[0x62] = &CPU::ld_r8_r8;
    instr[0x63] = &CPU::ld_r8_r8;
    instr[0x64] = &CPU::ld_r8_r8;
    instr[0x65] = &CPU::ld_r8_r8;
    instr[0x66] = &CPU::ld_r8_mHL;
    instr[0x67] = &CPU::ld_r8_r8;
    instr[0x68] = &CPU::ld_r8_r8;
    instr[0x69] = &CPU::ld_r8_r8;
    instr[0x6A] = &CPU::ld_r8_r8;
    instr[0x6B] = &CPU::ld_r8_r8;
    instr[0x6C] = &CPU::ld_r8_r8;
    instr[0x6D] = &CPU::ld_r8_r8;
    instr[0x6E] = &CPU::ld_r8_mHL;
    instr[0x6F] = &CPU::ld_r8_r8;

    instr[0x70] = &CPU::ld_mHL_r8;
    instr[0x71] = &CPU::ld_mHL_r8;
    instr[0x72] = &CPU::ld_mHL_r8;
    instr[0x73] = &CPU::ld_mHL_r8;
    instr[0x74] = &CPU::ld_mHL_r8;
    instr[0x75] = &CPU::ld_mHL_r8;
    instr[0x76] = &CPU::halt;
    instr[0x77] = &CPU::ld_mHL_r8;
    instr[0x78] = &CPU::ld_r8_r8;
    instr[0x79] = &CPU::ld_r8_r8;
    instr[0x7A] = &CPU::ld_r8_r8;
    instr[0x7B] = &CPU::ld_r8_r8;
    instr[0x7C] = &CPU::ld_r8_r8;
    instr[0x7D] = &CPU::ld_r8_r8;
    instr[0x7E] = &CPU::ld_r8_mHL;
    instr[0x7F] = &CPU::ld_r8_r8;

    instr[0x80] = &CPU::add_A_r8;
    instr[0x81] = &CPU::add_A_r8;
    instr[0x82] = &CPU::add_A_r8;
    instr[0x83] = &CPU::add_A_r8;
    instr[0x84] = &CPU::add_A_r8;
    instr[0x85] = &CPU::add_A_r8;
    instr[0x86] = &CPU::add_A_mHL;
    instr[0x87] = &CPU::add_A_r8;
    instr[0x88] = &CPU::adc_A_r8;
    instr[0x89] = &CPU::adc_A_r8;
    instr[0x8A] = &CPU::adc_A_r8;
    instr[0x8B] = &CPU::adc_A_r8;
    instr[0x8C] = &CPU::adc_A_r8;
    instr[0x8D] = &CPU::adc_A_r8;
    instr[0x8E] = &CPU::adc_A_mHL;
    instr[0x8F] = &CPU::adc_A_r8;

    instr[0x90] = &CPU::sub_A_r8;
    instr[0x91] = &CPU::sub_A_r8;
    instr[0x92] = &CPU::sub_A_r8;
    instr[0x93] = &CPU::sub_A_r8;
    instr[0x94] = &CPU::sub_A_r8;
    instr[0x95] = &CPU::sub_A_r8;
    instr[0x96] = &CPU::sub_A_mHL;
    instr[0x97] = &CPU::sub_A_r8;
    instr[0x98] = &CPU::sbc_A_r8;
    instr[0x99] = &CPU::sbc_A_r8;
    instr[0x9A] = &CPU::sbc_A_r8;
    instr[0x9B] = &CPU::sbc_A_r8;
    instr[0x9C] = &CPU::sbc_A_r8;
    instr[0x9D] = &CPU::sbc_A_r8;
    instr[0x9E] = &CPU::sbc_A_mHL;
    instr[0x9F] = &CPU::sbc_A_r8;

    instr[0xA0] = &CPU::and_A_r8;
    instr[0xA1] = &CPU::and_A_r8;
    instr[0xA2] = &CPU::and_A_r8;
    instr[0xA3] = &CPU::and_A_r8;
    instr[0xA4] = &CPU::and_A_r8;
    instr[0xA5] = &CPU::and_A_r8;
    instr[0xA6] = &CPU::and_A_mHL;
    instr[0xA7] = &CPU::and_A_r8;
    instr[0xA8] = &CPU::xor_A_r8;
    instr[0xA9] = &CPU::xor_A_r8;
    instr[0xAA] = &CPU::xor_A_r8;
    instr[0xAB] = &CPU::xor_A_r8;
    instr[0xAC] = &CPU::xor_A_r8;
    instr[0xAD] = &CPU::xor_A_r8;
    instr[0xAE] = &CPU::xor_A_mHL;
    instr[0xAF] = &CPU::xor_A_r8;

    instr[0xB0] = &CPU::or_A_r8;
    instr[0xB1] = &CPU::or_A_r8;
    instr[0xB2] = &CPU::or_A_r8;
    instr[0xB3] = &CPU::or_A_r8;
    instr[0xB4] = &CPU::or_A_r8;
    instr[0xB5] = &CPU::or_A_r8;
    instr[0xB6] = &CPU::or_A_mHL;
    instr[0xB7] = &CPU::or_A_r8;
    instr[0xB8] = &CPU::cp_A_r8;
    instr[0xB9] = &CPU::cp_A_r8;
    instr[0xBA] = &CPU::cp_A_r8;
    instr[0xBB] = &CPU::cp_A_r8;
    instr[0xBC] = &CPU::cp_A_r8;
    instr[0xBD] = &CPU::cp_A_r8;
    instr[0xBE] = &CPU::cp_A_mHL;
    instr[0xBF] = &CPU::cp_A_r8;

    instr[0xC0] = &CPU::ret_cc;
    instr[0xC1] = &CPU::pop_r16;
    instr[0xC2] = &CPU::jp_cc_n16;
    instr[0xC3] = &CPU::jp_n16;
    instr[0xC4] = &CPU::call_cc_n16;
    instr[0xC5] = &CPU::push_r16;
    instr[0xC6] = &CPU::add_A_n8;
    instr[0xC7] = &CPU::rst;
    instr[0xC8] = &CPU::ret_cc;
    instr[0xC9] = &CPU::ret;
    instr[0xCA] = &CPU::jp_cc_n16;
    instr[0xCB] = &CPU::prefix_cb;
    instr[0xCC] = &CPU::call_cc_n16;
    instr[0xCD] = &CPU::call_n16;
    instr[0xCE] = &CPU::adc_A_n8;
    instr[0xCF] = &CPU::rst;

    instr[0xD0] = &CPU::ret_cc;
    instr[0xD1] = &CPU::pop_r16;
    instr[0xD2] = &CPU::jp_cc_n16;
    // instr[0xD3] ILLEGAL
    instr[0xD4] = &CPU::call_cc_n16;
    instr[0xD5] = &CPU::push_r16;
    instr[0xD6] = &CPU::sub_A_n8;
    instr[0xD7] = &CPU::rst;
    instr[0xD8] = &CPU::ret_cc;
    instr[0xD9] = &CPU::reti;
    instr[0xDA] = &CPU::jp_cc_n16;
    // instr[0xDB] ILLEGAL
    instr[0xDC] = &CPU::call_cc_n16;
    // instr[0xDD] ILLEGAL
    instr[0xDE] = &CPU::sbc_A_n8;
    instr[0xDF] = &CPU::rst;

    instr[0xE0] = &CPU::ldh_mn16_A;
    instr[0xE1] = &CPU::pop_r16;
    instr[0xE2] = &CPU::ldh_mC_A;
    // instr[0xE3] ILLEGAL
    // instr[0xE4] ILLEGAL
    instr[0xE5] = &CPU::push_r16;
    instr[0xE6] = &CPU::and_A_n8;
    instr[0xE7] = &CPU::rst;
    instr[0xE8] = &CPU::add_SP_e8;
    instr[0xE9] = &CPU::jp_HL;
    instr[0xEA] = &CPU::ld_mn16_A;
    // instr[0xEB] ILLEGAL
    // instr[0xEC] ILLEGAL
    // instr[0xED] ILLEGAL
    instr[0xEE] = &CPU::xor_A_n8;
    instr[0xEF] = &CPU::rst;

    instr[0xF0] = &CPU::ldh_A_mn16;
    instr[0xF1] = &CPU::pop_AF;
    instr[0xF2] = &CPU::ldh_A_mC;
    instr[0xF3] = &CPU::di;
    // instr[0xF4] ILLEGAL
    instr[0xF5] = &CPU::push_r16;
    instr[0xF6] = &CPU::or_A_n8;
    instr[0xF7] = &CPU::rst;
    instr[0xF8] = &CPU::ld_HL_SPe8;
    instr[0xF9] = &CPU::ld_SP_HL;
    instr[0xFA] = &CPU::ld_A_mn16;
    instr[0xFB] = &CPU::ei;
    // instr[0xFC] ILLEGAL
    // instr[0xFD] ILLEGAL
    instr[0xFE] = &CPU::cp_A_n8;
    instr[0xFF] = &CPU::rst;

    cb_instr[0x00] = &CPU::rlc_r8;
    cb_instr[0x01] = &CPU::rlc_r8;
    cb_instr[0x02] = &CPU::rlc_r8;
    cb_instr[0x03] = &CPU::rlc_r8;
    cb_instr[0x04] = &CPU::rlc_r8;
    cb_instr[0x05] = &CPU::rlc_r8;
    cb_instr[0x06] = &CPU::rlc_mHL;
    cb_instr[0x07] = &CPU::rrc_r8;
    cb_instr[0x08] = &CPU::rrc_r8;
    cb_instr[0x09] = &CPU::rrc_r8;
    cb_instr[0x0A] = &CPU::rrc_r8;
    cb_instr[0x0B] = &CPU::rrc_r8;
    cb_instr[0x0C] = &CPU::rrc_r8;
    cb_instr[0x0D] = &CPU::rrc_r8;
    cb_instr[0x0E] = &CPU::rrc_mHL;
    cb_instr[0x0F] = &CPU::rrc_r8;

    cb_instr[0x10] = &CPU::rl_r8;
    cb_instr[0x11] = &CPU::rl_r8;
    cb_instr[0x12] = &CPU::rl_r8;
    cb_instr[0x13] = &CPU::rl_r8;
    cb_instr[0x14] = &CPU::rl_r8;
    cb_instr[0x15] = &CPU::rl_r8;
    cb_instr[0x16] = &CPU::rl_mHL;
    cb_instr[0x17] = &CPU::rl_r8;
    cb_instr[0x18] = &CPU::rr_r8;
    cb_instr[0x19] = &CPU::rr_r8;
    cb_instr[0x1A] = &CPU::rr_r8;
    cb_instr[0x1B] = &CPU::rr_r8;
    cb_instr[0x1C] = &CPU::rr_r8;
    cb_instr[0x1D] = &CPU::rr_r8;
    cb_instr[0x1E] = &CPU::rr_mHL;
    cb_instr[0x1F] = &CPU::rr_r8;

    cb_instr[0x20] = &CPU::sla_r8;
    cb_instr[0x21] = &CPU::sla_r8;
    cb_instr[0x22] = &CPU::sla_r8;
    cb_instr[0x23] = &CPU::sla_r8;
    cb_instr[0x24] = &CPU::sla_r8;
    cb_instr[0x25] = &CPU::sla_r8;
    cb_instr[0x26] = &CPU::sla_mHL;
    cb_instr[0x27] = &CPU::sla_r8;
    cb_instr[0x28] = &CPU::sra_r8;
    cb_instr[0x29] = &CPU::sra_r8;
    cb_instr[0x2A] = &CPU::sra_r8;
    cb_instr[0x2B] = &CPU::sra_r8;
    cb_instr[0x2C] = &CPU::sra_r8;
    cb_instr[0x2D] = &CPU::sra_r8;
    cb_instr[0x2E] = &CPU::sra_mHL;
    cb_instr[0x2F] = &CPU::sra_r8;

    cb_instr[0x30] = &CPU::swap_r8;
    cb_instr[0x31] = &CPU::swap_r8;
    cb_instr[0x32] = &CPU::swap_r8;
    cb_instr[0x33] = &CPU::swap_r8;
    cb_instr[0x34] = &CPU::swap_r8;
    cb_instr[0x35] = &CPU::swap_r8;
    cb_instr[0x36] = &CPU::swap_mHL;
    cb_instr[0x37] = &CPU::swap_r8;
    cb_instr[0x38] = &CPU::srl_r8;
    cb_instr[0x39] = &CPU::srl_r8;
    cb_instr[0x3A] = &CPU::srl_r8;
    cb_instr[0x3B] = &CPU::srl_r8;
    cb_instr[0x3C] = &CPU::srl_r8;
    cb_instr[0x3D] = &CPU::srl_r8;
    cb_instr[0x3E] = &CPU::srl_mHL;
    cb_instr[0x3F] = &CPU::srl_r8;

    cb_instr[0x40] = &CPU::bit_u3_r8;
    cb_instr[0x41] = &CPU::bit_u3_r8;
    cb_instr[0x42] = &CPU::bit_u3_r8;
    cb_instr[0x43] = &CPU::bit_u3_r8;
    cb_instr[0x44] = &CPU::bit_u3_r8;
    cb_instr[0x45] = &CPU::bit_u3_r8;
    cb_instr[0x46] = &CPU::bit_u3_mHL;
    cb_instr[0x47] = &CPU::bit_u3_r8;
    cb_instr[0x48] = &CPU::bit_u3_r8;
    cb_instr[0x49] = &CPU::bit_u3_r8;
    cb_instr[0x4A] = &CPU::bit_u3_r8;
    cb_instr[0x4B] = &CPU::bit_u3_r8;
    cb_instr[0x4C] = &CPU::bit_u3_r8;
    cb_instr[0x4D] = &CPU::bit_u3_r8;
    cb_instr[0x4E] = &CPU::bit_u3_mHL;
    cb_instr[0x4F] = &CPU::bit_u3_r8;

    cb_instr[0x50] = &CPU::bit_u3_r8;
    cb_instr[0x51] = &CPU::bit_u3_r8;
    cb_instr[0x52] = &CPU::bit_u3_r8;
    cb_instr[0x53] = &CPU::bit_u3_r8;
    cb_instr[0x54] = &CPU::bit_u3_r8;
    cb_instr[0x55] = &CPU::bit_u3_r8;
    cb_instr[0x56] = &CPU::bit_u3_mHL;
    cb_instr[0x57] = &CPU::bit_u3_r8;
    cb_instr[0x58] = &CPU::bit_u3_r8;
    cb_instr[0x59] = &CPU::bit_u3_r8;
    cb_instr[0x5A] = &CPU::bit_u3_r8;
    cb_instr[0x5B] = &CPU::bit_u3_r8;
    cb_instr[0x5C] = &CPU::bit_u3_r8;
    cb_instr[0x5D] = &CPU::bit_u3_r8;
    cb_instr[0x5E] = &CPU::bit_u3_mHL;
    cb_instr[0x5F] = &CPU::bit_u3_r8;

    cb_instr[0x60] = &CPU::bit_u3_r8;
    cb_instr[0x61] = &CPU::bit_u3_r8;
    cb_instr[0x62] = &CPU::bit_u3_r8;
    cb_instr[0x63] = &CPU::bit_u3_r8;
    cb_instr[0x64] = &CPU::bit_u3_r8;
    cb_instr[0x65] = &CPU::bit_u3_r8;
    cb_instr[0x66] = &CPU::bit_u3_mHL;
    cb_instr[0x67] = &CPU::bit_u3_r8;
    cb_instr[0x68] = &CPU::bit_u3_r8;
    cb_instr[0x69] = &CPU::bit_u3_r8;
    cb_instr[0x6A] = &CPU::bit_u3_r8;
    cb_instr[0x6B] = &CPU::bit_u3_r8;
    cb_instr[0x6C] = &CPU::bit_u3_r8;
    cb_instr[0x6D] = &CPU::bit_u3_r8;
    cb_instr[0x6E] = &CPU::bit_u3_mHL;
    cb_instr[0x6F] = &CPU::bit_u3_r8;

    cb_instr[0x70] = &CPU::bit_u3_r8;
    cb_instr[0x71] = &CPU::bit_u3_r8;
    cb_instr[0x72] = &CPU::bit_u3_r8;
    cb_instr[0x73] = &CPU::bit_u3_r8;
    cb_instr[0x74] = &CPU::bit_u3_r8;
    cb_instr[0x75] = &CPU::bit_u3_r8;
    cb_instr[0x76] = &CPU::bit_u3_mHL;
    cb_instr[0x77] = &CPU::bit_u3_r8;
    cb_instr[0x78] = &CPU::bit_u3_r8;
    cb_instr[0x79] = &CPU::bit_u3_r8;
    cb_instr[0x7A] = &CPU::bit_u3_r8;
    cb_instr[0x7B] = &CPU::bit_u3_r8;
    cb_instr[0x7C] = &CPU::bit_u3_r8;
    cb_instr[0x7D] = &CPU::bit_u3_r8;
    cb_instr[0x7E] = &CPU::bit_u3_mHL;
    cb_instr[0x7F] = &CPU::bit_u3_r8;

    cb_instr[0x80] = &CPU::res_u3_r8;
    cb_instr[0x81] = &CPU::res_u3_r8;
    cb_instr[0x82] = &CPU::res_u3_r8;
    cb_instr[0x83] = &CPU::res_u3_r8;
    cb_instr[0x84] = &CPU::res_u3_r8;
    cb_instr[0x85] = &CPU::res_u3_r8;
    cb_instr[0x86] = &CPU::res_u3_mHL;
    cb_instr[0x87] = &CPU::res_u3_r8;
    cb_instr[0x88] = &CPU::res_u3_r8;
    cb_instr[0x89] = &CPU::res_u3_r8;
    cb_instr[0x8A] = &CPU::res_u3_r8;
    cb_instr[0x8B] = &CPU::res_u3_r8;
    cb_instr[0x8C] = &CPU::res_u3_r8;
    cb_instr[0x8D] = &CPU::res_u3_r8;
    cb_instr[0x8E] = &CPU::res_u3_mHL;
    cb_instr[0x8F] = &CPU::res_u3_r8;

    cb_instr[0x90] = &CPU::res_u3_r8;
    cb_instr[0x91] = &CPU::res_u3_r8;
    cb_instr[0x92] = &CPU::res_u3_r8;
    cb_instr[0x93] = &CPU::res_u3_r8;
    cb_instr[0x94] = &CPU::res_u3_r8;
    cb_instr[0x95] = &CPU::res_u3_r8;
    cb_instr[0x96] = &CPU::res_u3_mHL;
    cb_instr[0x97] = &CPU::res_u3_r8;
    cb_instr[0x98] = &CPU::res_u3_r8;
    cb_instr[0x99] = &CPU::res_u3_r8;
    cb_instr[0x9A] = &CPU::res_u3_r8;
    cb_instr[0x9B] = &CPU::res_u3_r8;
    cb_instr[0x9C] = &CPU::res_u3_r8;
    cb_instr[0x9D] = &CPU::res_u3_r8;
    cb_instr[0x9E] = &CPU::res_u3_mHL;
    cb_instr[0x9F] = &CPU::res_u3_r8;

    cb_instr[0xA0] = &CPU::res_u3_r8;
    cb_instr[0xA1] = &CPU::res_u3_r8;
    cb_instr[0xA2] = &CPU::res_u3_r8;
    cb_instr[0xA3] = &CPU::res_u3_r8;
    cb_instr[0xA4] = &CPU::res_u3_r8;
    cb_instr[0xA5] = &CPU::res_u3_r8;
    cb_instr[0xA6] = &CPU::res_u3_mHL;
    cb_instr[0xA7] = &CPU::res_u3_r8;
    cb_instr[0xA8] = &CPU::res_u3_r8;
    cb_instr[0xA9] = &CPU::res_u3_r8;
    cb_instr[0xAA] = &CPU::res_u3_r8;
    cb_instr[0xAB] = &CPU::res_u3_r8;
    cb_instr[0xAC] = &CPU::res_u3_r8;
    cb_instr[0xAD] = &CPU::res_u3_r8;
    cb_instr[0xAE] = &CPU::res_u3_mHL;
    cb_instr[0xAF] = &CPU::res_u3_r8;

    cb_instr[0xB0] = &CPU::res_u3_r8;
    cb_instr[0xB1] = &CPU::res_u3_r8;
    cb_instr[0xB2] = &CPU::res_u3_r8;
    cb_instr[0xB3] = &CPU::res_u3_r8;
    cb_instr[0xB4] = &CPU::res_u3_r8;
    cb_instr[0xB5] = &CPU::res_u3_r8;
    cb_instr[0xB6] = &CPU::res_u3_mHL;
    cb_instr[0xB7] = &CPU::res_u3_r8;
    cb_instr[0xB8] = &CPU::res_u3_r8;
    cb_instr[0xB9] = &CPU::res_u3_r8;
    cb_instr[0xBA] = &CPU::res_u3_r8;
    cb_instr[0xBB] = &CPU::res_u3_r8;
    cb_instr[0xBC] = &CPU::res_u3_r8;
    cb_instr[0xBD] = &CPU::res_u3_r8;
    cb_instr[0xBE] = &CPU::res_u3_mHL;
    cb_instr[0xBF] = &CPU::res_u3_r8;

    cb_instr[0xC0] = &CPU::set_u3_r8;
    cb_instr[0xC1] = &CPU::set_u3_r8;
    cb_instr[0xC2] = &CPU::set_u3_r8;
    cb_instr[0xC3] = &CPU::set_u3_r8;
    cb_instr[0xC4] = &CPU::set_u3_r8;
    cb_instr[0xC5] = &CPU::set_u3_r8;
    cb_instr[0xC6] = &CPU::set_u3_mHL;
    cb_instr[0xC7] = &CPU::set_u3_r8;
    cb_instr[0xC8] = &CPU::set_u3_r8;
    cb_instr[0xC9] = &CPU::set_u3_r8;
    cb_instr[0xCA] = &CPU::set_u3_r8;
    cb_instr[0xCB] = &CPU::set_u3_r8;
    cb_instr[0xCC] = &CPU::set_u3_r8;
    cb_instr[0xCD] = &CPU::set_u3_r8;
    cb_instr[0xCE] = &CPU::set_u3_mHL;
    cb_instr[0xCF] = &CPU::set_u3_r8;

    cb_instr[0xD0] = &CPU::set_u3_r8;
    cb_instr[0xD1] = &CPU::set_u3_r8;
    cb_instr[0xD2] = &CPU::set_u3_r8;
    cb_instr[0xD3] = &CPU::set_u3_r8;
    cb_instr[0xD4] = &CPU::set_u3_r8;
    cb_instr[0xD5] = &CPU::set_u3_r8;
    cb_instr[0xD6] = &CPU::set_u3_mHL;
    cb_instr[0xD7] = &CPU::set_u3_r8;
    cb_instr[0xD8] = &CPU::set_u3_r8;
    cb_instr[0xD9] = &CPU::set_u3_r8;
    cb_instr[0xDA] = &CPU::set_u3_r8;
    cb_instr[0xDB] = &CPU::set_u3_r8;
    cb_instr[0xDC] = &CPU::set_u3_r8;
    cb_instr[0xDD] = &CPU::set_u3_r8;
    cb_instr[0xDE] = &CPU::set_u3_mHL;
    cb_instr[0xDF] = &CPU::set_u3_r8;

    cb_instr[0xE0] = &CPU::set_u3_r8;
    cb_instr[0xE1] = &CPU::set_u3_r8;
    cb_instr[0xE2] = &CPU::set_u3_r8;
    cb_instr[0xE3] = &CPU::set_u3_r8;
    cb_instr[0xE4] = &CPU::set_u3_r8;
    cb_instr[0xE5] = &CPU::set_u3_r8;
    cb_instr[0xE6] = &CPU::set_u3_mHL;
    cb_instr[0xE7] = &CPU::set_u3_r8;
    cb_instr[0xE8] = &CPU::set_u3_r8;
    cb_instr[0xE9] = &CPU::set_u3_r8;
    cb_instr[0xEA] = &CPU::set_u3_r8;
    cb_instr[0xEB] = &CPU::set_u3_r8;
    cb_instr[0xEC] = &CPU::set_u3_r8;
    cb_instr[0xED] = &CPU::set_u3_r8;
    cb_instr[0xEE] = &CPU::set_u3_mHL;
    cb_instr[0xEF] = &CPU::set_u3_r8;

    cb_instr[0xF0] = &CPU::set_u3_r8;
    cb_instr[0xF1] = &CPU::set_u3_r8;
    cb_instr[0xF2] = &CPU::set_u3_r8;
    cb_instr[0xF3] = &CPU::set_u3_r8;
    cb_instr[0xF4] = &CPU::set_u3_r8;
    cb_instr[0xF5] = &CPU::set_u3_r8;
    cb_instr[0xF6] = &CPU::set_u3_mHL;
    cb_instr[0xF7] = &CPU::set_u3_r8;
    cb_instr[0xF8] = &CPU::set_u3_r8;
    cb_instr[0xF9] = &CPU::set_u3_r8;
    cb_instr[0xFA] = &CPU::set_u3_r8;
    cb_instr[0xFB] = &CPU::set_u3_r8;
    cb_instr[0xFC] = &CPU::set_u3_r8;
    cb_instr[0xFD] = &CPU::set_u3_r8;
    cb_instr[0xFE] = &CPU::set_u3_mHL;
    cb_instr[0xFF] = &CPU::set_u3_r8;
}

/*
    Instructions
*/

UINT CPU::run_fde(bool prefix) {

    // fetch
    opcode = mmu.read(PC);    
    std::cout << PC << ' '
        << std::hex << std::setw(2)
        << std::setfill('0') << static_cast<int>(opcode) << '\n';
    
    PC += 1;

    // decode
    cpu_instr f;
    if(prefix) f = cb_instr[opcode];
    else f = instr[opcode];

    UINT cycles;

    // execute
    if(EI) {
        cycles = (this->*f)();
        IME = true;
        EI = false;
    } else {
        cycles = (this->*f)();
        // might seem redundant but function might set EI
        // FIXME, maybe there's a better way to dot his
    }

    return cycles;
}

// Prefix
UINT CPU::prefix_cb() {
    std::cout << __func__ << '\n';
    return 4 + run_fde(true);
}

// 8 bit Arithmetic and Logic Instructions
BYTE CPU::_add_8(BYTE a, BYTE b, bool carry) {
    // Z0HC
    if(carry)
        carry = registers.get_c_flag();
    
    BYTE sum = a + b + (BYTE) carry;

    registers.set_z_flag(sum == 0);

    registers.set_n_flag(0);

    // bit 3 overflow
    registers.set_h_flag( ((a & 0x0F) + (b & 0x0F) + (BYTE) carry) > 0x0F );

    // bit 7 overflow
    registers.set_c_flag( (a + b + (BYTE) carry) > 0xFF );

    return sum;
}

BYTE CPU::_sub_8(BYTE a, BYTE b, bool carry) {
    // Z1HC
    if(carry)
        carry = registers.get_c_flag();

    BYTE diff = a - b - (BYTE) carry;

    registers.set_z_flag(diff == 0);

    registers.set_n_flag(1);

    // bit 4 borrow
    registers.set_h_flag( ((b & 0x0F) + (BYTE) carry) > (a & 0x0F) );

    // bit 8 borrow
    registers.set_c_flag( (b + (BYTE) carry) > a );

    return diff;
}

BYTE CPU::_and(BYTE a, BYTE b) {
    // Z010

    BYTE ab = a & b;

    registers.set_z_flag(ab == 0);

    registers.set_n_flag(0);

    registers.set_h_flag(1);

    registers.set_c_flag(0);
    
    return ab;
}

BYTE CPU::_or(BYTE a, BYTE b) {
    // Z000

    BYTE ab = a | b;

    registers.set_z_flag(ab == 0);

    registers.set_n_flag(0);

    registers.set_h_flag(0);

    registers.set_c_flag(0);

    return ab;
}

BYTE CPU::_xor(BYTE a, BYTE b) {
    // Z000

    BYTE ab = a ^ b;

    registers.set_z_flag(ab == 0);
    
    registers.set_n_flag(0);

    registers.set_h_flag(0);

    registers.set_c_flag(0);

    return ab;
}

BYTE CPU::_dec(BYTE d) {
    // Z1H-

    BYTE dec = d - 1;

    registers.set_z_flag(dec == 0);

    registers.set_n_flag(1);

    registers.set_h_flag( (d & 0x0F) == 0 );

    return dec;
}

BYTE CPU::_inc(BYTE i) {
    // Z0H-

    BYTE inc = i + 1;

    registers.set_z_flag(inc == 0);
    
    registers.set_n_flag(0);
    
    registers.set_h_flag( (i & 0x0F) == 0x0F );

    return inc;
}

void CPU::_add_A_r8(bool carry) {
    // Z0HC

    UINT r8 = _register_8(opcode & 0x07);

    BYTE sum = _add_8(
        registers.read_8(A),
        registers.read_8(r8),
        carry
    );

    registers.write_8(A, sum);
}

void CPU::_add_A_mHL(bool carry) {
    // Z0HC

    WORD hl = registers.read_16(HL);

    BYTE mHL = mmu.read(hl);

    BYTE sum = _add_8(
        registers.read_8(A),
        mHL,
        carry
    );

    registers.write_8(A, sum);
}

void CPU::_add_A_n8(bool carry) {
    // Z0HC

    BYTE n8 = mmu.read(PC);

    BYTE sum = _add_8(
        registers.read_8(A),
        n8,
        carry
    );

    PC += 1;

    registers.write_8(A, sum);
}

void CPU::_sub_A_r8(bool carry) {
    // Z1HC

    UINT r8 = _register_8(opcode & 0x07);

    BYTE diff = _sub_8(
        registers.read_8(A),
        registers.read_8(r8),
        carry
    );

    registers.write_8(A, diff);
}

void CPU::_sub_A_mHL(bool carry) {
    // Z1HC

    WORD hl = registers.read_16(HL);

    BYTE mHL = mmu.read(hl);

    BYTE diff = _sub_8(
        registers.read_8(A),
        mHL,
        carry
    );

    registers.write_8(A, diff);
}

void CPU::_sub_A_n8(bool carry) {
    // Z1HC

    BYTE n8 = mmu.read(PC); // FIXME get n8 from opcode

    BYTE diff = _sub_8(
        registers.read_8(A),
        n8,
        carry
    );

    PC += 1;

    registers.write_8(A, diff);
}

UINT CPU::adc_A_r8() {
    std::cout << __func__ << '\n';
    _add_A_r8(true);
    
    // 4 T Cycles
    return 4;
}

UINT CPU::adc_A_mHL() {
    std::cout << __func__ << '\n';
    _add_A_mHL(true);

    // 8 T Cycles
    return 8;
}

UINT CPU::adc_A_n8() {
    std::cout << __func__ << '\n';
    _add_A_n8(true);

    // 8 T Cycles;
    return 8;
}

UINT CPU::add_A_r8() {
    std::cout << __func__ << '\n';
    _add_A_r8(false);
    
    // 4 T Cycles
    return 4;
}

UINT CPU::add_A_mHL() {
    std::cout << __func__ << '\n';
    _add_A_mHL(false);

    // 8 T Cycles
    return 8;
}

UINT CPU::add_A_n8() {
    std::cout << __func__ << '\n';
    _add_A_n8(false);

    // 8 T Cycles;
    return 8;
}

UINT CPU::and_A_r8() {
    std::cout << __func__ << '\n';
    // Z010

    UINT r8 = _register_8(opcode & 0x07); // get r8 from opcode fIXME

    BYTE a = _and(
        registers.read_8(A),
        registers.read_8(r8)
    );

    registers.write_8(A, a);

    // 4 T Cycles
    return 4;
}

UINT CPU::and_A_mHL() {
    std::cout << __func__ << '\n'; 
    // Z010

    WORD hl = registers.read_16(HL);

    BYTE mHL = mmu.read(hl);

    BYTE a = _and(
        registers.read_8(A),
        mHL
    );
    
    registers.write_8(A, a);

    // 8 T Cycles
    return 8;
}

UINT CPU::and_A_n8() {
    std::cout << __func__ << '\n';
    // Z010

    BYTE n8 = mmu.read(PC); // FIXME get n8 from opcode

    BYTE a = _and(
        registers.read_8(A),
        n8
    );

    registers.write_8(A, a);

    PC += 1;

    // 8 T Cycles
    return 8;
}

UINT CPU::cp_A_r8() {
    std::cout << __func__ << '\n';
    // Z1HC

    UINT r8 = _register_8(opcode & 0x07); // FIXME get r8 from opcode

    _sub_8(
        registers.read_8(A),
        registers.read_8(r8),
        false
    );

    // 4 T Cycles
    return 4;
}

UINT CPU::cp_A_mHL() {
    std::cout << __func__ << '\n';
    // Z1HC

    WORD hl = registers.read_16(HL);

    BYTE mHL = mmu.read(hl);

    _sub_8(
        registers.read_8(A),
        mHL,
        false
    );

    // 8 T Cycles
    return 8;
}

UINT CPU::cp_A_n8() {
    std::cout << __func__ << '\n';
    // Z1HC

    BYTE n8 = mmu.read(PC); // FIXME get n8 from opcode

    _sub_8(
        registers.read_8(A),
        n8,
        false
    );

    PC += 1;

    // 8 T Cycles
    return 8;
}

UINT CPU::dec_r8() {
    std::cout << __func__ << '\n';
    // ZNH-

    UINT r8 = _register_8(opcode & 0x07); // FIXME get r8 from opcode

    BYTE dec = _dec(registers.read_8(r8));

    registers.write_8(r8, dec);

    // 4 T Cycles
    return 4;
}

UINT CPU::dec_mHL() {
    std::cout << __func__ << '\n';
    // ZNH-

    WORD hl = registers.read_16(HL);

    BYTE mHL = mmu.read(hl);

    BYTE dec = _dec(mHL);

    mmu.write(hl, dec);

    // 12 T Cycles
    return 12;
}

UINT CPU::inc_r8() {
    std::cout << __func__ << '\n';
    // Z0H-

    UINT r8 = _register_8(opcode & 0x07); // FIXME get r8 from opcode

    BYTE inc = _inc(registers.read_8(r8));

    registers.write_8(r8, inc);

    // 4 T Cycles
    return 4;
}

UINT CPU::inc_mHL() {
    std::cout << __func__ << '\n';
    // Z0H-

    WORD hl = registers.read_16(HL);

    BYTE mHL = mmu.read(hl);

    BYTE inc = _inc(mHL);

    mmu.write(hl, inc);

    // 12 T Cycles
    return 12;
}

UINT CPU::or_A_r8() {
    std::cout << __func__ << '\n';
    // Z000

    UINT r8 = _register_8(opcode & 0x07); // FIXME get r8 from opcode

    BYTE o = _or(
        registers.read_8(A),
        registers.read_8(r8)
    );

    registers.write_8(A, o);

    // 4 T Cycles
    return 4;
}

UINT CPU::or_A_mHL() {
    std::cout << __func__ << '\n';
    // Z000

    WORD hl = registers.read_16(HL);

    BYTE mHL = mmu.read(hl);

    BYTE o = _or(
        registers.read_8(A),
        mHL
    );

    registers.write_8(A, o);

    // 8 T Cycles
    return 8;
}

UINT CPU::or_A_n8() {
    std::cout << __func__ << '\n';
    // Z000

    BYTE n8 = mmu.read(PC); // FIXME get n8 from opcode

    BYTE o = _or(
        registers.read_8(A),
        n8
    );

    registers.write_8(A, o);

    PC += 1;

    // 8 T Cycles
    return 8;
}

UINT CPU::sbc_A_r8() {
    std::cout << __func__ << '\n';
    // Z1HC

    _sub_A_r8(true);

    // 4 T Cycles
    return 4;
}

UINT CPU::sbc_A_mHL() {
    std::cout << __func__ << '\n';
     // Z1HC

    _sub_A_mHL(true);

    // 8 T Cycles
    return 8;
}

UINT CPU::sbc_A_n8() {
    std::cout << __func__ << '\n';
    // Z1HC

    _sub_A_n8(true);

    // 8 T Cycles
    return 8;
}

UINT CPU::sub_A_r8() {
    std::cout << __func__ << '\n';
    // Z1HC

    _sub_A_r8(false);

    // 4 T Cycles
    return 4;
}

UINT CPU::sub_A_mHL() {
    std::cout << __func__ << '\n';
     // Z1HC

    _sub_A_mHL(false);

    // 8 T Cycles
    return 8;
}

UINT CPU::sub_A_n8() {
    std::cout << __func__ << '\n';
    // Z1HC

    _sub_A_n8(false);

    // 8 T Cycles
    return 8;
}

UINT CPU::xor_A_r8() {
    std::cout << __func__ << '\n';
    // Z000
    
    UINT r8 = _register_8(opcode & 0x07); // FIXME r8

    BYTE x = _xor(
        registers.read_8(A),
        registers.read_8(r8)
    );

    registers.write_8(A, x);

    // 4 T Cycles
    return 4;
}

UINT CPU::xor_A_mHL() {
    std::cout << __func__ << '\n';
    // Z000

    WORD hl = registers.read_16(HL);

    BYTE mHL = mmu.read(hl);

    BYTE x = _xor(
        registers.read_8(A),
        mHL
    );

    registers.write_8(A, x);
    
    // 8 T Cycles
    return 8;
}

UINT CPU::xor_A_n8() {
    std::cout << __func__ << '\n';
    // Z000

    BYTE n8 = mmu.read(PC); // FIXME read n8 from opcode

    BYTE x = _xor(
        registers.read_8(A),
        n8
    );

    registers.write_8(A, x);
    
    PC += 1;

    // 8 T Cycles
    return 8;
}

// 16 bit Arithmetic Instructions
WORD CPU::_add_16(WORD a, WORD b) {
    // -0HC

    registers.set_n_flag(0);

    registers.set_h_flag( ((a & 0x0FFF) + (b & 0x0FFF)) > 0x0FFF );

    registers.set_c_flag( (a + b) > 0xFFFF );

    return a + b;
}

UINT CPU::add_HL_r16() {
    std::cout << __func__ << '\n';
    // -0HC

    UINT r16 = _register_16( (opcode >> 4u) & 0x03 ); // FIXME get r16 from opcode

    WORD sum = _add_16(
        registers.read_16(HL),
        registers.read_16(r16)
    );

    registers.write_16(HL, sum);

    // 8 T Cycles
    return 8;
}

UINT CPU::dec_r16() {
    std::cout << __func__ << '\n';
    
    UINT r16 = _register_16( (opcode >> 4u) & 0x03 ); // FIXME get r16 from opcode

    WORD dec = registers.read_16(r16) - 1;

    registers.write_16(r16, dec);

    // 8 T Cycles
    return 8;
}

UINT CPU::inc_r16() {
    std::cout << __func__ << '\n';

    UINT r16 = _register_16( (opcode >> 4u) & 0x03 ); // FIXME get r16 from opcode

    WORD inc = registers.read_16(r16) + 1;

    registers.write_16(r16, inc);

    // 8 T Cycles
    return 8;
}

// Bit Operations Instructions
void CPU::_bit(BYTE b, UINT n) {
    // Z01-

    registers.set_z_flag( (b >> n) & 0x01 );

    registers.set_n_flag(0);

    registers.set_h_flag(1);

}

BYTE CPU::_res(BYTE b, UINT n) {
    return (b & ~(1u << n));
}

BYTE CPU::_set(BYTE b, UINT n) {
    return (b | (1u << n));
}

BYTE CPU::_swap(BYTE b) {
    // Z000

    b = ((b & 0x0F) << 4u) | ((b & 0xF0) >> 4u);

    registers.set_z_flag(b == 0);
    
    registers.set_n_flag(0);

    registers.set_h_flag(0);

    registers.set_c_flag(0);

    return b;
}

UINT CPU::bit_u3_r8() {
    std::cout << __func__ << '\n';
    // Z01-

    UINT r8 = _register_8(opcode & 0x07); // GET r8 from opcode
    UINT u3 = (opcode >> 3u) & 0x07; // GET u3 from opcode

    _bit(
        registers.read_8(r8),
        u3
    );

    // 8 T Cycles
    return 8;
}

UINT CPU::bit_u3_mHL() {
    std::cout << __func__ << '\n';
    // Z01-

    WORD hl = registers.read_16(HL);

    BYTE mHL = mmu.read(hl);

    UINT u3 = (opcode >> 3u) & 0x07;

    _bit(
        mHL,
        u3
    );

    // 12 T Cycles
    return 12;
}

UINT CPU::res_u3_r8() {
    std::cout << __func__ << '\n';
    
    UINT r8 = _register_8(opcode & 0x07);
    UINT u3 = (opcode >> 3u) & 0x07;

    BYTE res = _res(
        registers.read_8(r8),
        u3
    );

    registers.write_8(r8, res);

    // 8 T Cycles
    return 8;
}

UINT CPU::res_u3_mHL() {
    std::cout << __func__ << '\n';

    WORD hl = registers.read_16(HL);

    BYTE mHL = mmu.read(hl);

    UINT u3 = (opcode >> 3u) & 0x07;

    BYTE res = _res(
        mHL,
        u3
    );

    mmu.write(hl, res);

    // 16 T Cycles
    return 16;
}

UINT CPU::set_u3_r8() {
    std::cout << __func__ << '\n';

    UINT r8 = _register_8(opcode & 0x07);
    UINT u3 = (opcode >> 3u) & 0x07;

    BYTE set = _set(
        registers.read_8(r8),
        u3
    );

    registers.write_8(r8, u3);

    // 8 T Cycles
    return 8;
}

UINT CPU::set_u3_mHL() {
    std::cout << __func__ << '\n';

    WORD hl = registers.read_16(HL);

    BYTE mHL = mmu.read(hl);

    UINT u3 = (opcode >> 3u) & 0x07;

    BYTE set = _set(
        mHL,
        u3
    );

    mmu.write(hl, set);

    // 16 T Cycles
    return 16;
}

UINT CPU::swap_r8() {
    std::cout << __func__ << '\n';
    // Z000

    UINT r8 = _register_8(opcode & 0x07);

    BYTE s = _swap(
        registers.read_8(r8)
    );
    
    registers.write_8(r8, s);

    // 8 T Cycles
    return 8;
}

UINT CPU::swap_mHL() {
    std::cout << __func__ << '\n';
    // Z000

    WORD hl = registers.read_16(HL);

    BYTE mHL = mmu.read(hl);

    BYTE s = _swap(
        mHL
    );

    mmu.write(hl, s);

    // 16 T Cycles
    return 16;
}

// Bit Shift Instructions
BYTE CPU::_rl(BYTE b, bool through_carry) {
    // Z00C

    bool _lsb;

    if(through_carry) {
        _lsb = registers.get_c_flag();
    } else {
        _lsb = ( (b >> 7u) & 0x01 );
    }

    BYTE rl = (b << 1u) | (BYTE) _lsb;

    registers.set_z_flag(rl == 0);

    registers.set_n_flag(0);

    registers.set_h_flag(0);

    registers.set_c_flag( (b >> 7u) & 0x01 );

    return rl;
}

BYTE CPU::_rr(BYTE b, bool through_carry) {
    // Z00C

    bool _msb;

    if(through_carry) {
        _msb = registers.get_c_flag();
    } else {
        _msb = ( b & 0x01 );
    }

    BYTE rr = (BYTE) (_msb << 7u) | (b >> 1u);

    registers.set_z_flag(rr == 0);
    
    registers.set_n_flag(0);

    registers.set_h_flag(0);

    registers.set_c_flag( b & 0x01 );

    return rr;
}

BYTE CPU::_sl(BYTE b) {

    BYTE sl = b << 1u;

    registers.set_z_flag(sl == 0);
    
    registers.set_n_flag(0);

    registers.set_h_flag(0);

    registers.set_c_flag( (b >> 7u) & 0x01 );

    return sl;
}

BYTE CPU::_sr(BYTE b, bool arithmetic) {

    bool _msb;

    if(arithmetic) {
        _msb = (b >> 7u) & 0x01;
    } else {
        _msb = 0;
    }

    BYTE sr = ((BYTE) _msb << 7u) | (b >> 1u);

    registers.set_z_flag(sr == 0);

    registers.set_n_flag(0);
    
    registers.set_h_flag(0);

    registers.set_c_flag( b & 0x01 );

    return sr;
}

UINT CPU::rl_r8() {
    std::cout << __func__ << '\n';
    // Z00C

    UINT r8 = _register_8(opcode & 0x07);

    BYTE rl = _rl(
        registers.read_8(r8),
        true
    );

    registers.write_8(r8, rl);

    // 8 T Cycles
    return 8;
}

UINT CPU::rl_mHL() {
    std::cout << __func__ << '\n';
    // Z00C

    WORD hl = registers.read_16(HL);

    BYTE mHL = mmu.read(hl);

    BYTE rl = _rl(
        mHL,
        true
    );

    mmu.write(hl, rl);

    // 16 T Cycles
    return 16;
}

UINT CPU::rla() {
    std::cout << __func__ << '\n';
    // 000C

    BYTE rl = _rl(
        registers.read_8(A),
        true
    );

    registers.set_z_flag(0);

    registers.write_8(A, rl);

    // 4 T Cycles
    return 4;
}

UINT CPU::rlc_r8() {
    std::cout << __func__ << '\n';
    // Z00C

    UINT r8 = _register_8(opcode & 0x07);

    BYTE rl = _rl(
        registers.read_8(r8),
        false
    );

    registers.write_8(r8, rl);

    // 8 T Cycles
    return 8;
}

UINT CPU::rlc_mHL() {
    std::cout << __func__ << '\n';
    // Z00C

    WORD hl = registers.read_16(HL);

    BYTE mHL = mmu.read(hl);

    BYTE rl = _rl(
        mHL,
        false
    );

    mmu.write(hl, rl);

    // 16 T Cycles
    return 16;
}

UINT CPU::rlca() {
    std::cout << __func__ << '\n';
    // 000C
    
    BYTE rl = _rl(
        registers.read_8(A),
        false
    );

    registers.set_z_flag(0);

    registers.write_8(A, rl);

    // 4 T Cycles
    return 4;
}

UINT CPU::rr_r8() {
    std::cout << __func__ << '\n';
    // Z00C

    UINT r8 = _register_8(opcode & 0x07);

    BYTE rr = _rr(
        registers.read_8(r8),
        true
    );

    registers.write_8(r8, rr);

    // 8 T Cycles
    return 8;
}

UINT CPU::rr_mHL() {
    std::cout << __func__ << '\n';
    // Z00C

    WORD hl = registers.read_16(HL);

    BYTE mHL = mmu.read(hl);

    BYTE rr = _rr(
        mHL,
        true
    );

    mmu.write(hl, rr);

    // 16 T Cycles
    return 16;
}

UINT CPU::rra() {
    std::cout << __func__ << '\n';
    // 000C

    BYTE rr = _rr(
        registers.read_8(A),
        true
    );

    registers.set_z_flag(0);

    registers.write_8(A, rr);

    // 4 T Cycles
    return 4;
}

UINT CPU::rrc_r8() {
    std::cout << __func__ << '\n';
    // Z00C

    UINT r8 = _register_8(opcode & 0x07);

    BYTE rr = _rr(
        registers.read_8(r8),
        false
    );

    registers.write_8(r8, rr);

    // 8 T Cycles
    return 8;
}

UINT CPU::rrc_mHL() {
    std::cout << __func__ << '\n';
    // Z00C

    WORD hl = registers.read_16(HL);

    BYTE mHL = mmu.read(hl);

    BYTE rr = _rr(
        mHL,
        false
    );

    mmu.write(hl, rr);

    // 16 T Cycles
    return 16;
}

UINT CPU::rrca() {
    std::cout << __func__ << '\n';
    // 000C
    
    BYTE rr = _rr(
        registers.read_8(A),
        false
    );

    registers.write_8(A, rr);

    // 4 T Cycles
    return 4;
}

UINT CPU::sla_r8() {
    std::cout << __func__ << '\n';
    // Z00C
    
    UINT r8 = _register_8(opcode & 0x07);

    BYTE sl = _sl(
        registers.read_8(r8)
    );

    registers.write_8(
        r8,
        sl
    );

    // 8 T Cycles
    return 8;
}

UINT CPU::sla_mHL() {
    std::cout << __func__ << '\n';
    // Z00C

    WORD hl = registers.read_16(HL);

    BYTE mHL = mmu.read(hl);

    BYTE sl = _sl(
        mHL
    );

    mmu.write(hl, sl);

    // 16 T Cycles
    return 16;
}

UINT CPU::sra_r8() {
    std::cout << __func__ << '\n';
    // Z00C
    
    UINT r8 = _register_8(opcode & 0x07);

    BYTE sr = _sr(
        registers.read_8(r8),
        true
    );

    registers.write_8(
        r8,
        sr
    );

    // 8 T Cycles
    return 8;
}

UINT CPU::sra_mHL() {
    std::cout << __func__ << '\n';
    // Z00C

    WORD hl = registers.read_16(HL);

    BYTE mHL = mmu.read(hl);

    BYTE sr = _sr(
        mHL,
        true
    );

    mmu.write(hl, sr);

    // 16 T Cycles
    return 16;
}

UINT CPU::srl_r8() {
    std::cout << __func__ << '\n';
    // Z00C
    
    UINT r8 = _register_8(opcode & 0x07);

    BYTE sr = _sr(
        registers.read_8(r8),
        false
    );

    registers.write_8(
        r8,
        sr
    );

    // 8 T Cycles
    return 8;
}

UINT CPU::srl_mHL() {
    std::cout << __func__ << '\n';
    // Z00C

    WORD hl = registers.read_16(HL);

    BYTE mHL = mmu.read(hl);

    BYTE sr = _sr(
        mHL,
        false
    );

    mmu.write(hl, sr);

    // 16 T Cycles
    return 16;
}

// Load Instructions
UINT CPU::ld_r8_r8(){
    
    UINT a_r8 = (opcode >> 3u) & 0x07;
    UINT b_r8 = opcode & 0x07;

    registers.write_8(
        a_r8,
        registers.read_8(b_r8)
    );

    // 4 T Cycles
    return 4;
}

UINT CPU::ld_r8_n8(){

    UINT r8 = (opcode >> 3u) & 0x07;
    BYTE n8 = mmu.read(PC);

    registers.write_8(
        r8,
        n8
    );

    PC += 1;

    // 8 T Cycles
    return 8;
}

UINT CPU::ld_r16_n16(){

    UINT r16 = _register_16( (opcode >> 4u) & 0x03 );
    WORD n16 = concat(
        mmu.read(PC + 1),
        mmu.read(PC)
    );

    registers.write_16(
        r16,
        n16
    );

    PC += 2;

    // 12 T Cycles
    return 12;
}

UINT CPU::ld_mHL_r8(){

    WORD hl = registers.read_16(HL);

    UINT r8 = _register_8(opcode & 0x07);
    
    mmu.write(
        hl,
        registers.read_8(r8)
    );

    // 8 T Cycles
    return 8;
}

UINT CPU::ld_mHL_n8(){

    WORD hl = registers.read_16(HL);

    BYTE n8 = mmu.read(PC);

    mmu.write(
        hl,
        n8
    );

    PC += 1;

    // 12 T Cycles
    return 12;
}

UINT CPU::ld_r8_mHL(){

    WORD hl = registers.read_16(HL);

    BYTE mHL = mmu.read(hl);

    UINT r8 = _register_8(opcode & 0x07);

    registers.write_8(r8, mHL);

    // 8 T Cycles
    return 8;
}

UINT CPU::ld_mr16_A(){
    
    UINT r16 = _register_16( (opcode >> 4u) & 0x03 );

    mmu.write(
        registers.read_16(r16),
        registers.read_8(A)
    );
    
    // 8 T Cycles
    return 8;
}

UINT CPU::ld_mn16_A(){

    WORD n16 = concat(
        mmu.read(PC + 1),
        mmu.read(PC)
    );

    mmu.write(
        n16,
        registers.read_8(A)
    );

    PC += 2;

    // 16 T Cycles
    return 16;
}

UINT CPU::ldh_mn16_A(){

    BYTE n8 = mmu.read(PC);

    mmu.write(
        0xFF00 + n8,
        registers.read_8(A)
    );

    PC += 1;

    // 12 T Cycles
    return 12;
}

UINT CPU::ldh_mC_A(){

    mmu.write(
        0xFF00 + registers.read_8(C),
        registers.read_8(A)    
    );

    // 8 T Cycles
    return 8;
}

UINT CPU::ld_A_mr16(){

    UINT r16 = _register_16( (opcode >> 4u) & 0x03 );

    registers.write_8(
        A,
        mmu.read(registers.read_16(r16))
    );

    // 8 T Cycles
    return 8;
}

UINT CPU::ld_A_mn16(){

    WORD n16 = concat(
        mmu.read(PC + 1),
        mmu.read(PC)
    );

    registers.write_8(
        A,
        mmu.read(n16)
    );

    PC += 2;

    // 16 T Cycles
    return 16;
}

UINT CPU::ldh_A_mn16(){

    BYTE n8 = mmu.read(PC);

    registers.write_8(
        A,
        mmu.read(0xFF00 + n8)
    );

    PC += 1;

    // 12 T Cycles
    return 12;
}

UINT CPU::ldh_A_mC(){

    registers.write_8(
        A,
        mmu.read(0xFF00 + registers.read_8(C))
    );

    // 8 T Cycles
    return 8;
}

UINT CPU::ld_mHLI_A(){

    WORD hl = registers.read_16(HL);

    mmu.write(
        hl,
        registers.read_8(A)
    );

    registers.write_16(HL, hl + 1);

    // 8 T Cycles
    return 8;
}

UINT CPU::ld_mHLD_A(){

    WORD hl = registers.read_16(HL);

    mmu.write(
        hl,
        registers.read_8(A)
    );

    registers.write_16(HL, hl - 1);

    // 8 T Cycles
    return 8;
}

UINT CPU::ld_A_mHLI(){

    WORD hl = registers.read_16(HL);

    registers.write_8(
        A,
        mmu.read(hl)
    );

    registers.write_16(HL, hl + 1);

    // 8 T Cycles
    return 8;
}

UINT CPU::ld_A_mHLD(){
    
    WORD hl = registers.read_16(HL);

    registers.write_8(
        A,
        mmu.read(hl)
    );

    registers.write_16(HL, hl - 1);

    // 8 T Cycles
    return 8;
}

// Jump and Subroutines
bool CPU::_condition(UINT c) {
    switch(c) {
        case 0:
            return !registers.get_z_flag();
        case 1:
            return registers.get_z_flag();
        case 2:
            return !registers.get_c_flag();
        case 3:
            return registers.get_c_flag();
    }
    throw "Unknown condition";
}

void CPU::_call(WORD address) {
    mmu.write(SP - 1, lsb(PC));

    mmu.write(SP - 2, msb(PC));

    SP -= 2;

    PC = address;
}

UINT CPU::call_n16() {
    std::cout << __func__ << '\n';

    WORD n16 = concat(
        mmu.read(PC + 1),
        mmu.read(PC)
    );

    _call(n16);

    // 24 T Cycles
    return 24;
}

UINT CPU::call_cc_n16() {
    std::cout << __func__ << '\n';
    WORD n16 = concat(
        mmu.read(PC + 1),
        mmu.read(PC)
    );

    bool cc = _condition( (opcode >> 3u) & 0x03 );

    if(cc) {
        _call(n16);

        // 24 T Cycles
        return 24;
    }

    PC += 2;

    // 12 T Cycles
    return 12;
}

UINT CPU::jp_HL() {
    std::cout << __func__ << '\n';

    PC = registers.read_16(HL);

    // 4 T Cycles
    return 4;
}

UINT CPU::jp_n16() {
    std::cout << __func__ << '\n';

    WORD n16 = concat(
        mmu.read(PC + 1),
        mmu.read(PC)
    );

    PC = n16;

    // 16 T Cycles
    return 16;
}

UINT CPU::jp_cc_n16() {
    std::cout << __func__ << '\n';

    WORD n16 = concat(
        mmu.read(PC + 1),
        mmu.read(PC)
    );

    bool cc = _condition( (opcode >> 3u) & 0x03 );

    if(cc) {
        PC = n16;

        // 16 T Cycles
        return 16;
    }

    PC += 2;

    // 12 T Cycles
    return 12;
}

UINT CPU::jr_n16() {
    std::cout << __func__ << '\n';

    SIGNED_BYTE n8 = mmu.read(PC);

    // FIXME signed and unsigend arithmetic?
    PC += n8 + 1;

    // 12 T Cycles
    return 12;
}

UINT CPU::jr_cc_n16() {
    std::cout << __func__ << '\n';

    SIGNED_BYTE n8 = mmu.read(PC);

    bool cc = _condition( (opcode >> 3u) & 0x03 );

    if(cc) {
        // FIXME signed and unsigend arithmetic?
        PC += n8 + 1;

        // 8 T Cycles
        return 8;
    }

    PC += 1;

    // 12 T Cycles
    return 12;
}

UINT CPU::ret() {
    std::cout << __func__ << '\n';
    
    WORD p = concat(
        mmu.read(SP + 1),
        mmu.read(SP)
    );

    SP += 2;

    PC = p;

    // 16 T Cycles
    return 16;
}

UINT CPU::ret_cc() {
    std::cout << __func__ << '\n';

    bool cc = _condition( (opcode >> 3u) & 0x03 );

    if(cc) {
        ret();

        // 20 T Cycles
        return 20;
    }

    // 8 T Cycles
    return 8;
}

UINT CPU::reti() {
    std::cout << __func__ << '\n';
    // FIXME set IME

    ret();

    IME = 1;

    // 16 T Cycles
    return 16;
}

UINT CPU::rst() {
    std::cout << __func__ << '\n';

    WORD vec = opcode & 0b00111000;

    _call(vec);

    // 16 T Cycles
    return 16;
}

// Stack Operations Instructions
UINT CPU::add_HL_SP() {
    std::cout << __func__ << '\n';
    // -0HC

    WORD sum = _add_16(
        registers.read_16(HL),
        SP
    );

    registers.write_16(HL, sum);

    // 8 T Cycles
    return 8;
}

UINT CPU::add_SP_e8() {
    std::cout << __func__ << '\n';

    SIGNED_BYTE e8 = mmu.read(PC);

    // Set flags
    _add_8((BYTE) SP, e8, false);

    SP += e8;

    PC += 1;

    // 16 T Cycles
    return 16;
}

UINT CPU::dec_SP() {
    std::cout << __func__ << '\n';

    SP -= 1;

    // 8 T Cycles
    return 8;
}

UINT CPU::inc_SP() {
    std::cout << __func__ << '\n';

    SP += 1;

    // 8 T Cycles
    return 8;
}

UINT CPU::ld_SP_n16() {
    std::cout << __func__ << '\n';
    
    WORD n16 = concat(
        mmu.read(PC + 1),
        mmu.read(PC)
    );

    SP = n16;

    PC += 2;

    // 12 T Cycles
    return 12;
}

UINT CPU::ld_mn16_SP() {
    std::cout << __func__ << '\n';

    WORD n16 = concat(
        mmu.read(PC + 1),
        mmu.read(PC)
    );

    mmu.write(n16, (BYTE) (SP & 0xFF));
    mmu.write(n16 + 1, (BYTE) (SP >> 8u));

    PC += 2;

    // 20 T Cycles
    return 20;
}

UINT CPU::ld_HL_SPe8() {
    std::cout << __func__ << '\n';
    // 00HC

    SIGNED_BYTE e8 = mmu.read(PC);
    
    // set right flags
    _add_8((BYTE) SP, e8, false);

    registers.write_16(
        HL,
        SP + e8
    );
    
    PC += 1;

    // 12 T Cycles
    return 12;
}

UINT CPU::ld_SP_HL() {
    std::cout << __func__ << '\n';
    
    SP = registers.read_16(HL);

    // 8 T Cycles
    return 8;
}

UINT CPU::pop_AF() {
    std::cout << __func__ << '\n';
    // ZNHC

    BYTE a = mmu.read(SP + 1),
         f = mmu.read(SP);
        
    registers.write_16(AF, concat(a, f));

    SP += 2;

    // 12 T Cycles
    return 12;
}

UINT CPU::pop_r16() {
    std::cout << __func__ << '\n';

    UINT r16 = _register_16( (opcode >> 4u) & 0x03 );

    BYTE lo = mmu.read(SP),
         hi = mmu.read(SP + 1);

    registers.write_16(
        r16,
        concat(hi, lo)
    );

    SP += 2;

    // 12 T Cycles
    return 12;
}

UINT CPU::push_AF() {
    std::cout << __func__ << '\n';

    mmu.write(
        SP - 1, 
        registers.read_8(A)
    );

    mmu.write(
        SP - 2,
        registers.read_8(F) & 0xF0
    );
    
    SP -= 2;

    // 16 T Cycles
    return 16;
}

UINT CPU::push_r16() {
    std::cout << __func__ << '\n';

    UINT r16 = _register_16( (opcode >> 4u) & 0x03 );

    WORD r = registers.read_16(r16);

    mmu.write(
        SP - 1, 
        msb(r)
    );

    mmu.write(
        SP - 2,
        lsb(r)
    );
    
    SP -= 2;

    // 16 T Cycles
    return 16;
}


// Miscellaneous Instructions
UINT CPU::ccf() {
    std::cout << __func__ << '\n';
    // -00C

    registers.set_n_flag(0);

    registers.set_h_flag(0);

    registers.set_c_flag(
        !registers.get_c_flag()
    );

    // 4 T Cycles
    return 4;
}

UINT CPU::cpl() {
    std::cout << __func__ << '\n';
    // -11-
    
    registers.write_8(
        A,
        ~registers.read_8(A)
    );

    registers.set_n_flag(1);

    registers.set_h_flag(1);

    // 4 T Cycles
    return 4;
}

UINT CPU::daa() {
    std::cout << __func__ << '\n';
    // Z-0C

    // FIXME decimal adjust accumulator ??

    BYTE a = registers.read_8(A);

    bool n = registers.get_n_flag(),
         h = registers.get_h_flag(),
         c = registers.get_c_flag();

    if(n) {
        if(c) {
            a -= 0x60;
        } 
        if(h) {
            a -= 0x06;
        }
    } else {
        if(c || a > 0x99) {
            a += 0x60;
            registers.set_c_flag(1);
        }
        if(h || (a & 0x0F) > 0x09) {
            a += 0x06;
        }
    }

    registers.set_z_flag(a == 0);

    registers.set_h_flag(0);

    // 4 T Cycles
    return 4;
}

UINT CPU::di() {
    std::cout << __func__ << '\n';

    IME = 0;

    // 4 T Cycles
    return 4;
}

UINT CPU::ei() {
    std::cout << __func__ << '\n';

    // Set IME on next instr
    EI = true;

    // 4 T Cycles FIXME add next instr cycles
    return 4;
}

UINT CPU::halt() {
    std::cout << __func__ << '\n';
    halt_mode = true;
    
    // Wait for interrupt
    return 0;
}

UINT CPU::nop() {
    std::cout << __func__ << '\n';
    // no operation

    // 4 T Cycles
    return 4;
}

UINT CPU::scf() {
    std::cout << __func__ << '\n';
    // -001

    registers.set_n_flag(0);

    registers.set_h_flag(0);

    registers.set_c_flag(1);

    // 4 T Cycles
    return 4;
}

UINT CPU::stop() {
    std::cout << __func__ << '\n';
    sleep_mode = true;
    
    // Wait for interrupt or joypad
    return 0;
}
