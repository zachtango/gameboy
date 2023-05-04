#include <vector>
#include <string>
#include <iostream>
#include <SDL2/SDL.h>
#include <chrono>
#include "cartridge.h"
#include "mmu.h"
#include "ppu.h"
#include "cpu.h"
#include "timer.h"
#include "joypad.h"

// #define VIDEO_HEIGHT 192
// #define VIDEO_WIDTH 128
#define VIDEO_HEIGHT 144
#define VIDEO_WIDTH 160
#define VIDEO_SCALE 3


int main() {

    std::vector<std::string> v {
        "01-special.gb",
        "02-interrupts.gb",
        "03-op sp,hl.gb",
        "04-op r,imm.gb",
        "05-op rp.gb",
        "06-ld r,r.gb",
        "07-jr,jp,call,ret,rst.gb",
        "08-misc instrs.gb",
        "09-op r,r.gb",
        "10-bit ops.gb",
        "11-op a,(hl).gb"
    };

    for(auto s : v) {
        Joypad joypad;
        Cartridge cartridge;
        Interrupts interrupts;
        PPU ppu = PPU(interrupts);
        Timer timer = Timer(interrupts);

        MMU mmu = MMU(
            cartridge,
            ppu,
            timer,
            joypad,
            interrupts
        );

        CPU cpu = CPU(mmu);
        
        cartridge.load_rom(("../gb-test-roms/cpu_instrs/individual/" + s).c_str());

        int i = 0;

        while(i < 10000) {
            cpu.fetch_decode_execute();
            i += 1;
        }

    }

    return 0;
}
