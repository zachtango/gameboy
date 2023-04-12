#include <deque>

typedef unsigned char BYTE; // 1 byte
typedef char SIGNED_BYTE; // 1 byte
typedef unsigned short WORD; // 2 bytes
typedef signed short SIGNED_WORD; // 2 bytes

typedef enum ppu_state {
    oam_search,
    pixel_transfer,
    h_blank,
    v_blank
} ppu_state;

class PPU {
    BYTE ly; // 0 - 143
    BYTE scx;
    BYTE scy;

    ppu_state state;
    
    unsigned int ticks;

    void tick();
    void render_line();

};

void PPU::render_line() {
    /*
        2 cases:
        
        1. SCX is at the beginning of a tile
        - scx % 8 == 0
        - i = [0, 160)
        2. SCX is in the middle of a tile --> add extra tile
        - scx % 8 != 0
        - i = [scx % 8, scx % 8 + 160)

    
    */
    std::deque<BYTE> line;

    for(int tile = 0; tile < 21; tile++) { // FIXME CHANGE TO TILES
        BYTE lx = tile * 8;
        BYTE tile_id = (scx + lx) / 8 + (scy + ly) / 8 * 20;
        
        // TODO: Implement wrapping

        /*
            2 32 x 32 tile maps in VRAM
                memory at 9800 - 9BFF and 9C00 - 9FFF

            use 9800 by default 
        */

        BYTE tile_number; // = Memory[0x9800 + tile_id] 32 x 32
        
        /*
            based on lcdc number, tile number is used as unsigned int or signed int
            8000 addressing vs 8800 addressing
        */

        BYTE tile_lsb; // = Memory[0x8000 + tile_number * 2]
        BYTE tile_msb; // = Memory[0x8000 + tile_number * 2 + 1]

        // produce 8 pixels of tile
        for(int i = 7; i >= 0; i--) {
            bool lsb = (tile_lsb >> i) & 0x01,
                 msb = (tile_msb >> i) & 0x01;
                
            BYTE c = (msb << 1u) | lsb;

            line.push_back(c);
        }
    }

    for(int i = 0; i < scx % 8; i++)
        line.pop_front();
    for(int i = 0; i < line.size() - 160; i++)
        line.pop_back();

    // render line to video
    // line
}

void PPU::tick() {
    /*
        Per line
            OAM Search 20 clocks
            - decide which sprites visible on that line (up to 10 in array)
            Pixel transfer 43 clocks
            - draw to screen
            H blank 51 clocks
            - rest
        144 lines

        last 10 lines are v blank (114 clocks)

    */

    ticks += 1;
    switch(state) {
        case oam_search:
            // collect sprite data

            if(ticks == 80) {
                // pixel fetcher

                state = pixel_transfer;
            }

            break;
        case pixel_transfer:
            // push pixel data to display

            // don't need to render every time, only 60 fps
            render_line();
            
            if(ticks == 172) {
                state = h_blank;
            }

            break;
        case h_blank:
            // wait for however many ticks left and then go to next state

            if(ticks == 456) {
                ticks = 0;
                ly += 1;

                if(ly == 144) {
                    state = v_blank;
                } else {
                    state = oam_search;
                }
            }

            break;
        case v_blank:
            // wait for 10 lines, go back to oam search

            if(ticks == 456) {
                ticks = 0;
                ly += 1;
                if(ly == 154) {
                    ly = 0;
                    state = oam_search;
                }
            }

            break;
        default:
            throw "Unknown PPU State";
    }

}

