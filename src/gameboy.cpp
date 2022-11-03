#include "gameboy.h"
#include <iostream>
#include <fstream>
#include <iomanip>

Gameboy::Gameboy(){
    pc = 0;
    t_cycles = 0;

    // https://gbdev.io/pandocs/Memory_Map.html
    // mem map
    // 0x0000 - 0x3FFF ROM cartridge, usually fixed bank
    // boot ROM burned inside the CPU, mapped over the cartridge ROM at first
    // boot-up animation plays before control handed to cartridge's ROM at 0x0100
    
    // https://gbdev.gg8.se/files/roms/bootroms/
    // boot rom
    ifstream file;
    file.open("roms/dmg_boot.bin", ios::binary | ios::ate);
    
    if( file.is_open() ){
        streampos size = file.tellg();
        char *buffer = new char[size];

        // load file instr into buffer
        file.seekg(0, ios::beg);
        file.read(buffer, size);
        file.close();

        for(uint32_t i = 0; i < size; i++){
            M[i] = buffer[i];
        }

        delete [] buffer;
    }

    // 0x4000 - 0x7FFF ROM cartridge, switchable bank
    // 0x8000 - 0x9FFFF video RAM
    // 0xA000 - 0xBFFFF external RAM
    

    // https://izik1.github.io/gbops/index.html
    // no prefix
    instruction_noprefix[0x00] = nop;
    instruction_noprefix[0x01] = ld_rrnn;
    instruction_noprefix[0x02] = ld_bca;
    instruction_noprefix[0x03] = inc_rr;
    instruction_noprefix[0x04] = inc_r;
    instruction_noprefix[0x05] = dec_r;
    instruction_noprefix[0x06] = ld_rn;
    instruction_noprefix[0x07] = rlc_a;
    instruction_noprefix[0x08] = ld_nnsp;
    instruction_noprefix[0x09] = add_hlrr;
    instruction_noprefix[0x0A] = ld_abc;
    instruction_noprefix[0x0B] = dec_rr;
    instruction_noprefix[0x0C] = inc_r;
    instruction_noprefix[0x0D] = dec_r;
    instruction_noprefix[0x0E] = ld_rn;
    instruction_noprefix[0x0F] = rrc_a;

    instruction_noprefix[0x10] = stop;
    instruction_noprefix[0x11] = ld_rrnn;
    instruction_noprefix[0x12] = ld_dea;
    instruction_noprefix[0x13] = inc_rr;
    instruction_noprefix[0x14] = inc_r;
    instruction_noprefix[0x15] = dec_r;
    instruction_noprefix[0x16] = ld_rn;
    instruction_noprefix[0x17] = rl_a;
    instruction_noprefix[0x18] = jr_pcdd;
    instruction_noprefix[0x19] = add_hlrr;
    instruction_noprefix[0x1A] = ld_ade;
    instruction_noprefix[0x1B] = dec_rr;
    instruction_noprefix[0x1C] = inc_r;
    instruction_noprefix[0x1D] = dec_r;
    instruction_noprefix[0x1E] = ld_rn;
    instruction_noprefix[0x1F] = rr_a;

    instruction_noprefix[0x20] = jr_fpcdd;
    instruction_noprefix[0x21] = ld_rrnn;
    instruction_noprefix[0x22] = ldi_hla;
    instruction_noprefix[0x23] = inc_hl;
    instruction_noprefix[0x24] = inc_r;
    instruction_noprefix[0x25] = dec_r;
    instruction_noprefix[0x26] = ld_rn;
    instruction_noprefix[0x27] = daa;
    instruction_noprefix[0x28] = jr_fpcdd;
    instruction_noprefix[0x29] = add_hlrr;
    instruction_noprefix[0x2A] = ldi_ahl;
    instruction_noprefix[0x2B] = dec_rr;
    instruction_noprefix[0x2C] = inc_r;
    instruction_noprefix[0x2D] = dec_r;
    instruction_noprefix[0x2E] = ld_rn;
    instruction_noprefix[0x2F] = cpl;

    instruction_noprefix[0x30] = jr_fpcdd;
    instruction_noprefix[0x31] = ld_rrnn;
    instruction_noprefix[0x32] = ldd_hla;
    instruction_noprefix[0x33] = inc_rr;
    instruction_noprefix[0x34] = inc_hl;
    instruction_noprefix[0x35] = dec_hl;
    instruction_noprefix[0x36] = ld_hln;
    instruction_noprefix[0x37] = scf;
    instruction_noprefix[0x38] = jr_fpcdd;
    instruction_noprefix[0x39] = add_hlrr;
    instruction_noprefix[0x3A] = ldd_ahl;
    instruction_noprefix[0x3B] = dec_rr;
    instruction_noprefix[0x3C] = inc_r;
    instruction_noprefix[0x3D] = dec_r;
    instruction_noprefix[0x3E] = ld_rn;
    instruction_noprefix[0x3F] = ccf;

    instruction_noprefix[0x40] = ld_rr;
    instruction_noprefix[0x41] = ld_rr;
    instruction_noprefix[0x42] = ld_rr;
    instruction_noprefix[0x43] = ld_rr;
    instruction_noprefix[0x44] = ld_rr;
    instruction_noprefix[0x45] = ld_rr;
    instruction_noprefix[0x46] = ld_rr;
    instruction_noprefix[0x47] = ld_rr;
    instruction_noprefix[0x48] = ld_rr;
    instruction_noprefix[0x49] = ld_rr;
    instruction_noprefix[0x4A] = ld_rr;
    instruction_noprefix[0x4B] = ld_rr;
    instruction_noprefix[0x4C] = ld_rr;
    instruction_noprefix[0x4D] = ld_rr;
    instruction_noprefix[0x4E] = ld_rr;
    instruction_noprefix[0x4F] = ld_rr;

    instruction_noprefix[0x50] = ld_rr;
    instruction_noprefix[0x51] = ld_rr;
    instruction_noprefix[0x52] = ld_rr;
    instruction_noprefix[0x53] = ld_rr;
    instruction_noprefix[0x54] = ld_rr;
    instruction_noprefix[0x55] = ld_rr;
    instruction_noprefix[0x56] = ld_rr;
    instruction_noprefix[0x57] = ld_rr;
    instruction_noprefix[0x58] = ld_rr;
    instruction_noprefix[0x59] = ld_rr;
    instruction_noprefix[0x5A] = ld_rr;
    instruction_noprefix[0x5B] = ld_rr;
    instruction_noprefix[0x5C] = ld_rr;
    instruction_noprefix[0x5D] = ld_rr;
    instruction_noprefix[0x5E] = ld_rr;
    instruction_noprefix[0x5F] = ld_rr;

    instruction_noprefix[0x60] = ld_rr;
    instruction_noprefix[0x61] = ld_rr;
    instruction_noprefix[0x62] = ld_rr;
    instruction_noprefix[0x63] = ld_rr;
    instruction_noprefix[0x64] = ld_rr;
    instruction_noprefix[0x65] = ld_rr;
    instruction_noprefix[0x66] = ld_rr;
    instruction_noprefix[0x67] = ld_rr;
    instruction_noprefix[0x68] = ld_rr;
    instruction_noprefix[0x69] = ld_rr;
    instruction_noprefix[0x6A] = ld_rr;
    instruction_noprefix[0x6B] = ld_rr;
    instruction_noprefix[0x6C] = ld_rr;
    instruction_noprefix[0x6D] = ld_rr;
    instruction_noprefix[0x6E] = ld_rr;
    instruction_noprefix[0x6F] = ld_rr;

    instruction_noprefix[0x70] = ld_rr;
    instruction_noprefix[0x71] = ld_rr;
    instruction_noprefix[0x72] = ld_rr;
    instruction_noprefix[0x73] = ld_rr;
    instruction_noprefix[0x74] = ld_rr;
    instruction_noprefix[0x75] = ld_rr;
    instruction_noprefix[0x76] = halt;
    instruction_noprefix[0x77] = ld_rr;
    instruction_noprefix[0x78] = ld_rr;
    instruction_noprefix[0x79] = ld_rr;
    instruction_noprefix[0x7A] = ld_rr;
    instruction_noprefix[0x7B] = ld_rr;
    instruction_noprefix[0x7C] = ld_rr;
    instruction_noprefix[0x7D] = ld_rr;
    instruction_noprefix[0x7E] = ld_rr;
    instruction_noprefix[0x7F] = ld_rr;

    instruction_noprefix[0x80] = add_ar;
    instruction_noprefix[0x81] = add_ar;
    instruction_noprefix[0x82] = add_ar;
    instruction_noprefix[0x83] = add_ar;
    instruction_noprefix[0x84] = add_ar;
    instruction_noprefix[0x85] = add_ar;
    instruction_noprefix[0x86] = add_ar;
    instruction_noprefix[0x87] = add_ar;
    instruction_noprefix[0x88] = adc_ar;
    instruction_noprefix[0x89] = adc_ar;
    instruction_noprefix[0x8A] = adc_ar;
    instruction_noprefix[0x8B] = adc_ar;
    instruction_noprefix[0x8C] = adc_ar;
    instruction_noprefix[0x8D] = adc_ar;
    instruction_noprefix[0x8E] = adc_ar;
    instruction_noprefix[0x8F] = adc_ar;

    instruction_noprefix[0x90] = sub_ar;
    instruction_noprefix[0x91] = sub_ar;
    instruction_noprefix[0x92] = sub_ar;
    instruction_noprefix[0x93] = sub_ar;
    instruction_noprefix[0x94] = sub_ar;
    instruction_noprefix[0x95] = sub_ar;
    instruction_noprefix[0x96] = sub_ar;
    instruction_noprefix[0x97] = sub_ar;
    instruction_noprefix[0x98] = sbc_ar;
    instruction_noprefix[0x99] = sbc_ar;
    instruction_noprefix[0x9A] = sbc_ar;
    instruction_noprefix[0x9B] = sbc_ar;
    instruction_noprefix[0x9C] = sbc_ar;
    instruction_noprefix[0x9D] = sbc_ar;
    instruction_noprefix[0x9E] = sbc_ar;
    instruction_noprefix[0x9F] = sbc_ar;

    instruction_noprefix[0x90] = sub_ar;
    instruction_noprefix[0x91] = sub_ar;
    instruction_noprefix[0x92] = sub_ar;
    instruction_noprefix[0x93] = sub_ar;
    instruction_noprefix[0x94] = sub_ar;
    instruction_noprefix[0x95] = sub_ar;
    instruction_noprefix[0x96] = sub_ar;
    instruction_noprefix[0x97] = sub_ar;
    instruction_noprefix[0x98] = sbc_ar;
    instruction_noprefix[0x99] = sbc_ar;
    instruction_noprefix[0x9A] = sbc_ar;
    instruction_noprefix[0x9B] = sbc_ar;
    instruction_noprefix[0x9C] = sbc_ar;
    instruction_noprefix[0x9D] = sbc_ar;
    instruction_noprefix[0x9E] = sbc_ar;
    instruction_noprefix[0x9F] = sbc_ar;

    instruction_noprefix[0xA0] = and_ar;
    instruction_noprefix[0xA1] = and_ar;
    instruction_noprefix[0xA2] = and_ar;
    instruction_noprefix[0xA3] = and_ar;
    instruction_noprefix[0xA4] = and_ar;
    instruction_noprefix[0xA5] = and_ar;
    instruction_noprefix[0xA6] = and_ar;
    instruction_noprefix[0xA7] = and_ar;
    instruction_noprefix[0xA8] = xor_ar;
    instruction_noprefix[0xA9] = xor_ar;
    instruction_noprefix[0xAA] = xor_ar;
    instruction_noprefix[0xAB] = xor_ar;
    instruction_noprefix[0xAC] = xor_ar;
    instruction_noprefix[0xAD] = xor_ar;
    instruction_noprefix[0xAE] = xor_ar;
    instruction_noprefix[0xAF] = xor_ar;

    instruction_noprefix[0xB0] = or_ar;
    instruction_noprefix[0xB1] = or_ar;
    instruction_noprefix[0xB2] = or_ar;
    instruction_noprefix[0xB3] = or_ar;
    instruction_noprefix[0xB4] = or_ar;
    instruction_noprefix[0xB5] = or_ar;
    instruction_noprefix[0xB6] = or_ar;
    instruction_noprefix[0xB7] = or_ar;
    instruction_noprefix[0xB8] = cp_ar;
    instruction_noprefix[0xB9] = cp_ar;
    instruction_noprefix[0xBA] = cp_ar;
    instruction_noprefix[0xBB] = cp_ar;
    instruction_noprefix[0xBC] = cp_ar;
    instruction_noprefix[0xBD] = cp_ar;
    instruction_noprefix[0xBE] = cp_ar;
    instruction_noprefix[0xBF] = cp_ar;

    instruction_noprefix[0xC0] = ret_f;
    instruction_noprefix[0xC1] = pop_rr;
    instruction_noprefix[0xC2] = jp_fnn;
    instruction_noprefix[0xC3] = jp_nn;
    instruction_noprefix[0xC4] = call_fnn;
    instruction_noprefix[0xC5] = push_rr;
    instruction_noprefix[0xC6] = add_ar;
    instruction_noprefix[0xC7] = rst_n;
    instruction_noprefix[0xC8] = ret_f;
    instruction_noprefix[0xC9] = ret;
    instruction_noprefix[0xCA] = jp_fnn;
    // instruction_noprefix[0xCB] CB prefixed handled at start
    instruction_noprefix[0xCC] = call_fnn;
    instruction_noprefix[0xCD] = call_nn;
    instruction_noprefix[0xCE] = adc_ar;
    instruction_noprefix[0xCF] = rst_n;

    instruction_noprefix[0xD0] = ret_f;
    instruction_noprefix[0xD1] = pop_rr;
    instruction_noprefix[0xD2] = jp_fnn;
    // instruction_noprefix[0xD3] = op_null; // illegal
    instruction_noprefix[0xD4] = call_fnn;
    instruction_noprefix[0xD5] = push_rr;
    instruction_noprefix[0xD6] = sub_ar;
    instruction_noprefix[0xD7] = rst_n;
    instruction_noprefix[0xD8] = ret_f;
    instruction_noprefix[0xD9] = reti;
    instruction_noprefix[0xDA] = jp_fnn;
    // instruction_noprefix[0xDB] = op_null; // illegal
    instruction_noprefix[0xDC] = call_fnn;
    // instruction_noprefix[0xDD] = op_null; illegal
    instruction_noprefix[0xDE] = sbc_ar;
    instruction_noprefix[0xDF] = rst_n;

    instruction_noprefix[0xE0] = ldh_na;
    instruction_noprefix[0xE1] = pop_rr;
    instruction_noprefix[0xE2] = ldh_ca;
    // instruction_noprefix[0xE3] = op_null;
    // instruction_noprefix[0xE4] = op_null;
    instruction_noprefix[0xE5] = push_rr;
    instruction_noprefix[0xE6] = add_ar;
    instruction_noprefix[0xE7] = rst_n;
    instruction_noprefix[0xE8] = add_spdd;
    instruction_noprefix[0xE9] = jp_hl;
    instruction_noprefix[0xEA] = ld_nna;
    // instruction_noprefix[0xEB] = op_null
    // instruction_noprefix[0xEC] = op_null
    // instruction_noprefix[0xED] = op_null
    instruction_noprefix[0xEE] = xor_ar;
    instruction_noprefix[0xEF] = rst_n;

    instruction_noprefix[0xF0] = ldh_an;
    instruction_noprefix[0xF1] = pop_rr;
    instruction_noprefix[0xF2] = ldh_ac;
    instruction_noprefix[0xF3] = di;
    // instruction_noprefix[0xF4] = op_null;
    instruction_noprefix[0xF5] = push_rr;
    instruction_noprefix[0xF6] = or_ar;
    instruction_noprefix[0xF7] = rst_n;
    instruction_noprefix[0xF8] = ld_hlspdd;
    instruction_noprefix[0xF9] = ld_sphl;
    instruction_noprefix[0xFA] = ld_ann;
    instruction_noprefix[0xFB] = ei;
    // instruction_noprefix[0xFC] = op_null;
    // instruction_noprefix[0xFD] = op_null;
    instruction_noprefix[0xFE] = cp_ar;
    instruction_noprefix[0xFF] = rst_n;

    // CB prefixed
    instruction_cbprefix[0x00] = rlc_r;
    instruction_cbprefix[0x01] = rlc_r;
    instruction_cbprefix[0x02] = rlc_r;
    instruction_cbprefix[0x03] = rlc_r;
    instruction_cbprefix[0x04] = rlc_r;
    instruction_cbprefix[0x05] = rlc_r;
    instruction_cbprefix[0x06] = rlc_hl;
    instruction_cbprefix[0x07] = rlc_a;
    instruction_cbprefix[0x08] = rrc_r;
    instruction_cbprefix[0x09] = rrc_r;
    instruction_cbprefix[0x0A] = rrc_r;
    instruction_cbprefix[0x0B] = rrc_r;
    instruction_cbprefix[0x0C] = rrc_r;
    instruction_cbprefix[0x0D] = rrc_r;
    instruction_cbprefix[0x0E] = rrc_hl;
    instruction_cbprefix[0x0F] = rrc_a;

    instruction_cbprefix[0x10] = rl_r;
    instruction_cbprefix[0x11] = rl_r;
    instruction_cbprefix[0x12] = rl_r;
    instruction_cbprefix[0x13] = rl_r;
    instruction_cbprefix[0x14] = rl_r;
    instruction_cbprefix[0x15] = rl_r;
    instruction_cbprefix[0x16] = rl_hl;
    instruction_cbprefix[0x17] = rl_a;
    instruction_cbprefix[0x18] = rr_r;
    instruction_cbprefix[0x19] = rr_r;
    instruction_cbprefix[0x1A] = rr_r;
    instruction_cbprefix[0x1B] = rr_r;
    instruction_cbprefix[0x1C] = rr_r;
    instruction_cbprefix[0x1D] = rr_r;
    instruction_cbprefix[0x1E] = rr_hl;
    instruction_cbprefix[0x1F] = rr_a;

    instruction_cbprefix[0x20] = sla_r;
    instruction_cbprefix[0x21] = sla_r;
    instruction_cbprefix[0x22] = sla_r;
    instruction_cbprefix[0x23] = sla_r;
    instruction_cbprefix[0x24] = sla_r;
    instruction_cbprefix[0x25] = sla_r;
    instruction_cbprefix[0x26] = sla_hl;
    instruction_cbprefix[0x27] = sla_r;
    instruction_cbprefix[0x28] = sra_r;
    instruction_cbprefix[0x29] = sra_r;
    instruction_cbprefix[0x2A] = sra_r;
    instruction_cbprefix[0x2B] = sra_r;
    instruction_cbprefix[0x2C] = sra_r;
    instruction_cbprefix[0x2D] = sra_r;
    instruction_cbprefix[0x2E] = sra_hl;
    instruction_cbprefix[0x2F] = sra_r;

    instruction_cbprefix[0x30] = swap_r;
    instruction_cbprefix[0x31] = swap_r;
    instruction_cbprefix[0x32] = swap_r;
    instruction_cbprefix[0x33] = swap_r;
    instruction_cbprefix[0x34] = swap_r;
    instruction_cbprefix[0x35] = swap_r;
    instruction_cbprefix[0x36] = swap_hl;
    instruction_cbprefix[0x37] = swap_r;
    instruction_cbprefix[0x38] = srl_r;
    instruction_cbprefix[0x39] = srl_r;
    instruction_cbprefix[0x3A] = srl_r;
    instruction_cbprefix[0x3B] = srl_r;
    instruction_cbprefix[0x3C] = srl_r;
    instruction_cbprefix[0x3D] = srl_r;
    instruction_cbprefix[0x3E] = srl_hl;
    instruction_cbprefix[0x3F] = srl_r;

    instruction_cbprefix[0x40] = bit_nr;
    instruction_cbprefix[0x41] = bit_nr;
    instruction_cbprefix[0x42] = bit_nr;
    instruction_cbprefix[0x43] = bit_nr;
    instruction_cbprefix[0x44] = bit_nr;
    instruction_cbprefix[0x45] = bit_nr;
    instruction_cbprefix[0x46] = bit_nhl;
    instruction_cbprefix[0x47] = bit_nr;
    instruction_cbprefix[0x48] = bit_nr;
    instruction_cbprefix[0x49] = bit_nr;
    instruction_cbprefix[0x4A] = bit_nr;
    instruction_cbprefix[0x4B] = bit_nr;
    instruction_cbprefix[0x4C] = bit_nr;
    instruction_cbprefix[0x4D] = bit_nr;
    instruction_cbprefix[0x4E] = bit_nhl;
    instruction_cbprefix[0x4F] = bit_nr;

    instruction_cbprefix[0x50] = bit_nr;
    instruction_cbprefix[0x51] = bit_nr;
    instruction_cbprefix[0x52] = bit_nr;
    instruction_cbprefix[0x53] = bit_nr;
    instruction_cbprefix[0x54] = bit_nr;
    instruction_cbprefix[0x55] = bit_nr;
    instruction_cbprefix[0x56] = bit_nhl;
    instruction_cbprefix[0x57] = bit_nr;
    instruction_cbprefix[0x58] = bit_nr;
    instruction_cbprefix[0x59] = bit_nr;
    instruction_cbprefix[0x5A] = bit_nr;
    instruction_cbprefix[0x5B] = bit_nr;
    instruction_cbprefix[0x5C] = bit_nr;
    instruction_cbprefix[0x5D] = bit_nr;
    instruction_cbprefix[0x5E] = bit_nhl;
    instruction_cbprefix[0x5F] = bit_nr;


    instruction_cbprefix[0x60] = bit_nr;
    instruction_cbprefix[0x61] = bit_nr;
    instruction_cbprefix[0x62] = bit_nr;
    instruction_cbprefix[0x63] = bit_nr;
    instruction_cbprefix[0x64] = bit_nr;
    instruction_cbprefix[0x65] = bit_nr;
    instruction_cbprefix[0x66] = bit_nhl;
    instruction_cbprefix[0x67] = bit_nr;
    instruction_cbprefix[0x68] = bit_nr;
    instruction_cbprefix[0x69] = bit_nr;
    instruction_cbprefix[0x6A] = bit_nr;
    instruction_cbprefix[0x6B] = bit_nr;
    instruction_cbprefix[0x6C] = bit_nr;
    instruction_cbprefix[0x6D] = bit_nr;
    instruction_cbprefix[0x6E] = bit_nhl;
    instruction_cbprefix[0x6F] = bit_nr;

    instruction_cbprefix[0x70] = bit_nr;
    instruction_cbprefix[0x71] = bit_nr;
    instruction_cbprefix[0x72] = bit_nr;
    instruction_cbprefix[0x73] = bit_nr;
    instruction_cbprefix[0x74] = bit_nr;
    instruction_cbprefix[0x75] = bit_nr;
    instruction_cbprefix[0x76] = bit_nhl;
    instruction_cbprefix[0x77] = bit_nr;
    instruction_cbprefix[0x78] = bit_nr;
    instruction_cbprefix[0x79] = bit_nr;
    instruction_cbprefix[0x7A] = bit_nr;
    instruction_cbprefix[0x7B] = bit_nr;
    instruction_cbprefix[0x7C] = bit_nr;
    instruction_cbprefix[0x7D] = bit_nr;
    instruction_cbprefix[0x7E] = bit_nhl;
    instruction_cbprefix[0x7F] = bit_nr;

    instruction_cbprefix[0x80] = res_nr;
    instruction_cbprefix[0x81] = res_nr;
    instruction_cbprefix[0x82] = res_nr;
    instruction_cbprefix[0x83] = res_nr;
    instruction_cbprefix[0x84] = res_nr;
    instruction_cbprefix[0x85] = res_nr;
    instruction_cbprefix[0x86] = res_nhl;
    instruction_cbprefix[0x87] = res_nr;
    instruction_cbprefix[0x88] = res_nr;
    instruction_cbprefix[0x89] = res_nr;
    instruction_cbprefix[0x8A] = res_nr;
    instruction_cbprefix[0x8B] = res_nr;
    instruction_cbprefix[0x8C] = res_nr;
    instruction_cbprefix[0x8D] = res_nr;
    instruction_cbprefix[0x8E] = res_nhl;
    instruction_cbprefix[0x8F] = res_nr;

    instruction_cbprefix[0x90] = res_nr;
    instruction_cbprefix[0x91] = res_nr;
    instruction_cbprefix[0x92] = res_nr;
    instruction_cbprefix[0x93] = res_nr;
    instruction_cbprefix[0x94] = res_nr;
    instruction_cbprefix[0x95] = res_nr;
    instruction_cbprefix[0x96] = res_nhl;
    instruction_cbprefix[0x97] = res_nr;
    instruction_cbprefix[0x98] = res_nr;
    instruction_cbprefix[0x99] = res_nr;
    instruction_cbprefix[0x9A] = res_nr;
    instruction_cbprefix[0x9B] = res_nr;
    instruction_cbprefix[0x9C] = res_nr;
    instruction_cbprefix[0x9D] = res_nr;
    instruction_cbprefix[0x9E] = res_nhl;
    instruction_cbprefix[0x9F] = res_nr;

    instruction_cbprefix[0x90] = res_nr;
    instruction_cbprefix[0x91] = res_nr;
    instruction_cbprefix[0x92] = res_nr;
    instruction_cbprefix[0x93] = res_nr;
    instruction_cbprefix[0x94] = res_nr;
    instruction_cbprefix[0x95] = res_nr;
    instruction_cbprefix[0x96] = res_nhl;
    instruction_cbprefix[0x97] = res_nr;
    instruction_cbprefix[0x98] = res_nr;
    instruction_cbprefix[0x99] = res_nr;
    instruction_cbprefix[0x9A] = res_nr;
    instruction_cbprefix[0x9B] = res_nr;
    instruction_cbprefix[0x9C] = res_nr;
    instruction_cbprefix[0x9D] = res_nr;
    instruction_cbprefix[0x9E] = res_nhl;
    instruction_cbprefix[0x9F] = res_nr;

    instruction_cbprefix[0xA0] = res_nr;
    instruction_cbprefix[0xA1] = res_nr;
    instruction_cbprefix[0xA2] = res_nr;
    instruction_cbprefix[0xA3] = res_nr;
    instruction_cbprefix[0xA4] = res_nr;
    instruction_cbprefix[0xA5] = res_nr;
    instruction_cbprefix[0xA6] = res_nhl;
    instruction_cbprefix[0xA7] = res_nr;
    instruction_cbprefix[0xA8] = res_nr;
    instruction_cbprefix[0xA9] = res_nr;
    instruction_cbprefix[0xAA] = res_nr;
    instruction_cbprefix[0xAB] = res_nr;
    instruction_cbprefix[0xAC] = res_nr;
    instruction_cbprefix[0xAD] = res_nr;
    instruction_cbprefix[0xAE] = res_nhl;
    instruction_cbprefix[0xAF] = res_nr;

    instruction_cbprefix[0xB0] = res_nr;
    instruction_cbprefix[0xB1] = res_nr;
    instruction_cbprefix[0xB2] = res_nr;
    instruction_cbprefix[0xB3] = res_nr;
    instruction_cbprefix[0xB4] = res_nr;
    instruction_cbprefix[0xB5] = res_nr;
    instruction_cbprefix[0xB6] = res_nhl;
    instruction_cbprefix[0xB7] = res_nr;
    instruction_cbprefix[0xB8] = res_nr;
    instruction_cbprefix[0xB9] = res_nr;
    instruction_cbprefix[0xBA] = res_nr;
    instruction_cbprefix[0xBB] = res_nr;
    instruction_cbprefix[0xBC] = res_nr;
    instruction_cbprefix[0xBD] = res_nr;
    instruction_cbprefix[0xBE] = res_nhl;
    instruction_cbprefix[0xBF] = res_nr;

    instruction_cbprefix[0xC0] = set_nr;
    instruction_cbprefix[0xC1] = set_nr;
    instruction_cbprefix[0xC2] = set_nr;
    instruction_cbprefix[0xC3] = set_nr;
    instruction_cbprefix[0xC4] = set_nr;
    instruction_cbprefix[0xC5] = set_nr;
    instruction_cbprefix[0xC6] = set_nr;
    instruction_cbprefix[0xC7] = set_nhl;
    instruction_cbprefix[0xC8] = set_nr;
    instruction_cbprefix[0xC9] = set_nr;
    instruction_cbprefix[0xCA] = set_nr;
    instruction_cbprefix[0xCB] = set_nr; // cb prefixed
    instruction_cbprefix[0xCC] = set_nr;
    instruction_cbprefix[0xCD] = set_nr;
    instruction_cbprefix[0xCE] = set_nhl;
    instruction_cbprefix[0xCF] = set_nr;

    instruction_cbprefix[0xD0] = set_nr;
    instruction_cbprefix[0xD1] = set_nr;
    instruction_cbprefix[0xD2] = set_nr;
    instruction_cbprefix[0xD3] = set_nr;
    instruction_cbprefix[0xD4] = set_nr;
    instruction_cbprefix[0xD5] = set_nr;
    instruction_cbprefix[0xD6] = set_nr;
    instruction_cbprefix[0xD7] = set_nhl;
    instruction_cbprefix[0xD8] = set_nr;
    instruction_cbprefix[0xD9] = set_nr;
    instruction_cbprefix[0xDA] = set_nr;
    instruction_cbprefix[0xDB] = set_nr; // cb prefixed
    instruction_cbprefix[0xDC] = set_nr;
    instruction_cbprefix[0xDD] = set_nr;
    instruction_cbprefix[0xDE] = set_nhl;
    instruction_cbprefix[0xDF] = set_nr;

    instruction_cbprefix[0xE0] = set_nr;
    instruction_cbprefix[0xE1] = set_nr;
    instruction_cbprefix[0xE2] = set_nr;
    instruction_cbprefix[0xE3] = set_nr;
    instruction_cbprefix[0xE4] = set_nr;
    instruction_cbprefix[0xE5] = set_nr;
    instruction_cbprefix[0xE6] = set_nr;
    instruction_cbprefix[0xE7] = set_nhl;
    instruction_cbprefix[0xE8] = set_nr;
    instruction_cbprefix[0xE9] = set_nr;
    instruction_cbprefix[0xEA] = set_nr;
    instruction_cbprefix[0xEB] = set_nr; // cb prefixed
    instruction_cbprefix[0xEC] = set_nr;
    instruction_cbprefix[0xED] = set_nr;
    instruction_cbprefix[0xEE] = set_nhl;
    instruction_cbprefix[0xEF] = set_nr;

    instruction_cbprefix[0xF0] = set_nr;
    instruction_cbprefix[0xF1] = set_nr;
    instruction_cbprefix[0xF2] = set_nr;
    instruction_cbprefix[0xF3] = set_nr;
    instruction_cbprefix[0xF4] = set_nr;
    instruction_cbprefix[0xF5] = set_nr;
    instruction_cbprefix[0xF6] = set_nr;
    instruction_cbprefix[0xF7] = set_nhl;
    instruction_cbprefix[0xF8] = set_nr;
    instruction_cbprefix[0xF9] = set_nr;
    instruction_cbprefix[0xFA] = set_nr;
    instruction_cbprefix[0xFB] = set_nr; // cb prefixed
    instruction_cbprefix[0xFC] = set_nr;
    instruction_cbprefix[0xFD] = set_nr;
    instruction_cbprefix[0xFE] = set_nhl;
    instruction_cbprefix[0xFF] = set_nr;
}

