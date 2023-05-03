#include <vector>
#include <string>
#include <iostream>
#include <SDL2/SDL.h>
#include <chrono>
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

        MMU mmu = MMU(&joypad);

        mmu.load_rom(("../gb-test-roms/cpu_instrs/individual/" + s).c_str());

        PPU ppu = PPU(mmu);
        bool running = true;
        // Loop through different textures and render them to the screen
        // ppu.init_logo();
        CPU cpu = CPU(mmu);
        Timer timer = Timer(mmu);

        int i = 0;
        
    }

    return 0;
}
