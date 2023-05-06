#include "ppu.h"

/* TICK MASTER LOOP */
void PPU::tick() {
    ticks += 1;
    
    switch(status & 0b11) {
        case ppu_mode::oam_scan:
            return oam_scan();
        case ppu_mode::pixel_transfer:
            return pixel_transfer();
        case ppu_mode::h_blank:
            return h_blank();
        case ppu_mode::v_blank:
            return v_blank();
    }

    std::cerr << "Unknown PPU mode\n";
    throw "Unknown PPU mode";
}

/* MEMORY FUNCTIONS */
BYTE PPU::read(WORD address) {
    BYTE *p = memory_map(address);
    return *p;
}

void PPU::write(WORD address, BYTE value) {
    // switch(address) {
    //     // status
    //     case 0xFF44:
    //     case 0xFF45:
    //     case 0xFF41:
    //         // sends STAT interrupt if supposed to
    //         is_stat_on();
    // }

    BYTE *p = memory_map(address);
    *p = value;
}

/* Given 16 bit address
    Return pointer to the BYTE in memory the address refers to
*/
BYTE* PPU::memory_map(WORD address) {
    // VRAM
    if(0x8000 <= address && address <= 0x9FFF)
        return &vram[address - 0x8000];
    
    // OAM
    if(0xFE00 <= address && address <= 0xFE9F)
        return &oam[address - 0xFE00];

    // LCD Registers
    switch(address) {
        // control
        case 0xFF40:
            return &control;

        // status
        case 0xFF44:
            return &ly;
        case 0xFF45:
            return &lyc;
        case 0xFF41:
            return &status;

        // position and scrolling
        case 0xFF42:
            return &scy;
        case 0xFF43:
            return &scx;
        case 0xFF4A:
            return &wy;
        case 0xFF4B:
            return &wx;

        // palettes
        case 0xFF47:
            return &bg_palette;
        case 0xFF48:
            return &obj_palette0;
        case 0xFF49:
            return &obj_palette1;
    }

    std::cerr << std::hex << address << " Address not in PPU range\n";
    throw "Address not in PPU range";
}

/* PPU MODES */

/* OAM SCAN
    For the current line (ly), get the 10 sprites to display
        store the 10 sprites in sprites array for pixel_transfer() to use
*/
void PPU::oam_scan() {
    // Only get sprites at end of OAM Scan mode
    // Not the most accurate timing, but will work for most GB games
    if(ticks < 80)
        return;

    /*
        https://gbdev.io/pandocs/OAM.html?highlight=oam%20scan#selection-priority
    
        Scan sprites from 0xFE00 to 0xFE9F, picking the first 10 valid ones
            - each sprite is 4 bytes

        Valid criteria:
            - ly >= top of sprite AND ly <= bottom of sprite
    */
    // make sure up to 10 Sprites are stored in sprite_addresses array
    int counter = 0;
    
    for(WORD address = 0x0000; counter < 10 && address < 0x00A0; address += 4) {
        // https://gbdev.io/pandocs/OAM.html?highlight=oam%20scan#selection-priority
        // each sprite is 4 bytes

        // lcd control bit 2 on ? 16 pixels : 8 pixels
        BYTE h = get_bit(control, 2) ? 16 : 8;

        // y = position of top of Sprite + 16
        BYTE y = oam[address];
        
        // x = postiion of left of Sprite + 8
        BYTE x = oam[address + 1];
        
        // position_y of Sprite = y - 16
        // validate Sprite
        if( (y - 16) <= ly && ly < (y - 16 + h) ) {
            // add Sprite to 10 Sprite list
            sprite_addresses[counter] = address;

            counter += 1;
        }
    }

    // set rest of sprite addresses to 0, so we don't render sprites from previous lines
    for(; counter < 10; counter++) {
        sprite_addresses[counter] = 0;
    }

    // switch mode to Pixel Transfer by setting lower 2 bits of lcd status
    SET_PPU_MODE(ppu_mode::pixel_transfer)
}

/* PIXEL TRANSFER
    For the current line (ly), determine the correct pixel for each 160 pixels in the line
        store correct pixels in line
        store correct pixels in video at video[ly]
*/
void PPU::pixel_transfer() {
    // Only write pixels to video at end of Pixel Transfer mode
    // Not the most accurate timing, but will work for most GB games
    if(ticks < 172)
        return;

    // write line to video at current line (ly)
    write_line();

    // https://gbdev.io/pandocs/STAT.html#ff41--stat-lcd-status
    // if H Blank source interrupt (bit 3 of status), send STAT interrupt
    if(get_bit(status, 3))
        interrupts.request_interrupt(LCD_STAT_INTERRUPT);

    // switch mode to H Blank by setting lower 2 bits of lcd status
    SET_PPU_MODE(ppu_mode::h_blank)
}

