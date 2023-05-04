#include "ppu.h"


/* TICK MASTER LOOP */
void PPU::tick() {
    ticks += 1;

    switch(mode) {
        case ppu_mode::oam_scan:
            oam_scan();
            break;
        case ppu_mode::pixel_transfer:
            pixel_transfer();
            break;
        case ppu_mode::h_blank:
            h_blank();
            break;
        case ppu_mode::v_blank:
            v_blank();
            break;
    }

    throw "Unknown mode";
}

/* MEMORY FUNCTIONS */
BYTE PPU::read(WORD address) {
    BYTE *p = memory_map(address);
    return *p;
}

void PPU::write(WORD address, BYTE value) {
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
    
    int counter = 0;
    
    for(WORD address = 0xFE00; counter < 10 && address < 0xFEA0; address += 4) {
        // lcd control bit 2 on ? 16 pixels : 8 pixels
        BYTE h = get_bit(control, 2) ? 16 : 8;

        // y = position of top of Sprite + 16
        BYTE y = read(address);
        
        // x = postiion of left of Sprite + 8
        BYTE x = read(address + 1);
        
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

void PPU::increment_ly() {
    // increment window ly if visible
    if(
        // https://gbdev.io/pandocs/LCDC.html#lcdc5--window-enable
        // window bits
        get_bit(control, 0) && // bg and window enable
        get_bit(control, 5) && // window

        // https://gbdev.io/pandocs/Scrolling.html#ff4aff4b--wy-wx-window-y-position-x-position-plus-7
        // window criteria
        ly >= wy &&
        (wx >= 0 && wx <= 166) &&
        (wy >= 0 && wy <= 143)
    ) {
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

    WORD tile_address_start = get_bit(control, 4) ? 0x8000 : 0x9000;
    
    WORD bg_tile_map_start = get_bit(control, 3) ? 0x9C00 : 0x9800;

    // (scy + ly) / 8 rows * 32 tiles / row
    bg_tile_map_start += (scy + ly) / 8 * 32;

    // window visibility 
    // https://gbdev.io/pandocs/Scrolling.html#ff4aff4b--wy-wx-window-y-position-x-position-plus-7
    bool window_visible = get_bit(control, 5) && 
                            (wx >= 0 && wx <= 166) && 
                            (wy >= 0 && wy < 144);

    WORD window_tile_map_start = get_bit(control, 6) ? 0x9C00 : 0x9800;

    // window_ly rows * 32 tiles / row
    window_tile_map_start += window_ly / 8 * 32;

    // https://gbdev.io/pandocs/LCDC.html
    if(get_bit(control, 0)) {
        int j = 0;

        while(j < 160) {
            int tile_pixel = ((scx + j) % 256);
            U32 offset = (scy + ly) % 8;
            BYTE tile_number = read(bg_tile_map_start + tile_pixel / 8);
            WORD tile_address;

            if(window_visible && ly >= wy && (j + 7) >= wx) {
                // window nums
                tile_pixel = (j + 7 - wx);
                tile_number = read(window_tile_map_start + tile_pixel / 8);
                offset = (window_ly % 8) * 2;
            }

            tile_address = 0x8800 + (-128 + tile_number) * 16 + ((scy + ly) % 8) * 2;
            if(get_bit(control, 4) || tile_number < 128) {
                tile_address = tile_address_start + tile_number * 16 + ((scy + ly) % 8) * 2; // tile_address_start + tile_number * 16 bits / tiles + (scy + ly) % 8 * 2 bits (individual lines of tile offset)
            }

            BYTE line_lsb = read(tile_address);
            BYTE line_msb = read(tile_address + 1);

            for(int p = tile_pixel % 8; p < 8; p++) {
                if(j >= 160) {
                    break;
                }

                line[j][0] = ((line_msb >> (7 - p) & 0x01) << 1) | (line_lsb >> (7 - p) & 0x01);
                line[j][1] = 0; // 0 for bg

                j += 1;
            }
        }
    }

    if(get_bit(control, 1)) {
        // render sprites
        for(int i = 0; i < 10; i++) {
            WORD address = sprite_addresses[i]; // first tile is top of sprite for h == 16

            if(address == 0)
                break;

            BYTE y = read(address),
                x = read(address + 1),
                tile_index = read(address + 2),
                flags = read(address + 3);
            
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

            BYTE line_lsb = read(0x8000 + tile_index * 16 + offset * 2);
            BYTE line_msb = read(0x8000 + tile_index * 16 + offset * 2 + 1);

            for(int p = 0; p < 8; p++) {
                if(x + p < 8 || x + p >= 168)
                    continue;
                int j = get_bit(flags, 5) ? x - 1 - p : x + p - 8;

                int k = ((line_msb >> (7 - p) & 0x01) << 1) | (line_lsb >> (7 - p) & 0x01);

                if( ( get_bit(flags, 7)  && (line[j][1] == 0 && line[j][0] >= 1 && line[j][0] <= 3) ) || 
                        (line[j][1] == 1 && line[j][3] <= x) || 
                        k == 0)
                    continue;

                // if(ly >= 88) {
                //     std::cout << std::dec << (int) y << ' ' << (int)k << " tile index\n";
                // }

                line[j][0] = k;
                line[j][1] = 1; // 1 for sprite
                line[j][2] = get_bit(flags, 4);
                line[j][3] = x;
            }
        }
    }
    
    // std::cout << std::dec << (int) ly << ' ';
    for(int j = 0; j < 160; j++) {
        if(line[j][1]) {
            // sprite
            auto palette = line[j][2] ? obj_palette1 : obj_palette0;
            // std::cout << j << ' ' << (int) line[j][2] << ' ';
            video[ly][j] = color[line[j][0]];
        } else {
            // bg
            video[ly][j] = color[line[j][0]];
        }
    }
    
    // switch mode to H Blank by setting lower 2 bits of lcd status
    SET_PPU_MODE(ppu_mode::h_blank)
}