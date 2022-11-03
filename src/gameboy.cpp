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
    // instruction_noprefix[0x00] = nop;
    // instruction_noprefix[0x01] = ld_rrnn;
    // instruction_noprefix[0x02] = ld_bca;
    // instruction_noprefix[0x03] = inc_rr;
    // instruction_noprefix[0x04] = inc_r;
    // instruction_noprefix[0x05] = dec_r;
    // instruction_noprefix[0x06] = ld_rn;
    // instruction_noprefix[0x07] = rlc_r;
    // instruction_noprefix[0x08] = ld_nnsp;
    // instruction_noprefix[0x09] = add_hlrr;
    // instruction_noprefix[0x0A] = ld_abc;
    // instruction_noprefix[0x0B] = dec_rr;
    // instruction_noprefix[0x0C] = inc_r;
    // instruction_noprefix[0x0D] = dec_r;
    // instruction_noprefix[0x0E] = ld_rn;
    // instruction_noprefix[0x0F] = rrc_r;

    // instruction_noprefix[0x10] = stop;
    // instruction_noprefix[0x11] = ld_rrnn;
    // instruction_noprefix[0x12] = ld_dea;
    // instruction_noprefix[0x13] = inc_rr;
    // instruction_noprefix[0x14] = inc_r;
    // instruction_noprefix[0x15] = dec_r;
    // instruction_noprefix[0x16] = ld_rn;
    // instruction_noprefix[0x17] = rla;
    // instruction_noprefix[0x18]; // implement me
    // instruction_noprefix[0x19] = add_hlrr;
    // instruction_noprefix[0x1A] = ld_ade;
    // instruction_noprefix[0x1B] = dec_rr;
    // instruction_noprefix[0x1C] = inc_r;
    // instruction_noprefix[0x1D] = dec_r;
    // instruction_noprefix[0x1E] = ld_rn;
    // instruction_noprefix[0x1F] = rra;

    // instruction_noprefix[0x20]; // impl me
    // instruction_noprefix[0x21] = ld_rrnn;
    // instruction_noprefix[0x22] = ldi_hla;
    // instruction_noprefix[0x23] = inc_hl;
    // instruction_noprefix[0x24] = inc_r;
    // instruction_noprefix[0x25] = dec_r;
    // instruction_noprefix[0x26] = ld_rn;
    // instruction_noprefix[0x27] = daa;
    // instruction_noprefix[0x28]; // impl me
    // instruction_noprefix[0x29] = add_hlrr;
    // instruction_noprefix[0x2A] = ldi_ahl;
    // instruction_noprefix[0x2B] = dec_rr;
    // instruction_noprefix[0x2C] = inc_r;
    // instruction_noprefix[0x2D] = dec_r;
    // instruction_noprefix[0x2E] = ld_rn;
    // instruction_noprefix[0x2F] = cpl;

    // instruction_noprefix[0x30]; // impl me
    // instruction_noprefix[0x31] = ld_rrnn;
    // instruction_noprefix[0x32] = ldd_hla;
    // instruction_noprefix[0x33] = inc_rr;
    // instruction_noprefix[0x34] = inc_hl;
    // instruction_noprefix[0x35] = dec_hl;
    // instruction_noprefix[0x36] = ld_hln;
    // instruction_noprefix[0x37] = scf;
    // instruction_noprefix[0x38]; // impl me
    // instruction_noprefix[0x39] = add_hlrr;
    // instruction_noprefix[0x3A] = ldd_ahl;
    // instruction_noprefix[0x3B] = dec_rr;
    // instruction_noprefix[0x3C] = inc_r;
    // instruction_noprefix[0x3D] = dec_r;
    // instruction_noprefix[0x3E] = ld_rn;
    // instruction_noprefix[0x3F] = ccf;

    // instruction_noprefix[0x40] = ld_rr;
    // instruction_noprefix[0x41] = ld_rr;
    // instruction_noprefix[0x42] = ld_rr;
    // instruction_noprefix[0x43] = ld_rr;
    // instruction_noprefix[0x44] = ld_rr;
    // instruction_noprefix[0x45] = ld_rr;
    // instruction_noprefix[0x46] = ld_rr;
    // instruction_noprefix[0x47] = ld_rr;
    // instruction_noprefix[0x48] = ld_rr;
    // instruction_noprefix[0x49] = ld_rr;
    // instruction_noprefix[0x4A] = ld_rr;
    // instruction_noprefix[0x4B] = ld_rr;
    // instruction_noprefix[0x4C] = ld_rr;
    // instruction_noprefix[0x4D] = ld_rr;
    // instruction_noprefix[0x4E] = ld_rr;
    // instruction_noprefix[0x4F] = ld_rr;

    // instruction_noprefix[0x50] = ld_rr;
    // instruction_noprefix[0x51] = ld_rr;
    // instruction_noprefix[0x52] = ld_rr;
    // instruction_noprefix[0x53] = ld_rr;
    // instruction_noprefix[0x54] = ld_rr;
    // instruction_noprefix[0x55] = ld_rr;
    // instruction_noprefix[0x56] = ld_rr;
    // instruction_noprefix[0x57] = ld_rr;
    // instruction_noprefix[0x58] = ld_rr;
    // instruction_noprefix[0x59] = ld_rr;
    // instruction_noprefix[0x5A] = ld_rr;
    // instruction_noprefix[0x5B] = ld_rr;
    // instruction_noprefix[0x5C] = ld_rr;
    // instruction_noprefix[0x5D] = ld_rr;
    // instruction_noprefix[0x5E] = ld_rr;
    // instruction_noprefix[0x5F] = ld_rr;

    // instruction_noprefix[0x60] = ld_rr;
    // instruction_noprefix[0x61] = ld_rr;
    // instruction_noprefix[0x62] = ld_rr;
    // instruction_noprefix[0x63] = ld_rr;
    // instruction_noprefix[0x64] = ld_rr;
    // instruction_noprefix[0x65] = ld_rr;
    // instruction_noprefix[0x66] = ld_rr;
    // instruction_noprefix[0x67] = ld_rr;
    // instruction_noprefix[0x68] = ld_rr;
    // instruction_noprefix[0x69] = ld_rr;
    // instruction_noprefix[0x6A] = ld_rr;
    // instruction_noprefix[0x6B] = ld_rr;
    // instruction_noprefix[0x6C] = ld_rr;
    // instruction_noprefix[0x6D] = ld_rr;
    // instruction_noprefix[0x6E] = ld_rr;
    // instruction_noprefix[0x6F] = ld_rr;

    // instruction_noprefix[0x70] = ld_rr;
    // instruction_noprefix[0x71] = ld_rr;
    // instruction_noprefix[0x72] = ld_rr;
    // instruction_noprefix[0x73] = ld_rr;
    // instruction_noprefix[0x74] = ld_rr;
    // instruction_noprefix[0x75] = ld_rr;
    // instruction_noprefix[0x76] = halt;
    // instruction_noprefix[0x77] = ld_rr;
    // instruction_noprefix[0x78] = ld_rr;
    // instruction_noprefix[0x79] = ld_rr;
    // instruction_noprefix[0x7A] = ld_rr;
    // instruction_noprefix[0x7B] = ld_rr;
    // instruction_noprefix[0x7C] = ld_rr;
    // instruction_noprefix[0x7D] = ld_rr;
    // instruction_noprefix[0x7E] = ld_rr;
    // instruction_noprefix[0x7F] = ld_rr;

    // instruction_noprefix[0x80] = add_ar;
    // instruction_noprefix[0x81] = add_ar;
    // instruction_noprefix[0x82] = add_ar;
    // instruction_noprefix[0x83] = add_ar;
    // instruction_noprefix[0x84] = add_ar;
    // instruction_noprefix[0x85] = add_ar;
    // instruction_noprefix[0x86] = add_ar;
    // instruction_noprefix[0x87] = add_ar;
    // instruction_noprefix[0x88] = adc_ar;
    // instruction_noprefix[0x89] = adc_ar;
    // instruction_noprefix[0x8A] = adc_ar;
    // instruction_noprefix[0x8B] = adc_ar;
    // instruction_noprefix[0x8C] = adc_ar;
    // instruction_noprefix[0x8D] = adc_ar;
    // instruction_noprefix[0x8E] = adc_ar;
    // instruction_noprefix[0x8F] = adc_ar;

    // instruction_noprefix[0x90] = sub_ar;
    // instruction_noprefix[0x91] = sub_ar;
    // instruction_noprefix[0x92] = sub_ar;
    // instruction_noprefix[0x93] = sub_ar;
    // instruction_noprefix[0x94] = sub_ar;
    // instruction_noprefix[0x95] = sub_ar;
    // instruction_noprefix[0x96] = sub_ar;
    // instruction_noprefix[0x97] = sub_ar;
    // instruction_noprefix[0x98] = sbc_ar;
    // instruction_noprefix[0x99] = sbc_ar;
    // instruction_noprefix[0x9A] = sbc_ar;
    // instruction_noprefix[0x9B] = sbc_ar;
    // instruction_noprefix[0x9C] = sbc_ar;
    // instruction_noprefix[0x9D] = sbc_ar;
    // instruction_noprefix[0x9E] = sbc_ar;
    // instruction_noprefix[0x9F] = sbc_ar;

    // instruction_noprefix[0x90] = sub_ar;
    // instruction_noprefix[0x91] = sub_ar;
    // instruction_noprefix[0x92] = sub_ar;
    // instruction_noprefix[0x93] = sub_ar;
    // instruction_noprefix[0x94] = sub_ar;
    // instruction_noprefix[0x95] = sub_ar;
    // instruction_noprefix[0x96] = sub_ar;
    // instruction_noprefix[0x97] = sub_ar;
    // instruction_noprefix[0x98] = sbc_ar;
    // instruction_noprefix[0x99] = sbc_ar;
    // instruction_noprefix[0x9A] = sbc_ar;
    // instruction_noprefix[0x9B] = sbc_ar;
    // instruction_noprefix[0x9C] = sbc_ar;
    // instruction_noprefix[0x9D] = sbc_ar;
    // instruction_noprefix[0x9E] = sbc_ar;
    // instruction_noprefix[0x9F] = sbc_ar;

    // instruction_noprefix[0xA0] = and_ar;
    // instruction_noprefix[0xA1] = and_ar;
    // instruction_noprefix[0xA2] = and_ar;
    // instruction_noprefix[0xA3] = and_ar;
    // instruction_noprefix[0xA4] = and_ar;
    // instruction_noprefix[0xA5] = and_ar;
    // instruction_noprefix[0xA6] = and_ar;
    // instruction_noprefix[0xA7] = and_ar;
    // instruction_noprefix[0xA8] = xor_ar;
    // instruction_noprefix[0xA9] = xor_ar;
    // instruction_noprefix[0xAA] = xor_ar;
    // instruction_noprefix[0xAB] = xor_ar;
    // instruction_noprefix[0xAC] = xor_ar;
    // instruction_noprefix[0xAD] = xor_ar;
    // instruction_noprefix[0xAE] = xor_ar;
    // instruction_noprefix[0xAF] = xor_ar;

    // instruction_noprefix[0xB0] = or_ar;
    // instruction_noprefix[0xB1] = or_ar;
    // instruction_noprefix[0xB2] = or_ar;
    // instruction_noprefix[0xB3] = or_ar;
    // instruction_noprefix[0xB4] = or_ar;
    // instruction_noprefix[0xB5] = or_ar;
    // instruction_noprefix[0xB6] = or_ar;
    // instruction_noprefix[0xB7] = or_ar;
    // instruction_noprefix[0xB8] = cp_ar;
    // instruction_noprefix[0xB9] = cp_ar;
    // instruction_noprefix[0xBA] = cp_ar;
    // instruction_noprefix[0xBB] = cp_ar;
    // instruction_noprefix[0xBC] = cp_ar;
    // instruction_noprefix[0xBD] = cp_ar;
    // instruction_noprefix[0xBE] = cp_ar;
    // instruction_noprefix[0xBF] = cp_ar;

    // // impl me
    // instruction_noprefix[0xC0] = ret;
    // instruction_noprefix[0xC1] = ret;
    // instruction_noprefix[0xC2] = ret;
    // instruction_noprefix[0xC3] = ret;
    // instruction_noprefix[0xC4] = ret;
    // instruction_noprefix[0xC5] = ret;
    // instruction_noprefix[0xC6] = ret;
    // instruction_noprefix[0xC7] = ret;
    // instruction_noprefix[0xC8] = cp_ar;
    // instruction_noprefix[0xC9] = cp_ar;
    // instruction_noprefix[0xCA] = cp_ar;
    // instruction_noprefix[0xCB] = cp_ar; // cb prefixed
    // instruction_noprefix[0xCC] = cp_ar;
    // instruction_noprefix[0xCD] = cp_ar;
    // instruction_noprefix[0xCE] = cp_ar;
    // instruction_noprefix[0xCF] = cp_ar;

    // instruction_noprefix[0xD0] = ret;
    // instruction_noprefix[0xD1] = ret;
    // instruction_noprefix[0xD2] = ret;
    // instruction_noprefix[0xD3] = ret;
    // instruction_noprefix[0xD4] = ret;
    // instruction_noprefix[0xD5] = ret;
    // instruction_noprefix[0xD6] = ret;
    // instruction_noprefix[0xD7] = ret;
    // instruction_noprefix[0xD8] = cp_ar;
    // instruction_noprefix[0xD9] = cp_ar;
    // instruction_noprefix[0xDA] = cp_ar;
    // instruction_noprefix[0xDB] = cp_ar; // cb prefixed
    // instruction_noprefix[0xDC] = cp_ar;
    // instruction_noprefix[0xDD] = cp_ar;
    // instruction_noprefix[0xDE] = cp_ar;
    // instruction_noprefix[0xDF] = cp_ar;

    // instruction_noprefix[0xE0] = ret;
    // instruction_noprefix[0xE1] = ret;
    // instruction_noprefix[0xE2] = ret;
    // instruction_noprefix[0xE3] = ret;
    // instruction_noprefix[0xE4] = ret;
    // instruction_noprefix[0xE5] = ret;
    // instruction_noprefix[0xE6] = ret;
    // instruction_noprefix[0xE7] = ret;
    // instruction_noprefix[0xE8] = cp_ar;
    // instruction_noprefix[0xE9] = cp_ar;
    // instruction_noprefix[0xEA] = cp_ar;
    // instruction_noprefix[0xEB] = cp_ar; // cb prefixed
    // instruction_noprefix[0xEC] = cp_ar;
    // instruction_noprefix[0xED] = cp_ar;
    // instruction_noprefix[0xEE] = cp_ar;
    // instruction_noprefix[0xEF] = cp_ar;

    // instruction_noprefix[0xF0] = ret;
    // instruction_noprefix[0xF1] = ret;
    // instruction_noprefix[0xF2] = ret;
    // instruction_noprefix[0xF3] = ret;
    // instruction_noprefix[0xF4] = ret;
    // instruction_noprefix[0xF5] = ret;
    // instruction_noprefix[0xF6] = ret;
    // instruction_noprefix[0xF7] = ret;
    // instruction_noprefix[0xF8] = cp_ar;
    // instruction_noprefix[0xF9] = cp_ar;
    // instruction_noprefix[0xFA] = cp_ar;
    // instruction_noprefix[0xFB] = cp_ar; // cb prefixed
    // instruction_noprefix[0xFC] = cp_ar;
    // instruction_noprefix[0xFD] = cp_ar;
    // instruction_noprefix[0xFE] = cp_ar;
    // instruction_noprefix[0xFF] = cp_ar;

    // // CB prefixed
    // instruction_cbprefix[0x00] = nop;
    // instruction_cbprefix[0x01] = ld_rrnn;
    // instruction_cbprefix[0x02] = ld_bca;
    // instruction_cbprefix[0x03] = inc_rr;
    // instruction_cbprefix[0x04] = inc_r;
    // instruction_cbprefix[0x05] = dec_r;
    // instruction_cbprefix[0x06] = ld_rn;
    // instruction_cbprefix[0x07] = rlc_r;
    // instruction_cbprefix[0x08] = ld_nnsp;
    // instruction_cbprefix[0x09] = add_hlrr;
    // instruction_cbprefix[0x0A] = ld_abc;
    // instruction_cbprefix[0x0B] = dec_rr;
    // instruction_cbprefix[0x0C] = inc_r;
    // instruction_cbprefix[0x0D] = dec_r;
    // instruction_cbprefix[0x0E] = ld_rn;
    // instruction_cbprefix[0x0F] = rrc_r;

    // instruction_cbprefix[0x10] = stop;
    // instruction_cbprefix[0x11] = ld_rrnn;
    // instruction_cbprefix[0x12] = ld_dea;
    // instruction_cbprefix[0x13] = inc_rr;
    // instruction_cbprefix[0x14] = inc_r;
    // instruction_cbprefix[0x15] = dec_r;
    // instruction_cbprefix[0x16] = ld_rn;
    // instruction_cbprefix[0x17] = rla;
    // instruction_cbprefix[0x18]; // implement me
    // instruction_cbprefix[0x19] = add_hlrr;
    // instruction_cbprefix[0x1A] = ld_ade;
    // instruction_cbprefix[0x1B] = dec_rr;
    // instruction_cbprefix[0x1C] = inc_r;
    // instruction_cbprefix[0x1D] = dec_r;
    // instruction_cbprefix[0x1E] = ld_rn;
    // instruction_cbprefix[0x1F] = rra;

    // instruction_cbprefix[0x20]; // impl me
    // instruction_cbprefix[0x21] = ld_rrnn;
    // instruction_cbprefix[0x22] = ldi_hla;
    // instruction_cbprefix[0x23] = inc_hl;
    // instruction_cbprefix[0x24] = inc_r;
    // instruction_cbprefix[0x25] = dec_r;
    // instruction_cbprefix[0x26] = ld_rn;
    // instruction_cbprefix[0x27] = daa;
    // instruction_cbprefix[0x28]; // impl me
    // instruction_cbprefix[0x29] = add_hlrr;
    // instruction_cbprefix[0x2A] = ldi_ahl;
    // instruction_cbprefix[0x2B] = dec_rr;
    // instruction_cbprefix[0x2C] = inc_r;
    // instruction_cbprefix[0x2D] = dec_r;
    // instruction_cbprefix[0x2E] = ld_rn;
    // instruction_cbprefix[0x2F] = cpl;

    // instruction_cbprefix[0x30]; // impl me
    // instruction_cbprefix[0x31] = ld_rrnn;
    // instruction_cbprefix[0x32] = ldd_hla;
    // instruction_cbprefix[0x33] = inc_rr;
    // instruction_cbprefix[0x34] = inc_hl;
    // instruction_cbprefix[0x35] = dec_hl;
    // instruction_cbprefix[0x36] = ld_hln;
    // instruction_cbprefix[0x37] = scf;
    // instruction_cbprefix[0x38]; // impl me
    // instruction_cbprefix[0x39] = add_hlrr;
    // instruction_cbprefix[0x3A] = ldd_ahl;
    // instruction_cbprefix[0x3B] = dec_rr;
    // instruction_cbprefix[0x3C] = inc_r;
    // instruction_cbprefix[0x3D] = dec_r;
    // instruction_cbprefix[0x3E] = ld_rn;
    // instruction_cbprefix[0x3F] = ccf;

    // instruction_cbprefix[0x40] = ld_rr;
    // instruction_cbprefix[0x41] = ld_rr;
    // instruction_cbprefix[0x42] = ld_rr;
    // instruction_cbprefix[0x43] = ld_rr;
    // instruction_cbprefix[0x44] = ld_rr;
    // instruction_cbprefix[0x45] = ld_rr;
    // instruction_cbprefix[0x46] = ld_rr;
    // instruction_cbprefix[0x47] = ld_rr;
    // instruction_cbprefix[0x48] = ld_rr;
    // instruction_cbprefix[0x49] = ld_rr;
    // instruction_cbprefix[0x4A] = ld_rr;
    // instruction_cbprefix[0x4B] = ld_rr;
    // instruction_cbprefix[0x4C] = ld_rr;
    // instruction_cbprefix[0x4D] = ld_rr;
    // instruction_cbprefix[0x4E] = ld_rr;
    // instruction_cbprefix[0x4F] = ld_rr;

    // instruction_cbprefix[0x50] = ld_rr;
    // instruction_cbprefix[0x51] = ld_rr;
    // instruction_cbprefix[0x52] = ld_rr;
    // instruction_cbprefix[0x53] = ld_rr;
    // instruction_cbprefix[0x54] = ld_rr;
    // instruction_cbprefix[0x55] = ld_rr;
    // instruction_cbprefix[0x56] = ld_rr;
    // instruction_cbprefix[0x57] = ld_rr;
    // instruction_cbprefix[0x58] = ld_rr;
    // instruction_cbprefix[0x59] = ld_rr;
    // instruction_cbprefix[0x5A] = ld_rr;
    // instruction_cbprefix[0x5B] = ld_rr;
    // instruction_cbprefix[0x5C] = ld_rr;
    // instruction_cbprefix[0x5D] = ld_rr;
    // instruction_cbprefix[0x5E] = ld_rr;
    // instruction_cbprefix[0x5F] = ld_rr;

    // instruction_cbprefix[0x60] = ld_rr;
    // instruction_cbprefix[0x61] = ld_rr;
    // instruction_cbprefix[0x62] = ld_rr;
    // instruction_cbprefix[0x63] = ld_rr;
    // instruction_cbprefix[0x64] = ld_rr;
    // instruction_cbprefix[0x65] = ld_rr;
    // instruction_cbprefix[0x66] = ld_rr;
    // instruction_cbprefix[0x67] = ld_rr;
    // instruction_cbprefix[0x68] = ld_rr;
    // instruction_cbprefix[0x69] = ld_rr;
    // instruction_cbprefix[0x6A] = ld_rr;
    // instruction_cbprefix[0x6B] = ld_rr;
    // instruction_cbprefix[0x6C] = ld_rr;
    // instruction_cbprefix[0x6D] = ld_rr;
    // instruction_cbprefix[0x6E] = ld_rr;
    // instruction_cbprefix[0x6F] = ld_rr;

    // instruction_cbprefix[0x70] = ld_rr;
    // instruction_cbprefix[0x71] = ld_rr;
    // instruction_cbprefix[0x72] = ld_rr;
    // instruction_cbprefix[0x73] = ld_rr;
    // instruction_cbprefix[0x74] = ld_rr;
    // instruction_cbprefix[0x75] = ld_rr;
    // instruction_cbprefix[0x76] = halt;
    // instruction_cbprefix[0x77] = ld_rr;
    // instruction_cbprefix[0x78] = ld_rr;
    // instruction_cbprefix[0x79] = ld_rr;
    // instruction_cbprefix[0x7A] = ld_rr;
    // instruction_cbprefix[0x7B] = ld_rr;
    // instruction_cbprefix[0x7C] = ld_rr;
    // instruction_cbprefix[0x7D] = ld_rr;
    // instruction_cbprefix[0x7E] = ld_rr;
    // instruction_cbprefix[0x7F] = ld_rr;

    // instruction_cbprefix[0x80] = add_ar;
    // instruction_cbprefix[0x81] = add_ar;
    // instruction_cbprefix[0x82] = add_ar;
    // instruction_cbprefix[0x83] = add_ar;
    // instruction_cbprefix[0x84] = add_ar;
    // instruction_cbprefix[0x85] = add_ar;
    // instruction_cbprefix[0x86] = add_ar;
    // instruction_cbprefix[0x87] = add_ar;
    // instruction_cbprefix[0x88] = adc_ar;
    // instruction_cbprefix[0x89] = adc_ar;
    // instruction_cbprefix[0x8A] = adc_ar;
    // instruction_cbprefix[0x8B] = adc_ar;
    // instruction_cbprefix[0x8C] = adc_ar;
    // instruction_cbprefix[0x8D] = adc_ar;
    // instruction_cbprefix[0x8E] = adc_ar;
    // instruction_cbprefix[0x8F] = adc_ar;

    // instruction_cbprefix[0x90] = sub_ar;
    // instruction_cbprefix[0x91] = sub_ar;
    // instruction_cbprefix[0x92] = sub_ar;
    // instruction_cbprefix[0x93] = sub_ar;
    // instruction_cbprefix[0x94] = sub_ar;
    // instruction_cbprefix[0x95] = sub_ar;
    // instruction_cbprefix[0x96] = sub_ar;
    // instruction_cbprefix[0x97] = sub_ar;
    // instruction_cbprefix[0x98] = sbc_ar;
    // instruction_cbprefix[0x99] = sbc_ar;
    // instruction_cbprefix[0x9A] = sbc_ar;
    // instruction_cbprefix[0x9B] = sbc_ar;
    // instruction_cbprefix[0x9C] = sbc_ar;
    // instruction_cbprefix[0x9D] = sbc_ar;
    // instruction_cbprefix[0x9E] = sbc_ar;
    // instruction_cbprefix[0x9F] = sbc_ar;

    // instruction_cbprefix[0x90] = sub_ar;
    // instruction_cbprefix[0x91] = sub_ar;
    // instruction_cbprefix[0x92] = sub_ar;
    // instruction_cbprefix[0x93] = sub_ar;
    // instruction_cbprefix[0x94] = sub_ar;
    // instruction_cbprefix[0x95] = sub_ar;
    // instruction_cbprefix[0x96] = sub_ar;
    // instruction_cbprefix[0x97] = sub_ar;
    // instruction_cbprefix[0x98] = sbc_ar;
    // instruction_cbprefix[0x99] = sbc_ar;
    // instruction_cbprefix[0x9A] = sbc_ar;
    // instruction_cbprefix[0x9B] = sbc_ar;
    // instruction_cbprefix[0x9C] = sbc_ar;
    // instruction_cbprefix[0x9D] = sbc_ar;
    // instruction_cbprefix[0x9E] = sbc_ar;
    // instruction_cbprefix[0x9F] = sbc_ar;

    // instruction_cbprefix[0xA0] = and_ar;
    // instruction_cbprefix[0xA1] = and_ar;
    // instruction_cbprefix[0xA2] = and_ar;
    // instruction_cbprefix[0xA3] = and_ar;
    // instruction_cbprefix[0xA4] = and_ar;
    // instruction_cbprefix[0xA5] = and_ar;
    // instruction_cbprefix[0xA6] = and_ar;
    // instruction_cbprefix[0xA7] = and_ar;
    // instruction_cbprefix[0xA8] = xor_ar;
    // instruction_cbprefix[0xA9] = xor_ar;
    // instruction_cbprefix[0xAA] = xor_ar;
    // instruction_cbprefix[0xAB] = xor_ar;
    // instruction_cbprefix[0xAC] = xor_ar;
    // instruction_cbprefix[0xAD] = xor_ar;
    // instruction_cbprefix[0xAE] = xor_ar;
    // instruction_cbprefix[0xAF] = xor_ar;

    // instruction_cbprefix[0xB0] = or_ar;
    // instruction_cbprefix[0xB1] = or_ar;
    // instruction_cbprefix[0xB2] = or_ar;
    // instruction_cbprefix[0xB3] = or_ar;
    // instruction_cbprefix[0xB4] = or_ar;
    // instruction_cbprefix[0xB5] = or_ar;
    // instruction_cbprefix[0xB6] = or_ar;
    // instruction_cbprefix[0xB7] = or_ar;
    // instruction_cbprefix[0xB8] = cp_ar;
    // instruction_cbprefix[0xB9] = cp_ar;
    // instruction_cbprefix[0xBA] = cp_ar;
    // instruction_cbprefix[0xBB] = cp_ar;
    // instruction_cbprefix[0xBC] = cp_ar;
    // instruction_cbprefix[0xBD] = cp_ar;
    // instruction_cbprefix[0xBE] = cp_ar;
    // instruction_cbprefix[0xBF] = cp_ar;

    // // impl me
    // instruction_cbprefix[0xC0] = ret;
    // instruction_cbprefix[0xC1] = ret;
    // instruction_cbprefix[0xC2] = ret;
    // instruction_cbprefix[0xC3] = ret;
    // instruction_cbprefix[0xC4] = ret;
    // instruction_cbprefix[0xC5] = ret;
    // instruction_cbprefix[0xC6] = ret;
    // instruction_cbprefix[0xC7] = ret;
    // instruction_cbprefix[0xC8] = cp_ar;
    // instruction_cbprefix[0xC9] = cp_ar;
    // instruction_cbprefix[0xCA] = cp_ar;
    // instruction_cbprefix[0xCB] = cp_ar; // cb prefixed
    // instruction_cbprefix[0xCC] = cp_ar;
    // instruction_cbprefix[0xCD] = cp_ar;
    // instruction_cbprefix[0xCE] = cp_ar;
    // instruction_cbprefix[0xCF] = cp_ar;

    // instruction_cbprefix[0xD0] = ret;
    // instruction_cbprefix[0xD1] = ret;
    // instruction_cbprefix[0xD2] = ret;
    // instruction_cbprefix[0xD3] = ret;
    // instruction_cbprefix[0xD4] = ret;
    // instruction_cbprefix[0xD5] = ret;
    // instruction_cbprefix[0xD6] = ret;
    // instruction_cbprefix[0xD7] = ret;
    // instruction_cbprefix[0xD8] = cp_ar;
    // instruction_cbprefix[0xD9] = cp_ar;
    // instruction_cbprefix[0xDA] = cp_ar;
    // instruction_cbprefix[0xDB] = cp_ar; // cb prefixed
    // instruction_cbprefix[0xDC] = cp_ar;
    // instruction_cbprefix[0xDD] = cp_ar;
    // instruction_cbprefix[0xDE] = cp_ar;
    // instruction_cbprefix[0xDF] = cp_ar;

    // instruction_cbprefix[0xE0] = ret;
    // instruction_cbprefix[0xE1] = ret;
    // instruction_cbprefix[0xE2] = ret;
    // instruction_cbprefix[0xE3] = ret;
    // instruction_cbprefix[0xE4] = ret;
    // instruction_cbprefix[0xE5] = ret;
    // instruction_cbprefix[0xE6] = ret;
    // instruction_cbprefix[0xE7] = ret;
    // instruction_cbprefix[0xE8] = cp_ar;
    // instruction_cbprefix[0xE9] = cp_ar;
    // instruction_cbprefix[0xEA] = cp_ar;
    // instruction_cbprefix[0xEB] = cp_ar; // cb prefixed
    // instruction_cbprefix[0xEC] = cp_ar;
    // instruction_cbprefix[0xED] = cp_ar;
    // instruction_cbprefix[0xEE] = cp_ar;
    // instruction_cbprefix[0xEF] = cp_ar;

    // instruction_cbprefix[0xF0] = ret;
    // instruction_cbprefix[0xF1] = ret;
    // instruction_cbprefix[0xF2] = ret;
    // instruction_cbprefix[0xF3] = ret;
    // instruction_cbprefix[0xF4] = ret;
    // instruction_cbprefix[0xF5] = ret;
    // instruction_cbprefix[0xF6] = ret;
    // instruction_cbprefix[0xF7] = ret;
    // instruction_cbprefix[0xF8] = cp_ar;
    // instruction_cbprefix[0xF9] = cp_ar;
    // instruction_cbprefix[0xFA] = cp_ar;
    // instruction_cbprefix[0xFB] = cp_ar; // cb prefixed
    // instruction_cbprefix[0xFC] = cp_ar;
    // instruction_cbprefix[0xFD] = cp_ar;
    // instruction_cbprefix[0xFE] = cp_ar;
    // instruction_cbprefix[0xFF] = cp_ar;
}

void Gameboy::step(){
    // update timer
    // instr

    // first instr at 0 (booting)

    // first instr for cartridges at 0x0100
    
    uint8_t cycles = 0;
    
    // fetch instr PC
    opcode = readByte();

    // decode instr function table
    execInstruction();
    // execute instr


    t_cycles += cycles;

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

uint8_t Gameboy::ld_ann(){
    uint8_t lsb = readByte();
    uint8_t msb = readByte();

    uint16_t nn = (msb << 8u) | lsb;

    R[A] = M[nn];

    return 16;
}

uint8_t Gameboy::ld_nna(){
    uint8_t lsb = readByte();
    uint8_t msb = readByte();

    uint16_t nn = (msb << 8u) | lsb;

    M[nn] = R[A];

    return 16;
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

// END ROTATE AND SHIFT instructions

// SINGLE BIT OPERATION instructions

// END SINGLE BIT OPERATION instructions

// CPU CONTROL instructions

// END CPU CONTROL instructions

// JUMP instructions

// END JUMP instructions