/* H BLANK
    For the current line (ly), wait the number of ticks left
        FIXME: am I supposed to implement something here?
*/
void PPU::h_blank() {
    // Wait until end of H Blank mode
    if(ticks < 456)
        return;
    
    // reset ticks back to 0 for starting at beginning of next line
    ticks = 0;
    
    increment_ly();

    // bottom of screen reached
    if(ly == 144) {
        // std::cout << "vblank sent\n";
        
        // switch mode to V Blank by setting lower 2 bits of lcd status
        SET_PPU_MODE(ppu_mode::v_blank)

        // send v blank interrupt
        interrupts.request_interrupt(VBLANK_INTERRUPT);

        // https://gbdev.io/pandocs/STAT.html#ff41--stat-lcd-status
        // if V Blank source interrupt (bit 4 of status), send STAT interrupt
        if(get_bit(control, 4))
            interrupts.request_interrupt(LCD_STAT_INTERRUPT);
        
        return;
    }

    // we can still write next line (bottom of screen not reached)
    // switch mode to OAM Scan by setting lower 2 bits of lcd status
    SET_PPU_MODE(ppu_mode::oam_scan)
}

/* V BLANK
    For the last 10 lines, wait the number of ticks left    
*/
void PPU::v_blank() {
    // Wait until end of line
    if(ticks < 456)
        return;

    // reset ticks back to 0 for starting at beginning of next line
    ticks = 0;
    
    increment_ly();

    // end of V Blank, frame finished
    if(ly == 154) {
        // reset ly and window ly for next frame
        ly = 0;
        window_ly = 0;
        
        // switch mode to OAM Scan by setting lower 2 bits of lcd status
        SET_PPU_MODE(ppu_mode::oam_scan)
    }
}


/* PPU MODE HELPERS */
bool PPU::is_stat_on() {
    // holds mask to AND with STAT
    BYTE mask = get_bit(status, 2) ? 1 << 6 : 0;

    mask |= 1 << (status & 0b11 + 3);

    bool stat_on = mask & status;

    // send STAT interrupt when going from low to high
    if(!previous_stat_on && stat_on) {
        interrupts.request_interrupt(LCD_STAT_INTERRUPT);
    }

    previous_stat_on = stat_on;

    return stat_on;
}

bool PPU::is_window_visible() {
    return (
        // https://gbdev.io/pandocs/LCDC.html#lcdc5--window-enable
        // window bits
        get_bit(control, 0) && // bg and window enable
        get_bit(control, 5) && // window

        // https://gbdev.io/pandocs/Scrolling.html#ff4aff4b--wy-wx-window-y-position-x-position-plus-7
        // window criteria
        ly >= wy &&
        (wx >= 0 && wx <= 166) &&
        (wy >= 0 && wy <= 143)
    );
}

void PPU::increment_ly() {
    // increment window ly if visible
    if(is_window_visible()) {
        window_ly += 1;
    }

    // increment ly
    ly += 1;

    // set LYC=LY flag (bit 2 of status)
    status = set_bit(status, 2, lyc == ly);

    // check LYC=LY STAT interrupt source
    if(get_bit(status, 6))
        interrupts.request_interrupt(LCD_STAT_INTERRUPT);
}

