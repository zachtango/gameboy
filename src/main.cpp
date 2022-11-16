#include "gameboy.h"
#include "ppu.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <bitset>

using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


int main(int argv, char** args){


    Gameboy gb = Gameboy();
    PPU platform = PPU();

    

    for(int i = 0; i < 0x10000; i++){
        gb.step();
        // platform.updateWindow(gb.backgroundTileMap);
    }
    
    gb.printVRAM();

    platform.updateWindow(gb.backgroundTileMap, gb.windowTileMap);

    return 0;
}

