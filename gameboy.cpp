#include <vector>
#include <string>
#include <iostream>
#include <SDL2/SDL.h>
#include <chrono>
#include "mmu.h"
#include "ppu.h"
#include "cpu.h"
#include "timer.h"

// #define VIDEO_HEIGHT 192
// #define VIDEO_WIDTH 128
#define VIDEO_HEIGHT 144
#define VIDEO_WIDTH 160
#define VIDEO_SCALE 3

BYTE video[256][256];

int main() {


    
    /*
        Block	VRAM Address
        0	    $8000–$87FF
        1	    $8800–$8FFF
        2	    $9000–$97FF

        24 x 16 Tiles
        192 x 128 Bytes
    */

    // Create a new window and renderer
    SDL_Window* window = SDL_CreateWindow("Gameboy Screen", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, VIDEO_WIDTH * VIDEO_SCALE, VIDEO_HEIGHT * VIDEO_SCALE, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_RenderSetScale(renderer, VIDEO_SCALE, VIDEO_SCALE);
    // Create a new texture to hold the pixel data
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB332, SDL_TEXTUREACCESS_STREAMING, VIDEO_WIDTH, VIDEO_HEIGHT);

    SDL_PumpEvents();

    std::vector<std::string> v = {
        // "01-special.gb",
        "02-interrupts.gb",
        // "03-op sp,hl.gb",
        // "04-op r,imm.gb",
        // "05-op rp.gb",
        // "06-ld r,r.gb",
        // "07-jr,jp,call,ret,rst.gb",
        // "08-misc instrs.gb",
        // "09-op r,r.gb",
        // "10-bit ops.gb",
        // "11-op a,(hl).gb"
    };

    for(auto s : v) {
        MMU mmu;
        mmu.load_boot_rom("DMG_ROM.bin");
        // mmu.load_rom(("individual/" + s).c_str());
        mmu.load_rom("dmg-acid2.gb");
        // mmu.load_rom("DMG_ROM.bin");

        PPU ppu = PPU(mmu);
        bool running = true;
        // Loop through different textures and render them to the screen
        // ppu.init_logo();
        CPU cpu = CPU(mmu, s);
        Timer timer = Timer(mmu);
        
        auto last_time = std::chrono::high_resolution_clock::now();
        const int frame_rate = 60; // 60 fps

        int i = 0;
        while(running) {
            
            UINT cycles = 0;
            
            if(!cpu.halt_mode) {
                cycles = cpu.run_fde();
            } else {
                // std::cout << std::hex << (int) mmu.read(IF_ADDRESS) << '\n';
                if(mmu.read(IF_ADDRESS)) {
                    cpu.halt_mode = false;
                }
                cycles = 4;
            }

            // Update timer and ppu
            for(int j = 0; j < cycles; j++) {
                timer.increment();
                ppu.tick();
            }

            auto current_time = std::chrono::high_resolution_clock::now();
            auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - last_time).count();
            auto frame_time = 1000 / frame_rate;

            if(elapsed_time >= frame_time && *mmu.write_to_tile) {

                // // Update the texture with the new pixel data
                SDL_UpdateTexture(texture, NULL, ppu.video, VIDEO_WIDTH);

                // Clear the screen and render the texture
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, texture, NULL, NULL);
                SDL_RenderPresent(renderer);

                last_time = current_time;
            }


            if(cpu.IME) {
                cpu.handle_interrupts();
            }

            if(cpu.EI) {
                std::cout << "IME enabled\n";
                cpu.IME = true;
                cpu.EI = false;
            }
            // std::cout << cpu.IME << ' ' << cpu.EI << ' ' << cpu.halt_mode << '\n';

            // Handle events and update the running flag
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    running = false;
                }
            }
            i += 1;
            // std::cout << i << '\n';
        }

    }

    // Clean up SDL resources
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
