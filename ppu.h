#include <utility>
#include "constants.h"
#include "helpers.h"
#include "mmu.h"

typedef enum ppu_mode {
    oam_scan = 2,
    pixel_transfer = 3,
    h_blank = 0,
    v_blank = 1
} ppu_mode;

class PPU {
public:
    PPU(MMU &mmu) : mmu(mmu) {
        // initialize PPU state
        mode = ppu_mode::oam_scan;
        ly = 0;
        window_ly = 0;
        scy = 0;
        scx = 0;
        ticks = 0;

        // set video to black
        for(int i = 0; i < 144; i++)
            for(int j = 0; j < 160; j++)
                video[i][j] = 0;
    }

    /* TICK MASTER LOOP */
    void tick();

    /* MEMORY FUNCTIONS */
    BYTE read(WORD address);
    void write(WORD address, BYTE value);

    /* VIDEO + TILES */
    WORD video[144][160];
    WORD tiles[192][128];

private:
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
    void write_line();
    void increment_ly();

    // For reading and writing to memory
    MMU mmu;

    ppu_mode mode;

    /* RENDERING */
    const WORD color[4] {
        0b0000111111111111, // White
        0b0000010101010101, // Mid Gray
        0b0000001000100010, // Light Gray
        0b0000000000000000 // Black
    };

    // internal window ly counter (not mapped to memory)
    BYTE window_ly;

    // Sprite addresses of 10 Sprites picked from OAM scan
    WORD sprite_addresses[10];
    
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
    BYTE oam[0xA0];

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


    // Clear lower 2 bits of status and OR the mode
    #define SET_PPU_MODE(mode) { status = (status & 0xFC) | mode; }
};