void Gameboy::step(){
    // update timer
    // instr

    // first instr at 0 (booting)

    // first instr for cartridges at 0x0100
    
    uint8_t cycles = 0;
    
    // fetch instr
    opcode = readByte();

    // decode and execute instr
    execInstruction();

    t_cycles += cycles;

    // check for interrupts
    /*
        Interrupt           ISR address
        vertical bank       0x0040
        lcd status triggers 0x0048
        timer overflow      0x0050
        serial link         0x0058
        joypad press        0x0060
    */



}

uint8_t Gameboy::readByte(){
    return M[pc++];
}

void Gameboy::execInstruction(){
    cout << hex << setfill('0') << setw(2) << (int) opcode << endl;
    if(opcode == 0xCB){
        opcode = readByte();
        //(this->*instruction_cbprefix[opcode])();
    } else{
        //(this->*instruction_noprefix[opcode])();
    }
    cout << "exec: " << hex << setfill('0') << (int) opcode << endl;
}
bool Gameboy::checkCondition(uint8_t cc){
    /*
        0 NZ
        1 Z
        2 NC
        3 C
    */

    bool z = checkZero(), c = checkCarry();

    return (cc == 0 && !z) || (cc == 1 && z) || 
        (cc == 2 && !c) || (cc == 3 && c);
}

