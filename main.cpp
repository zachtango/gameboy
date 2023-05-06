#include <vector>
#include <string>

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

const int FRAME_RATE = 60;

int main() {

    std::vector<std::string> v {
        // "roms/dmg-acid2.gb",
        // "roms/DrMario.gb",
        "bgbtest.gb",
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

    // Create a window and renderer
    SDL_Window* window = SDL_CreateWindow("Gameboy", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, VIDEO_WIDTH * 3, VIDEO_HEIGHT * 3, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_RenderSetScale(renderer, 3, 3);

    // Create a texture to hold the pixel data
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_BGR444, SDL_TEXTUREACCESS_STREAMING, VIDEO_WIDTH, VIDEO_HEIGHT);
    
    SDL_PumpEvents();

    for(auto s : v) {
        // for rendering SDL at 60 fps
        auto start_time = std::chrono::high_resolution_clock::now();

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

        CPU cpu = CPU(mmu, false);
        
        // cartridge.load_rom(("../gb-test-roms/cpu_instrs/individual/" + s).c_str());
        cartridge.load_rom(s.c_str());

        int i = 0, end;
        
        bool running = true;
        while(running) {
            U32 cycles;
            
            if(!cpu.halt_mode) {
                cycles = cpu.fetch_decode_execute();
            } else {
                // https://gbdev.io/pandocs/halt.html
                // wake up CPU if interrupt_enable (0xFFFF) AND interrupt_flag (0xFF0F) is non zero
                if(mmu.read(0xFFFF) & mmu.read(0xFF0F) & 0b00011111)
                    cpu.halt_mode = false;
                cycles = 4;
            }

            for(int j = 0; j < cycles; j++) {
                timer.tick();
                ppu.tick();
            }

            cycles = interrupts.handle_interrupt(cpu);

            for(int j = 0; j < cycles; j++) {
                timer.tick();
                ppu.tick();
            }

            // Get the current time again
            auto current_time = std::chrono::high_resolution_clock::now();
            
            // Calculate the elapsed time since the last frame
            auto elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>(current_time - start_time);
            
            // Calculate the minimum amount of time that should pass before rendering the next frame
            auto min_frame_time = std::chrono::microseconds(1000000 / FRAME_RATE);
            
            if(elapsed_time >= min_frame_time) {
                // Render the next frame
                // Stream the pixel data to the texture
                SDL_UpdateTexture(texture, nullptr, ppu.video, 160 * sizeof(WORD));
                
                // Render the texture to the window
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, texture, nullptr, nullptr);
                SDL_RenderPresent(renderer);
                
                // Reset the start time to the current time
                start_time = std::chrono::high_resolution_clock::now();
            }

            // handle user input
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                switch(event.type) {
                    case SDL_QUIT:
                        running = false;
                        break;
                    case SDL_KEYDOWN:
                    case SDL_KEYUP:
                        bool pressed = event.type == SDL_KEYDOWN;
                        switch(event.key.keysym.sym) {
                            case SDLK_UP:
                                joypad.key_action(UP_INPUT, pressed);
                                break;
                            case SDLK_DOWN:
                                joypad.key_action(DOWN_INPUT, pressed);
                                break;
                            case SDLK_LEFT:
                                joypad.key_action(LEFT_INPUT, pressed);
                                break;
                            case SDLK_RIGHT:
                                joypad.key_action(RIGHT_INPUT, pressed);
                                break;
                            case SDLK_z:
                                joypad.key_action(A_INPUT, pressed);
                                break;
                            case SDLK_x:
                                joypad.key_action(B_INPUT, pressed);
                                break;
                            case SDLK_c:
                                joypad.key_action(START_INPUT, pressed);
                                break;
                            case SDLK_v:
                                joypad.key_action(SELECT_INPUT, pressed);
                                break;
                        }
                        break;
                }
            }

            i += 1;
        }
    }

    return 0;
}
