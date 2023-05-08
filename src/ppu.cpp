#include "ppu.h"

/* TICK MASTER LOOP */
void PPU::tick() {
    // sends STAT interrupt if needed
    is_stat_on();

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

    // https://gbdev.io/pandocs/Hardware_Reg_List.html?highlight=hardware%20registers#hardware-registers
    // some registers are R/W, R, or W
    if(address == 0xFF44) {
        // LY 0xFF44
        std::cerr << "Cannot write to read only register LY (0xFF44)\n";
        throw "Cannot write to read only register LY (0xFF44)";
    } else if(address == 0xFF41) {
        // status bits 3 - 6 R/W
        status &= 0b10000111;
        value = (status | (value & 0b01111000));
    } else if(address == 0xFF45) {
        status = set_bit(status, 2, value == ly);
    }

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
        case 0xFF41:
            return &status;
        case 0xFF44:
            return &ly;
        case 0xFF45:
            return &lyc;

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
    number_of_sprites = 0;
    
    for(WORD address = 0x0000; number_of_sprites < 10 && address < 0x00A0; address += 4) {
        // https://gbdev.io/pandocs/OAM.html?highlight=oam%20scan#selection-priority
        // each sprite is 4 bytes

        // lcd control bit 2 on ? 16 pixels : 8 pixels
        BYTE h = get_bit(control, 2) ? 16 : 8;

        // y = position of top of Sprite + 16
        BYTE y = oam[address];
        
        // position_y of Sprite = y - 16
        // validate Sprite
        if( (y - 16) <= ly && ly < (y - 16 + h) ) {
            // add Sprite to 10 Sprite list
            sprite_addresses[number_of_sprites] = address;
            number_of_sprites += 1;
        }
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
    // https://forums.nesdev.org/viewtopic.php?t=17754
    if(ticks < (172 + 10 * number_of_sprites))
        return;
    
    // clear bg, sprite buffer
    for(int i = 0; i < 160; i++)
        line[i][0] = 0;

    // USED DURING DEBUGGING
    // print_tile_maps();
    // write_tiles();

    write_line();

    // switch mode to H Blank by setting lower 2 bits of lcd status
    SET_PPU_MODE(ppu_mode::h_blank)
}

/* H BLANK
    For the current line (ly), wait the number of ticks left
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
        // send v blank interrupt
        interrupts.request_interrupt(VBLANK_INTERRUPT);

        // switch mode to V Blank by setting lower 2 bits of lcd status
        SET_PPU_MODE(ppu_mode::v_blank)
    } else {
        // we can still write next line (bottom of screen not reached)
        // switch mode to OAM Scan by setting lower 2 bits of lcd status
        SET_PPU_MODE(ppu_mode::oam_scan)
    }
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
    if(ly == 0) {
        // switch mode to OAM Scan by setting lower 2 bits of lcd status
        SET_PPU_MODE(ppu_mode::oam_scan)
    }
}


/* PPU MODE HELPERS */
bool PPU::is_stat_on() {
    // logically ORs all stat sources
    bool stat_on = (get_bit(status, 6) && get_bit(status, 2)) ||
                    (get_bit(status, 5) && ((status && 0b11) == ppu_mode::oam_scan)) ||
                    (get_bit(status, 4) && ((status && 0b11) == ppu_mode::v_blank)) ||
                    (get_bit(status, 3) && ((status && 0b11) == ppu_mode::h_blank));

    // send STAT interrupt when going from low to high
    if(!previous_stat_on && stat_on) {
        interrupts.request_interrupt(LCD_STAT_INTERRUPT);
    }

    previous_stat_on = stat_on;

    return stat_on;
}

/*
    Returns true if
        control(0), control(5), ly >= wy, ly < wy + 144, wx >= 0, wx <= 166, wy >= 0, wy <= 143
*/
bool PPU::is_window_visible() {
    return (
        // https://gbdev.io/pandocs/LCDC.html#lcdc5--window-enable
        // window bits
        get_bit(control, 0) && // bg and window enable
        get_bit(control, 5) && // window

        // https://gbdev.io/pandocs/Scrolling.html#ff4aff4b--wy-wx-window-y-position-x-position-plus-7
        // window criteria
        (ly >= wy) &&
        (ly <= wy + 143) &&
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

    if(ly == 154) {
        // reset ly and window ly for next frame
        ly = 0;
        window_ly = 0;
    }

    // set LYC=LY flag (bit 2 of status)
    status = set_bit(status, 2, lyc == ly);

    is_stat_on();
}

void PPU::print_tile_maps() {
    // https://gbdev.io/pandocs/Tile_Maps.html
    // 9800 - 9BFF 32 x 32 tile map
    std::cout << "\n0x9800 - 0x9BFF Tile Map\n";
    int counter = 0;
    for(WORD address = 0x9800; address < 0x9C00; address++, counter++) {
        if((counter % 32) == 0) {
            std::cout << '\n';
        }
        std::cout << std::hex << (int) vram[address - 0x8000] << ' ';
    }
    std::cout << '\n';

    // 9C00 - 9FFF 32 x 32 tile map
    std::cout << "\n0x9C00 - 0x9FFF Tile Map\n";
    for(WORD address = 0x9C00; address <= 0x9FFF; address++, counter++) {
        if((counter % 32) == 0) {
            std::cout << '\n';
        }
        std::cout << std::hex << (int) vram[address - 0x8000] << ' ';
    }
}

void PPU::write_tiles() {

    int x = 0, y = 0;
    for(WORD address = 0x0000; address < 0x1800; address += BYTES_PER_TILE) {
        for(int i = 0; i < 8; i++) {
            BYTE tile_lo = vram[address + 2 * i];
            BYTE tile_hi = vram[address + 2 * i + 1];
            
            for(int p = 0; p < 8; p++) {
                BYTE pixel_value = (get_bit(tile_hi, (7 - p)) << 1) | get_bit(tile_lo, (7 - p));
                
                // palette mappings from index to color
                int index_to_color[4] {
                    (bg_palette) & 0x03,
                    (bg_palette >> 2) & 0x03,
                    (bg_palette >> 4) & 0x03,
                    (bg_palette >> 6) & 0x03,
                };

                video[y + i][164 + x + p] = color[index_to_color[pixel_value]];
            }
        }
        x += 8;
        if(x == 128) {
            y += 8;
            x = 0;
        }
    }
}

void PPU::write_line() {

    // https://gbdev.io/pandocs/LCDC.html
    // window and background bit of LCD Control
    // handle window and background
    if(get_bit(control, 0)) {
        // tracks current pixel in the line
        int x = 0;
        
        while(x < 160) {
            // hold line of 8x8 tile being rendered
            int tile_line;
            // hold column of 8x8 tile being rendered
            int tile_column;

            // hold address of tile within tilemap (32 x 32) --> (0 ... 1023) + tile_map_address_start
            WORD tile_map_address;

            // http://pixelbits.16-b.it/GBEDG/ppu/
            // background and window pixel fetching
            if(is_window_visible() && (x + 7) >= wx) {
                // https://gbdev.io/pandocs/Scrolling.html#ff4aff4b--wy-wx-window-y-position-x-position-plus-7
                // window conditions

                // get window tilemap address start (bit 6 of control decides 9C00 vs 9800)
                tile_map_address = get_bit(control, 6) ? 0x9C00 : 0x9800;

                // factor in the window tile's position in the current line
                tile_map_address += (x + 7 - wx) / 8;

                // factor in the window tile's position in the current column
                tile_map_address += 32 * (window_ly / 8);

                tile_column = (x + 7 - wx) % 8;
                tile_line = window_ly % 8;
            } else {
                // get background tilemap address start (bit 3 of control decides 9C00 vs 9800)
                tile_map_address = get_bit(control, 3) ? 0x9C00 : 0x9800;

                // factor in the background tile's position in the current line
                // we mod 32 because there are 32 tiles in a line (numbered 0 - 31)
                tile_map_address += ((x + scx) / 8) % 32;

                // factor in the background tile's position in the current column
                // we mod 32 because there are 32 tiles in a column (numbered 0 - 31)
                tile_map_address += 32 * (((ly + scy) / 8) % 32);

                tile_column = (x + scx) % 8;
                tile_line = (ly + scy) % 8;
            }

            // https://gbdev.io/pandocs/Tile_Data.html
            // get tile id used to get the address of where the tile data
            BYTE tile_id = vram[tile_map_address - 0x8000];
            WORD tile_address;

            if(get_bit(control, 4)) {
                // 8000 addressing
                // 0x8000 + 16 bytes per tile * tile_id + 2 bytes per tile line
                tile_address = 0x8000 + BYTES_PER_TILE * tile_id + BYTES_PER_TILE_LINE * tile_line;
            } else {
                // 8800 addressing
                // 0x9000 + 16 bytes per tile * tile_id + 2 bytes per tile line
                tile_address = 0x9000 + BYTES_PER_TILE * (SIGNED_BYTE) tile_id + BYTES_PER_TILE_LINE * tile_line;
            }

            // gameboy is little endian --> lsb comes first
            BYTE tile_lo = vram[tile_address - 0x8000];
            BYTE tile_hi = vram[tile_address + 1 - 0x8000];

            for(; tile_column < 8; tile_column++, x++) {
                if(x >= 160)
                    break;

                // store 2bpp pixel value
                line[x][0] = (get_bit(tile_hi, (7 - tile_column)) << 1) | get_bit(tile_lo, (7 - tile_column));
                // store 0 for background and window
                line[x][1] = 0;

                // if(!(is_window_visible() && (x + 7) >= wx))
                //     line[x][0] = 0;
            }
        }
    }

    // https://gbdev.io/pandocs/LCDC.html
    // sprite enable bit
    // handle sprites
    if(get_bit(control, 1)) {
        // render sprites
        for(int i = 0; i < number_of_sprites; i++) {
            WORD sprite_address = sprite_addresses[i];
            if(sprite_address == NULL_SPRITE_ADDRESS)
                continue;

            // assume all sprites at this point are valid (not off screen etc.)

            // https://gbdev.io/pandocs/OAM.html
            // sprite decoding

            // sprite_y = y position on screen + 16
            BYTE sprite_y = oam[sprite_address];
            // sprite_x = x position on screen + 8
            BYTE sprite_x = oam[sprite_address + 1];

            BYTE tile_id = oam[sprite_address + 2];
            BYTE sprite_flags = oam[sprite_address + 3];

            // ignore bit 0 for 8x16 objects
            if(get_bit(control, 2))
                tile_id &= 0xFE;

            // line of the sprite to render
            BYTE sprite_line = ly - (sprite_y - 16);

            // handle vertically flipped sprites
            if(get_bit(sprite_flags, 6)) {
                sprite_line = (get_bit(control, 2) ? 16 : 8) - sprite_line - 1;
            }

            BYTE tile_lo = vram[BYTES_PER_TILE * tile_id + BYTES_PER_TILE_LINE * sprite_line];
            BYTE tile_hi = vram[BYTES_PER_TILE * tile_id + BYTES_PER_TILE_LINE * sprite_line + 1];

            for(BYTE sprite_column = 0; sprite_column < 8; sprite_column++) {
                // get current x position on screen
                BYTE x = sprite_x + sprite_column;
                if(x < 8 || x >= 168)
                    continue;
                // x = sprite_x - 8
                x -= 8;

                BYTE column = 7 - sprite_column;
                // handle horizontally flipped sprites
                if(get_bit(sprite_flags, 5)) {
                    column = sprite_column;
                }

                // 2bpp pixel value
                BYTE p = (get_bit(tile_hi, column) << 1) | get_bit(tile_lo, column);
                
                if(// bit7 bg and window over sprite
                    (get_bit(sprite_flags, 7) && line[x][1] == 0 && (line[x][0] >= 1 && line[x][1] <= 3)) ||
                    // pixel is transparent
                    (p == 0) ||
                    // same priority x coordinate
                    (line[x][1] == 1 && sprite_x >= line[x][3])
                    ) 
                    continue;
                
                
                // store 2bpp pixel value
                line[x][0] = p;
                // store 1 for sprite
                line[x][1] = 1;
                // store which palette to use
                line[x][2] = get_bit(sprite_flags, 4);
                // store sprite_x to compare to other sprites
                line[x][3] = sprite_x;

            }
        }
    }
    
    // render line onto screen
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
            (palette) & 0x03,
            (palette >> 2) & 0x03,
            (palette >> 4) & 0x03,
            (palette >> 6) & 0x03,
        };

        video[ly][x] = color[index_to_color[line[x][0]]];
    }
}