bool Gameboy::checkZero(){
    return (R[F] >> 7u); // return bit 7
}

bool Gameboy::checkN(){
    return (R[F] >> 6u) & 0x1; // return bit 6
}

bool Gameboy::checkH(){
    return (R[F] >> 5u) & 0x1; // return bit 5
}

bool Gameboy::checkCarry(){
    return (R[F] >> 4u) & 0x1; // return bit 4
}

void Gameboy::setZero(bool z){

    if(z){
        R[F] |= 0x80;
    } else{
        R[F] &= 0x7F;
    }

}

void Gameboy::setN(bool n){
    if(n){
        R[F] |= 0x40;
    } else{
        R[F] &= 0xBF;
    }
}

void Gameboy::setH(bool h){
    if(h){
        R[F] |= 0x20;
    } else{
        R[F] &= 0xDF;
    }
}

void Gameboy::setCarry(bool c){
    
    if(c){
        R[F] |= 0x10;
    } else{
        R[F] &= 0xEF;
    }

}

bool checkHAdd(int16_t a, int16_t b, bool c){
    return ( (a & 0x0F) + (b & 0x0F + c) ) > 0x0F;
}

bool checkHSub(int16_t a, int16_t b, bool c){
    return (a & 0x0F) < (b & 0x0F + c);
}

