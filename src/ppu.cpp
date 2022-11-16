#include "ppu.h"
#include <iostream>
/*
    tile - 8 x 8 pixels
    background - 32 x 32 tiles, 256 x 256 pixels
    gameboy display - 160 x 144 pixels
        moved across the background
        gpu does this by defining a point in the background that's the top left of the screen
        moves this point between frames --> background scrolls on the screen
        two GPU registers scroll x and scroll y has top left pos

    pallete - in a pixel, 4 shades of color can be represented --> 2 bits each
    a tile then needs 8 x 8 x 2 bits (16 bytes)

        0   pixel off   [255, 255, 255]
        1   33% on      [192, 192, 192]
        2   66% on      [96, 96, 96]
        3   on          [0, 0, 0]

        background palette registers holds the colors
        bit 7 6 color 3, bit 5 4 color 2, bit 3 2 color 1, bit 1 0 color 0

    one row of the tile is 2 bytse --> 
    each pixels low bit is in one byte and each
    pixel's high bit in another byte

    VRAM tile data
        8000 - 97FF: each tile takes 16 bytes; 384 tiles in total

        LCDC.4 = 1 BG / WIN is 8000 - 87FF

*/
using namespace std;

PPU::PPU(){
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("SDL Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, VIDEO_WIDTH, VIDEO_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, VIDEO_WIDTH, VIDEO_HEIGHT);
    screenSurface = SDL_GetWindowSurface(window);
}

void PPU::draw(uint32_t *video, uint8_t const* const* tileMap){
    uint16_t videoAddress = 0;
    uint16_t tile = 0;

    for(uint8_t i = 0; i < 32; i++){
        for(uint8_t by = 0; by < 16; by += 2){ // sets of 2 bytes (lo and hi bit)
            // do b and b + 1th byte of the 32 tiles before the bytes greater than b + 1
            tile = i * 32; // 1st tile of row i

            for(uint8_t j = 0; j < 32; j++){
                // lo byte
                uint8_t lo = *(tileMap[tile] + by);

                // hi byte
                uint8_t hi = *(tileMap[tile] + by + 1);

                for(int8_t b = 7; b >= 0; b--){ // individual bit 7 - 0
                    uint8_t color = ( ( hi >> b & 0x1 ) << 1u ) | ( lo >> b & 0x1 );

                    switch(color){
                        case 0: // white
                            video[videoAddress] = WHITE;
                            break;
                        case 1: // light gray
                            video[videoAddress] = LIGHT_GRAY;
                            break;
                        case 2: // gray
                            video[videoAddress] = GRAY;
                            break;
                        case 3: // black
                            video[videoAddress] = BLACK;
                    }

                    videoAddress += 1;
                }

                tile += 1;
            }

        }
    }
}

void PPU::updateWindow(
    uint8_t const* const* backgroundTileMap,
    uint8_t const* const* windowTileMap
){
    // for(int i = 0; i < 32; i++){
    //     for(int j = 0; j < 32; j++){
    //         cout << (int) *backgroundTileMap[i * 32 + j] << ' ';
    //     }
    //     cout << endl;
    // }
    
    // background is array of 256 grps of 16 bytes
    /*
        VRAM tile data
        0 	$8000–$87FF 	0–127 	0–127

        The Game Boy contains two 32x32 tile maps in VRAM at the memory areas $9800-$9BFF and $9C00-$9FFF.
        Any of these maps can be used to display the Background or the Window.

        3	BG tile map area	0=9800-9BFF, 1=9C00-9FFF
    */
    
    /*
        PIXEL_FORMAT_RGBA8888
        The pixel is represented by a 32-bit value, with A in bits (24-31), R in bits 16-23, G in bits 8-15 and B in bits 0-7.
        
        we have 4 RGBA values, 0 0 0 1 (A or the opactiy, is always 1) (black)
                                84, 84, 84, 1
                                168, 168, 168, 1
                                255, 255, 255, 1 (white)
        https://convertingcolors.com/rgbpercent-color-66_66_66.html?search=RGBPercent(66%,%2066%,%2066%)


        in the vb ram each pixel is 2 bits --> 4 colors --> map these bits to a video array where each entry is 32 bits PIXEL_FORMAT_RGBA8888
            0	White
            1	Light gray
            2	Dark gray
            3	Black
    */
    
    /*
        backgroundTileMap[i] = pointer to first byte of tile i (tile i jth byte = pointer[tile i] + j)
            1st byte is the lo bit, 2nd byte is the hi bit

        to put it into a video array, it's 32 by 32 --> put first 32 tiles, then next 32 tiles, next next 32 tiles

        0 - 2047 [0 - 31] tiles each 8 by 8 pixels
            0 - 255 1st 8 pixels of 32 tiles (1st row)
            256 - 511 2nd 8 pixels of 32 tiles (2nd row)
            ...
        2048 - 4095 [32 - 63] tiles
        ...
    */

    uint32_t video[VIDEO_HEIGHT * VIDEO_WIDTH];

    // draw background
    draw(video, backgroundTileMap);
    draw(video, windowTileMap);

    SDL_UpdateTexture(texture, NULL, video, VIDEO_WIDTH * sizeof(video[0]));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    SDL_Event e; bool quit = false; while( quit == false ){ while( SDL_PollEvent( &e ) ){ if( e.type == SDL_QUIT ) quit = true; } }
    
}

PPU::~PPU(){
    SDL_DestroyWindow(window);
    window = nullptr;

    SDL_DestroyTexture(texture);
    texture = nullptr;

    SDL_DestroyRenderer(renderer);
    renderer = nullptr;

    SDL_Quit();
}
