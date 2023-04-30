#include "constants.h"
#include "helpers.h"
#include "mmu.h"

typedef enum ppu_state {
    oam_search,
    pixel_transfer,
    h_blank,
    v_blank
} ppu_state;

class PPU {
public:
    const BYTE pallete[4] {
        0b00100101,
        0b01101111,
        0b10010011,
        0b11111111
    };

    PPU() {}
    PPU(MMU &mmu) : mmu(mmu) {
        ticks = 0;
        ly = 0;
        window_ly = 0;
        state = oam_search;
        mmu.write(
            0xFF41,
            (mmu.read(0xFF41) & ~0b11) | 0b10
        );
    }
    
    BYTE ly; // 0 - 153
    BYTE window_ly;
    BYTE scx;
    BYTE scy;
    BYTE lcdc;

    ppu_state state;

    MMU mmu;

    unsigned int ticks;

    void init_logo();
    void tick();
    void render_line();
    void render_tiles();
    void increment_y();

    BYTE video[144][160];

    BYTE tiles[192][128];

};