bool checkC8Add(int8_t a, int8_t b, bool c){
    return ( a > 0 && (b + c) > (INT8_MAX - a) ) ;
}

bool checkC8Sub(int8_t a, int8_t b, bool c){
    return a < (b + c);
}

bool checkC16Add(int16_t a, int16_t b, bool c){
    return ( a > 0 && (b + c) > (INT16_MAX - a) );
}

bool checkC16Sub(int16_t a, int16_t b, bool c){
    return a < (b + c);
}

void op_null(){
    // throw err
}

// LOAD Instructions

// 8 bit load instructions

// assume registers 0 - 7 correspond to B, C, D, E, H, L, A, F
uint8_t Gameboy::ld_rr(){
    uint8_t r1 = (opcode >> 3u) & 0x7;
    uint8_t r2 = opcode & 0x7;

    R[r1] = R[r2];

    return 4; // num t-cycles
}

uint8_t Gameboy::ld_rn(){
    uint8_t r = (opcode >> 3u) & 0x7;
    uint8_t n = readByte();

    R[r] = n;

    return 8; 
}

uint8_t Gameboy::ld_rhl(){
    uint8_t r = (opcode >> 3u) & 0x7;
    uint16_t hl = (R[H] << 8u) | R[L];

    R[r] = M[hl];

    return 8;
}