void PPU::write_line() {

    // https://gbdev.io/pandocs/LCDC.html
    // window and background bit of LCD Control
    if(get_bit(control, 0)) {
        // tracks current pixel in the line
        int x = 0;
        
        while(x < 160) {
            // hold line of 8x8 tile being rendered
            int tile_y;
            // hold column of 8x8 tile being rendered
            int tile_x;

            // hold address of tile within tilemap (32 x 32) --> (0 ... 1023) + tile_map_address_start
            WORD tile_map_address;

            // http://pixelbits.16-b.it/GBEDG/ppu/
            // background and window pixel fetching
            if(is_window_visible() && x + 7 >= wx) {
                // https://gbdev.io/pandocs/Scrolling.html#ff4aff4b--wy-wx-window-y-position-x-position-plus-7
                // window conditions

                // get window tilemap address start (bit 6 of control decides 9C00 vs 9800)
                tile_map_address = get_bit(control, 6) ? 0x9C00 : 0x9800;

                // factor in the window tile's position in the current line
                tile_map_address += (x + 7 - wx) / 8;

                // factor in the window tile's position in the current column
                tile_map_address += 32 * (window_ly) / 8;

                tile_x = (x + 7 - wx) % 8;
                tile_y = window_ly % 8;
            } else {
                // get background tilemap address start (bit 3 of control decides 9C00 vs 9800)
                tile_map_address = get_bit(control, 3) ? 0x9C00 : 0x9800;

                // factor in the background tile's position in the current line
                // we mod 32 because there are 32 tiles in a line (numbered 0 - 31)
                tile_map_address += ((x + scx) / 8) % 32;

                // factor in the background tile's position in the current column
                // we mod 32 because there are 32 tiles in a column (numbered 0 - 31)
                tile_map_address += 32 * (((ly + scy) / 8) % 32);

                tile_x = (x + scx) % 8;
                tile_y = (ly + scy) % 8;
            }

            // https://gbdev.io/pandocs/Tile_Data.html
            // get tile id used to get the address of where the tile data
            BYTE tile_id = vram[tile_map_address - 0x8000];
            WORD tile_address;
            if(get_bit(control, 4)) {
                // 8000 addressing
                // 0x8000 + 16 bytes per tile * tile_id + 2 bytes per tile line
                tile_address = 0x8000 + 16 * tile_id + 2 * tile_y;
            } else {
                // 8800 addressing
                // 0x9000 + 16 bytes per tile * tile_id + 2 bytes per tile line
                tile_address = 0x9000 + 16 * (SIGNED_BYTE) tile_id + 2 * tile_y;
            }

            // gameboy is little endian --> lsb comes first
            BYTE tile_lo = vram[tile_address - 0x8000];
            BYTE tile_hi = vram[tile_address + 1 - 0x8000];

            for(; tile_x < 8; tile_x++, x++) {
                if(x >= 160)
                    break;

                // store 2bpp pixel value
                line[x][0] = (get_bit(tile_hi, (7 - tile_x)) << 1) | get_bit(tile_lo, (7 - tile_x));
                // store 0 for background and window
                line[x][1] = 0;
            }
        }
    }

    if(get_bit(control, 1)) {
        // render sprites
        for(int i = 0; i < 10; i++) {
            WORD address = sprite_addresses[i]; // first tile is top of sprite for h == 16

            if(address == 0)
                break;

            BYTE y = vram[address - 0x8000],
                x = vram[address + 1 - 0x8000],
                tile_index = vram[address + 2 - 0x8000],
                flags = vram[address + 3 - 0x8000];
            
            if(x == 0 || x >= 168)
                continue;

            if(get_bit(control, 2)) {
                tile_index &= 0xFE;
            } 

            U32 offset = ly - (y - 16); // ly - (y - 16)
            U32 h = get_bit(control, 2) ? 16 : 8;
            if(get_bit(flags, 6)) {
                offset = h - offset - 1;
            }

            BYTE line_lsb = vram[0x8000 + tile_index * 16 + offset * 2 - 0x8000];
            BYTE line_msb = vram[0x8000 + tile_index * 16 + offset * 2 + 1 - 0x8000];

            for(int p = 0; p < 8; p++) {
                if(x + p < 8 || x + p >= 168)
                    continue;
                int j = get_bit(flags, 5) ? x - 1 - p : x + p - 8;

                int k = ((line_msb >> (7 - p) & 0x01) << 1) | (line_lsb >> (7 - p) & 0x01);

                if( ( get_bit(flags, 7)  && (line[j][1] == 0 && line[j][0] >= 1 && line[j][0] <= 3) ) || 
                        (line[j][1] == 1 && line[j][3] <= x) || 
                        k == 0)
                    continue;

                line[j][0] = k;
                line[j][1] = 1; // 1 for sprite
                line[j][2] = get_bit(flags, 4);
                line[j][3] = x;
            }
        }
    }
    
    for(int x = 0; x < 160; x++) {
        BYTE palette;
        if(line[x][1]) {
            // sprite
            palette = line[x][2] ? obj_palette1 : obj_palette0;
        } else {
            // background or window
            palette = bg_palette;
        }
        
        // palette mappings from index to color
        int index_to_color[4] {
            (palette) & 0b11,
            (palette >> 2) & 0b11,
            (palette >> 4) & 0b11,
            (palette >> 6) & 0b11,
        };

        video[ly][x] = color[index_to_color[line[x][0]]];
    }
        
    // switch mode to H Blank by setting lower 2 bits of lcd status
    SET_PPU_MODE(ppu_mode::h_blank)
}