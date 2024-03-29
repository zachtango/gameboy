#ifndef PPU_H
#define PPU_H

#include <utility>
#include "constants.h"
#include "helpers.h"
#include "interrupts.h"

#define BYTES_PER_TILE 16
#define BYTES_PER_TILE_LINE 2
#define NULL_SPRITE_ADDRESS 0xFF

typedef enum ppu_mode {
    oam_scan = 2,
    pixel_transfer = 3,
    h_blank = 0,
    v_blank = 1
} ppu_mode;

class PPU {
public:
    // Clear lower 2 bits of status and OR the mode
    #define SET_PPU_MODE(mode) { status = (status & 0xFC) | mode; }

    PPU(Interrupts &interrupts) : interrupts(interrupts) {
        // initialize PPU state
        SET_PPU_MODE(ppu_mode::oam_scan)

        previous_stat_on = false;
        ly = 0;
        window_ly = 0;
        scy = 0;
        scx = 0;
        ticks = 0;
        number_of_sprites = 0;

        // set video to black
        for(int i = 0; i < 144; i++)
            for(int j = 0; j < 160; j++)
                video[i][j] = 0;
        // set line to black
        for(int j = 0; j < 160; j++)
            line[j][0] = 0;

        std::cout << "PPU INITALIZED\n";
    }

    /* TICK MASTER LOOP */
    void tick();

    /* MEMORY FUNCTIONS */
    BYTE read(WORD address);
    void write(WORD address, BYTE value);

    /* VIDEO */
    WORD video[144][160];

    // DIMENSIONS USED FOR DEBUGGING
    // WORD video[192][160 + 4 + 128];

private:
    // holds whether STAT was on last check
    bool previous_stat_on;

    /* TICK COUNTER */
    U32 ticks;

    /* MEMORY FUNCTIONS HELPER */
    BYTE *memory_map(WORD address);

    /* PPU MODES */
    void oam_scan();
    void pixel_transfer();
    void h_blank();
    void v_blank();

    /* PPU MODE HELPERS */
    void print_tile_maps();
    bool is_stat_on();
    bool is_window_visible();
    void write_line();
    void increment_ly();
    void write_tiles();
    
    // For sending VBLANK and LCD_STAT interrupts
    Interrupts &interrupts;

    /* RENDERING */
    const WORD color[4] {
        0b0111111111111111, // White
        0b0101001010010100, // Light Gray
        0b0001110011100111, // Mid Gray
        0b0000000000000000 // Black
    };

    // internal window ly counter (not mapped to memory)
    BYTE window_ly;

    // Sprite addresses of 10 Sprites picked from OAM scan
    WORD sprite_addresses[10];
    BYTE number_of_sprites;
    
    /* contains pixel data of current line
        0: 2 bit pixel value
        1: bg or sprite
        2: sprite flags (if sprite)
        3: x value (if sprite)
    */
    BYTE line[160][4]; 

    /* MEMORY */

    // 0x8000 - 0x9FFF
    BYTE vram[0x2000];
        
    // 0xFE00 - 0xFE9F
    BYTE oam[0x00A0];

    // LCD Registers (0xFF40 - 0xFF4B)
        // control
        BYTE control; // 0xFF40
        
        // status
        BYTE ly; // 0xFF44
        BYTE lyc; // 0xFF45
        BYTE status; // 0xFF41
        
        // position and scrolling
        BYTE scy; // 0xFF42
        BYTE scx; // 0xFF43
        BYTE wy; // 0xFF4A
        BYTE wx; // 0xFF4B

        // palettes
        BYTE bg_palette; // 0xFF47
        BYTE obj_palette0; // 0xFF48
        BYTE obj_palette1; // 0xFF49
};

#endif