uint8_t Gameboy::ld_hlr(){
    uint8_t r = (opcode >> 3u) & 0x7;
    uint16_t hl = (R[H] << 8u) | R[L];

    M[hl] = R[r];

    return 8;
}

uint8_t Gameboy::ld_hln(){
    uint16_t hl = (R[H] << 8u) | R[L];
    uint8_t n = readByte();

    M[hl] = M[n];

    return 12;
}

uint8_t Gameboy::ld_abc(){
    uint16_t bc = (R[B] << 8u) | R[C];

    R[A] = M[bc];

    return 8;
}

uint8_t Gameboy::ld_ade(){
    uint16_t de = (R[D] << 8u) | R[E];

    R[A] = M[de];

    return 8;
}

uint8_t Gameboy::ld_ann(){
    uint8_t lsb = readByte();
    uint8_t msb = readByte();

    uint16_t nn = (msb << 8u) | lsb;

    R[A] = M[nn];

    return 16;
}

uint8_t Gameboy::ld_bca(){
    uint16_t bc = (R[B] << 8u) | R[C];

    M[bc] = R[A];

    return 8;
}

uint8_t Gameboy::ld_dea(){
    uint16_t de = (R[D] << 8u) | R[E];

    M[de] = R[A];

    return 8;
}

uint8_t Gameboy::ld_nna(){
    uint8_t lsb = readByte();
    uint8_t msb = readByte();

    uint16_t nn = (msb << 8u) | lsb;

    M[nn] = R[A];

    return 16;
}

uint8_t Gameboy::ldh_an(){
    uint8_t n = readByte();
    uint16_t s = 0xFF00 | n;

    R[A] = M[s];

    return 12;
}

uint8_t Gameboy::ldh_na(){
    uint8_t n = readByte();
    uint16_t d = 0xFF00 | n;

    M[d] = R[A];

    return 12;
}

uint8_t Gameboy::ldh_ac(){
    uint16_t s = 0xFF00 | R[C];

    R[A] = R[s];

    return 8;
}

uint8_t Gameboy::ldh_ca(){
    uint16_t d = 0xFF00 | R[C];

    M[d] = R[A];

    return 8;
}

uint8_t Gameboy::ldd_ahl(){
    uint16_t hl = (R[H] << 8u) | R[L];

    R[A] = M[hl];

    hl -= 1;

    R[H] = hl >> 8u;
    R[L] = (uint8_t) hl; // first byte truncated

    return 8;
}

uint8_t Gameboy::ldd_hla(){
    uint16_t hl = (R[H] << 8u) | R[L];

    M[hl] = R[A];

    hl -= 1;

    R[H] = hl >> 8u;
    R[L] = (uint8_t) hl; // first byte truncated

    return 8;
}

uint8_t Gameboy::ldi_ahl(){
    uint16_t hl = (R[H] << 8u) | R[L];

    R[A] = M[hl];

    hl -= 1;

    R[H] = hl >> 8u;
    R[L] = (uint8_t) hl; // first byte truncated

    return 8;
}

uint8_t Gameboy::ldi_hla(){
    uint16_t hl = (R[H] << 8u) | R[L];

    M[hl] = R[A];

    hl -= 1;

    R[H] = hl >> 8u;
    R[L] = (uint8_t) hl; // first byte truncated

    return 8;
}

// 16 bit load instructions

uint8_t Gameboy::ld_rrnn(){
    uint8_t rr = (opcode >> 4u);
    // 0, 1, 2, 3 corresponds to BC, DE, HL, SP

    uint8_t lsb = readByte();
    uint8_t msb = readByte();

    switch(rr){
        case 0:
            R[B] = msb;
            R[C] = lsb;
            break;
        case 1:
            R[D] = msb;
            R[E] = lsb;
            break;
        case 2:
            R[H] = msb;
            R[L] = lsb;
            break;
        case 3:
            sp = (msb << 8u) | lsb;
    }

    return 12;
}

uint8_t Gameboy::ld_nnsp(){
    uint8_t lsb = readByte();
    uint8_t msb = readByte();

    uint16_t nn = (msb << 8u) | lsb;

    M[nn] = (uint8_t) sp;
    M[nn + 1] = (sp >> 8u);    

    return 20;
}

uint8_t Gameboy::ld_sphl(){
    sp = (R[H] << 8u) | R[L];

    return 8;
}

uint8_t Gameboy::push_rr(){
    uint8_t rr = (opcode >> 4u) & 0x3;
    // 0, 1, 2, 3 corresponds to bc, de, hl, af
    sp -= 1;

    uint8_t msb, lsb;

    switch(rr){
        case 0:
            msb = R[B];
            lsb = R[C];
            break;
        case 1:
            msb = R[D];
            lsb = R[E];
            break;
        case 2:
            msb = R[H];
            lsb = R[L];
            break;
        case 3:
            msb = R[A];
            lsb = R[F];
    }

    M[sp] = msb;
    sp -= 1;
    M[sp] = lsb;

    return 16;
}

uint8_t Gameboy::pop_rr(){
    uint8_t rr = (opcode >> 4u) & 0x3;

    uint8_t lsb = M[sp];
    sp += 1;

    uint8_t msb = M[sp];
    sp += 1;

    switch(rr){
        case 0:
            R[B] = msb;
            R[C] = lsb;
            break;
        case 1:
            R[D] = msb;
            R[E] = lsb;
            break;
        case 2:
            R[H] = msb;
            R[L] = lsb;
            break;
        case 3:
            R[A] = msb;
            R[F] = lsb;
    }

    return 12;
}

// ARITHMETIC instructions

// 8 bit ARITHMETIC
// FIXME CHECK FLAGS

uint8_t Gameboy::add_ar(){

    uint8_t r = opcode & 0x0F;

    setN(0);
    setH( checkHAdd(R[A], R[r], 0) );
    setCarry( checkC8Add(R[A], R[r], 0) );

    R[A] += R[r];

    setZero(R[A] == 0);

    return 4;
}

uint8_t Gameboy::add_an(){
    
    uint8_t n = readByte();

    setN(0);
    setH( checkHAdd(R[A], n, 0) );
    setCarry( checkC8Add(R[A], n, 0) );

    R[A] += n;

    setZero(R[A] == 0);

    return 8;
}

uint8_t Gameboy::add_ahl(){

    uint16_t hl = (R[H] << 8u) | R[L];

    setN(0);
    setH( checkHAdd(R[A], M[hl], 0) );
    setCarry( checkC8Add(R[A], M[hl], 0) );

    R[A] += M[hl];

    setZero(R[A] == 0);

    return 8;
}

uint8_t Gameboy::adc_ar(){

    uint8_t r = opcode & 0x0F;
    bool c = checkCarry();

    setN(0);
    setH( checkHAdd(R[A], R[r], c) );
    setCarry( checkC8Add(R[A], R[r], c) );

    R[A] += R[r] + c;

    setZero(R[A] == 0);

    return 4;
}

uint8_t Gameboy::adc_an(){

    uint8_t n = readByte();

    bool c = checkCarry();

    setN(0);
    setH( checkHAdd(R[A], n, c) );
    setCarry( checkC8Add(R[A], n, c) );

    R[A] += n + c;

    setZero(R[A] == 0);

    return 8;
}

uint8_t Gameboy::adc_ahl(){

    uint16_t hl = (R[H] << 8u) | R[L];

    bool c = checkCarry();

    setN(0);
    setH( checkHAdd(R[A], M[hl], c) );
    setCarry( checkC8Add(R[A], M[hl], c) );

    R[A] += M[hl] + c;

    setZero(R[A] == 0);

    return 8;
}

uint8_t Gameboy::sub_ar(){
    // a - b = a + -b

    uint8_t r = opcode & 0x0F;
    
    setN(1);
    setH( checkHSub(R[A], R[r], 0) );
    setCarry( checkC8Sub(R[A], R[r], 0) );

    R[A] -= r;

    setZero(R[A] == 0);

    return 4;
}

uint8_t Gameboy::sub_an(){
    
    uint8_t n = readByte();

    setN(1);
    setH( checkHSub(R[A], n, 0) );
    setCarry( checkC8Sub(R[A], n, 0) );

    R[A] -= n;

    setZero(R[A] == 0);

    return 8;
}

uint8_t Gameboy::sub_ahl(){

    uint16_t hl = (R[H] << 8u) | R[L];

    setN(0);
    setH( checkHSub(R[A], M[hl], 0) );
    setCarry( checkC8Sub(R[A], M[hl], 0) );

    R[A] -= M[hl];

    setZero(R[A] == 0);

    return 8;
}

uint8_t Gameboy::sbc_ar(){

    uint8_t r = opcode & 0x0F;

    bool c = checkCarry();

    setN(1);
    setH( checkHSub(R[A], R[r], c) );
    setCarry( checkC8Sub(R[A], R[r], c) );

    R[A] -= (R[r] + c);

    setZero(R[A] == 0);

    return 4;
}

uint8_t Gameboy::sbc_an(){

    uint8_t n = readByte();

    bool c = checkCarry();

    setN(1);
    setH( checkHSub(R[A], n, c) );
    setCarry( checkC8Sub(R[A], n, c) );

    R[A] -= (n + c);

    setZero(R[A] == 0);

    return 8;
}

uint8_t Gameboy::sbc_ahl(){

    uint16_t hl = (R[H] << 8u) | R[L];

    bool c = checkCarry();

    setN(1);
    setH( checkHSub(R[A], M[hl], c) );
    setCarry( checkC8Sub(R[A], M[hl], c) );

    R[A] -= (M[hl] + c);

    setZero(R[A] == 0);

    return 8;
}

uint8_t Gameboy::and_ar(){

    uint8_t r = opcode & 0x0F;

    R[A] &= R[r];

    setZero(R[A] == 0);
    setN(0);
    setH(1);
    setCarry(0);

    return 4;
}

uint8_t Gameboy::and_an(){

    uint8_t n = readByte();

    R[A] &= n;

    setZero(R[A] == 0);
    setN(0);
    setH(1);
    setCarry(0);

    return 8;
}

uint8_t Gameboy::and_ahl(){

    uint16_t hl = (R[H] << 8u) | R[L];

    R[A] &= M[hl];

    setZero(R[A] == 0);
    setN(0);
    setH(1);
    setCarry(0);

    return 8;
}

uint8_t Gameboy::xor_ar(){

    uint8_t r = opcode & 0x0F;

    R[A] ^= R[r];

    setZero(R[A] == 0);
    setN(0);
    setH(0);
    setCarry(0);

    return 4;
}

uint8_t Gameboy::xor_an(){

    uint8_t n = readByte();

    R[A] ^= n;

    setZero(R[A] == 0);
    setN(0);
    setH(0);
    setCarry(0);

    return 8;
}

uint8_t Gameboy::xor_ahl(){

    uint16_t hl = (R[H] << 8u) | R[L];

    R[A] ^= M[hl];

    setZero(R[A] == 0);
    setN(0);
    setH(0);
    setCarry(0);

    return 8;
}

uint8_t Gameboy::or_ar(){

    uint8_t r = opcode & 0x0F;

    R[A] |= R[r];

    setZero(R[A] == 0);
    setN(0);
    setH(0);
    setCarry(0);

    return 4;
}

uint8_t Gameboy::or_an(){

    uint8_t n = readByte();

    R[A] |= n;

    setZero(R[A] == 0);
    setN(0);
    setH(0);
    setCarry(0);

    return 8;
}

uint8_t Gameboy::or_ahl(){

    uint16_t hl = (R[H] << 8u) | R[L];

    R[A] |= M[hl];

    setZero(R[A] == 0);
    setN(0);
    setH(0);
    setCarry(0);

    return 8;
}

uint8_t Gameboy::cp_ar(){

    uint8_t r = opcode & 0x0F;

    setN(1);
    setH( checkHSub(R[A], R[r], 0) );
    setCarry( checkC8Sub(R[A], R[r], 0) );
    setZero(R[A] - R[r] == 0);

    return 4;
}

uint8_t Gameboy::cp_an(){

    uint8_t n = readByte();

    setN(1);
    setH( checkHSub(R[A], n, 0) );
    setCarry( checkC8Sub(R[A], n, 0) );
    setZero(R[A] - n == 0);

    return 8;
}

uint8_t Gameboy::cp_ahl(){

    uint16_t hl = (R[H] << 8u) | R[L];

    setN(1);
    setH( checkHSub(R[A], M[hl], 0) );
    setCarry( checkC8Sub(R[A], M[hl], 0) );
    setZero(R[A] - M[hl] == 0);

    return 8;
}

uint8_t Gameboy::inc_r(){

    uint8_t r = opcode >> 3u;

    setN(0);
    setH( checkHAdd(R[r], 1, 0) );

    R[r] += 1;

    setZero(R[r] == 0);

    return 4;
}

uint8_t Gameboy::inc_hl(){

    uint16_t hl = (R[H] << 8u) | R[L];

    setN(0);
    setH( checkC8Add(M[hl], 1, 0) );

    M[hl] += 1;

    setZero(M[hl] == 0);

    return 12;
}

uint8_t Gameboy::dec_r(){

    uint8_t r = opcode >> 3u;

    setN(1);
    setH( checkHSub(R[r], 1, 0) );

    R[r] -= 1;

    setZero(R[r] == 0);

    return 4;
}

uint8_t Gameboy::dec_hl(){

    uint16_t hl = (R[H] << 8u) | R[L];

    setN(1);
    setH( checkHSub(M[hl], 1, 0) );

    M[hl] -= 1;

    setZero(M[hl] == 0);

    return 12;
}

uint8_t Gameboy::daa(){

    // decimal adjust a
    return 4;
}

uint8_t Gameboy::cpl(){

    R[A] ^= 0xFF; // complement A

    setN(1);
    setH(1);

    return 4;
}

// 16 bit arithmetic
uint8_t Gameboy::add_hlrr(){
        
    // rr may be BC = 0 DE = 1 HL = 2 SP = 3
    uint16_t rr;

    switch(opcode >> 4u){
        case 0:
            rr = (R[B] << 8u) | R[C];
        case 1:
            rr = (R[D] << 8u) | R[E];
        case 2:
            rr = (R[H] << 8u) | R[L];
        case 3:
            rr = sp;
    }

    uint16_t hl = (R[H] << 8u) | R[L];

    setN(0);
    setH( checkHAdd(hl, rr, 0) );
    setCarry( checkC16Add(hl, rr, 0) );

    hl += rr;

    R[H] = hl >> 8u;
    R[L] = (uint8_t) hl;

    return 8;
}

uint8_t Gameboy::inc_rr(){
    if(opcode >> 4u == 3){
        sp += 1;
        return 8;
    }

    // rr may be BC = 0 DE = 1 HL = 2 SP = 3
    uint8_t r1, r2;

    switch(opcode >> 4u){
        case 0:
            r1 = B, r2 = C;
            break;
        case 1:
            r1 = D, r2 = E;
            break;
        case 2:
            r1 = H, r2 = L;
    }

    uint16_t rr = (R[r1] << 8u) | R[r2];
    rr += 1;

    R[r1] = rr >> 8u;
    R[r2] = (uint8_t) rr;

    return 8;
}

uint8_t Gameboy::dec_rr(){
    if(opcode >> 4u == 3){
        sp -= 1;
        return 8;
    }

    // rr may be BC = 0 DE = 1 HL = 2 SP = 3
    uint8_t r1, r2;

    switch(opcode >> 4u){
        case 0:
            r1 = B, r2 = C;
            break;
        case 1:
            r1 = D, r2 = E;
            break;
        case 2:
            r1 = H, r2 = L;
    }

    uint16_t rr = (R[r1] << 8u) | R[r2];
    rr -= 1;

    R[r1] = rr >> 8u;
    R[r2] = (uint8_t) rr;

    return 8;
}

uint8_t Gameboy::add_spdd(){
    setZero(0);
    setN(0);

    int8_t dd = readByte();

    if(dd < 0){ // subtraction
        setH( checkHSub(sp, (~dd + 1), 0) );
        setCarry( checkC16Sub(sp, (~dd + 1), 0) );
    } else{ // addition
        setH( checkHAdd(sp, dd, 0) );
        setCarry( checkC16Add(sp, dd, 0) );
    }

    sp += dd;

    return 16;
}

uint8_t Gameboy::ld_hlspdd(){
    setZero(0);
    setN(0);

    int8_t dd = readByte();

    if(dd < 0){ // subtraction
        setH( checkHSub(sp, (~dd + 1), 0) );
        setCarry( checkC16Sub(sp, (~dd + 1), 0) );
    } else{ // addition
        setH( checkHAdd(sp, dd, 0) );
        setCarry( checkC16Add(sp, dd, 0) );
    }

    uint16_t res = sp + dd;

    R[H] = res >> 8u;
    R[L] = res; 

    return 12;
}

// END ARITHMETIC instructions

// ROTATE AND SHIFT instructions
void Gameboy::rlc(uint8_t *reg){
    setN(0);
    setH(0);

    setCarry( *reg >> 7u ); // bit 7

    *reg <<= 1u; // shift left 1
    *reg |= checkCarry(); // set bit 0 to carry flag

    setZero(*reg == 0);
}

void Gameboy::rl(uint8_t *reg){
    setN(0);
    setH(0);

    bool c = checkCarry();
    setCarry( *reg >> 7u );

    *reg << 1u;
    *reg |= c; // set bit 0 to prev carry flag

    setZero(*reg == 0);
}

void Gameboy::rrc(uint8_t *reg){
    setN(0);
    setH(0);

    setCarry( *reg & 0x1 );

    *reg >>= 1u;
    *reg |= (checkCarry() << 7u);

    setZero(*reg == 0);
}

void Gameboy::rr(uint8_t *reg){
    setN(0);
    setH(0);

    bool c = checkCarry();
    setCarry( *reg & 0x1 );

    *reg >>= 1u;
    *reg |= (c << 7u);

    setZero(*reg == 0);
}

void Gameboy::sla(uint8_t *reg){
    setN(0);
    setH(0);
    setCarry(*reg >> 7u);

    *reg <<= 1u;

    setZero(*reg == 0);
}

void Gameboy::swap(uint8_t *reg){
    setN(0);
    setH(0);
    setCarry(0);

    uint8_t lsn = *reg & 0b00001111;
    uint8_t msn = (*reg & 0b11110000) >> 4u;

    *reg = (lsn << 4u) | msn;

    setZero(*reg == 0);
}

void Gameboy::sra(uint8_t *reg){
    setN(0);
    setH(0);
    setCarry(*reg & 0x1);

    bool b7 = *reg & 0b10000000;

    *reg >>= 1u;
    *reg |= (b7 << 7u);

    setZero(*reg == 0);
}

void Gameboy::srl(uint8_t *reg){
    setN(0);
    setH(0);

    setCarry(*reg & 0x1);

    *reg >>= 1u;

    setZero(*reg == 0);
}

uint8_t Gameboy::rlc_a(){
    rlc( &R[A] );
    setZero(0);

    return 4;
}

uint8_t Gameboy::rl_a(){
    rl( &R[A] );
    setZero(0);

    return 4;
}

uint8_t Gameboy::rrc_a(){
    rrc( &R[A] );
    setZero(0);

    return 4;
}

uint8_t Gameboy::rr_a(){
    rr( &R[A] );
    setZero(0);

    return 4;
}

uint8_t Gameboy::rlc_r(){
    uint8_t r = opcode;

    rlc( &R[r] );

    return 8;
}

uint8_t Gameboy::rlc_hl(){
    

    uint16_t hl = (R[H] << 8u) | R[L];

    rlc( &M[hl] );

    return 16;
}

uint8_t Gameboy::rl_r(){
    uint8_t r = opcode & 0x0F;

    rl( &R[r] );

    return 8;
}

uint8_t Gameboy::rl_hl(){
    

    uint16_t hl = (R[H] << 8u) | R[L];

    rl( &M[hl] );

    return 16;
}

uint8_t Gameboy::rrc_r(){
    uint8_t r = opcode & 0x0F;

    rrc( &R[r] );

    return 8;
}

uint8_t Gameboy::rrc_hl(){
    

    uint16_t hl = (R[H] << 8u) | R[L];

    rrc( &M[hl] );

    return 16;
}

uint8_t Gameboy::rr_r(){
    uint8_t r = opcode & 0x0F;

    rr( &R[r] );

    return 8;
}

uint8_t Gameboy::rr_hl(){
    

    uint16_t hl = (R[H] << 8u) | R[L];

    rr( &M[hl] );

    return 16;
}

uint8_t Gameboy::sla_r(){
    uint8_t r = opcode & 0x0F;

    sla( &R[r] );

    return 8;
}

uint8_t Gameboy::sla_hl(){
    

    uint16_t hl = (R[H] << 8u) | R[L];

    sla( &M[hl] );

    return 16;
}

uint8_t Gameboy::swap_r(){
    uint8_t r = opcode & 0x0F;

    swap( &R[r] );

    return 8;
}

uint8_t Gameboy::swap_hl(){
    

    uint16_t hl = (R[H] << 8u) | R[L];

    swap( &M[hl] );

    return 16;
}

uint8_t Gameboy::sra_r(){
    uint8_t r = opcode & 0x0F;

    sra( &R[r] );

    return 8;
}

uint8_t Gameboy::sra_hl(){
    

    uint16_t hl = (R[H] << 8u) | R[L];

    sra( &M[hl] );

    return 16;
}

uint8_t Gameboy::srl_r(){
    uint8_t r = opcode & 0x0F;

    srl( &R[r] );

    return 8;
}

uint8_t Gameboy::srl_hl(){
    uint16_t hl = (R[H] << 8u) | R[L];

    srl( &M[hl] );

    return 16;
}

// END ROTATE AND SHIFT instructions

// SINGLE BIT OPERATION instructions
uint8_t Gameboy::bit_nr(){
    setN(0);
    setH(1);

    uint8_t bit = (opcode >> 3u) & 0x7;
    uint8_t r = opcode & 0x7;

    setZero( ( (R[r] >> bit) & 0x1 ) == 0 );

    return 8;
}

uint8_t Gameboy::bit_nhl(){
    setN(0);
    setH(1);

    uint8_t bit = (opcode >> 3u) & 0x7;
    uint16_t hl = (R[H] << 8u) | R[L];

    setZero( ( (M[hl] >> bit) & 0x1 ) == 0 );

    return 12;
}

uint8_t Gameboy::set_nr(){
    uint8_t bit = (opcode >> 3u) & 0x7;
    uint8_t r = opcode & 0x7;

    R[r] |= (0x1 << bit);

    return 8;
}

uint8_t Gameboy::set_nhl(){
    uint8_t bit = (opcode >> 3u) & 0x7;
    uint16_t hl = (R[H] << 8u) | R[L];

    M[hl] |= (0x1 << bit);

    return 16;
}

uint8_t Gameboy::res_nr(){
    uint8_t bit = (opcode >> 3u) & 0x7;
    uint8_t r = opcode & 0x7;

    R[r] &= ( (0x1 << bit) ^ 0xFF );

    return 8;
}

uint8_t Gameboy::res_nhl(){
    uint8_t bit = (opcode >> 3u) & 0x7;
    uint16_t hl = (R[H] << 8u) | R[L];

    M[hl] &= ( (0x1 << bit) ^ 0xFF );

    return 16;
}
// END SINGLE BIT OPERATION instructions

// CPU CONTROL instructions
uint8_t Gameboy::ccf(){
    setN(0);
    setH(0);
    setCarry(checkCarry() ^ 1);

    return 4;
}

uint8_t Gameboy::scf(){
    setN(0);
    setH(0);
    setCarry(1);

    return 4;
}

uint8_t Gameboy::nop(){
    return 4;
}

// uint8_t Gameboy::halt(){
    
// }

// uint8_t Gameboy::stop(){

// }

uint8_t Gameboy::di(){
    IME = 0;
    return 4;
}

uint8_t Gameboy::ei(){
    IME = 1;
    return 4;
}

// END CPU CONTROL instructions

// JUMP instructions

uint8_t Gameboy::jp_nn(){
    uint8_t lsb = readByte();
    uint8_t msb = readByte();

    pc = (msb << 8u) | lsb;

    return 16;
}

uint8_t Gameboy::jp_hl(){
    pc = (R[H] << 8u) | R[L];

    return 4;
}

uint8_t Gameboy::jp_fnn(){
    uint8_t cc = (opcode >> 3u) & 0x3;

    uint8_t lsb = readByte();
    uint8_t msb = readByte();

    if( checkCondition(cc) ){
        pc = (msb << 8u) | lsb;

        return 16;
    }

    return 12;   
}

uint8_t Gameboy::jr_pcdd(){
    int8_t dd = readByte();

    pc += dd;

    return 12;
}

uint8_t Gameboy::jr_fpcdd(){
    uint8_t cc = (opcode >> 3u) & 0x3;

    int8_t dd = readByte();

    if( checkCondition(cc) ){
        pc += dd;

        return 12;
    }

    return 8; 
}

uint8_t Gameboy::call_nn(){
    uint8_t lsb = readByte();
    uint8_t msb = readByte();

    sp -= 1;
    M[sp] = pc >> 8u;

    sp -= 1;
    M[sp] = pc;

    pc = (msb << 8u) | lsb;

    return 24;
}

uint8_t Gameboy::call_fnn(){
    uint8_t cc = (opcode >> 3u) & 0x3;

    uint8_t lsb = readByte();
    uint8_t msb = readByte();

    if( checkCondition(cc) ){
        sp -= 1;
        M[sp] = pc >> 8u;

        sp -= 1;
        M[sp] = pc;

        pc = (msb << 8u) | lsb;

        return 24;
    }

    return 12;
}

uint8_t Gameboy::ret(){
    uint8_t lsb = M[sp];
    sp += 1;

    uint8_t msb = M[sp];
    sp += 1;

    pc = (msb << 8u) | lsb;

    return 16;
}

uint8_t Gameboy::ret_f(){
    uint8_t cc = (opcode >> 3u) & 0x3;

    if( checkCondition(cc) ){
        uint8_t lsb = M[sp];
        sp += 1;

        uint8_t msb = M[sp];
        sp += 1;

        pc = (msb << 8u) | lsb;

        return 20;
    }

    return 8;
}

uint8_t Gameboy::reti(){
    
    IME = 1;
    ret();

    return 16;
}

uint8_t Gameboy::rst_n(){
    uint8_t rstAddress = (opcode >> 3u) & 0b00111000;

    pc = rstAddress;

    return 16;
}

// END JUMP instructions