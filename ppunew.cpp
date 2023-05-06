#include <algorithm>
#include "ppu.h"

struct Tile {
    BYTE line[16];
};


void PPU::init_logo() {
    
    BYTE tile_data[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xF0, 0x00, 0xF0, 0x00, 0xFC, 0x00, 0xFC, 0x00, 0xFC, 0x00, 0xFC, 0x00, 0xF3, 0x00, 0xF3, 0x00,
        0x3C, 0x00, 0x3C, 0x00, 0x3C, 0x00, 0x3C, 0x00, 0x3C, 0x00, 0x3C, 0x00, 0x3C, 0x00, 0x3C, 0x00,
        0xF0, 0x00, 0xF0, 0x00, 0xF0, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF3, 0x00, 0xF3, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCF, 0x00, 0xCF, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x0F, 0x00, 0x3F, 0x00, 0x3F, 0x00, 0x0F, 0x00, 0x0F, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0x0F, 0x00, 0x0F, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x00, 0xF0, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF3, 0x00, 0xF3, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x00, 0xC0, 0x00,
        0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0xFF, 0x00, 0xFF, 0x00,
        0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC3, 0x00, 0xC3, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x00, 0xFC, 0x00,
        0xF3, 0x00, 0xF3, 0x00, 0xF0, 0x00, 0xF0, 0x00, 0xF0, 0x00, 0xF0, 0x00, 0xF0, 0x00, 0xF0, 0x00,
        0x3C, 0x00, 0x3C, 0x00, 0xFC, 0x00, 0xFC, 0x00, 0xFC, 0x00, 0xFC, 0x00, 0x3C, 0x00, 0x3C, 0x00,
        0xF3, 0x00, 0xF3, 0x00, 0xF3, 0x00, 0xF3, 0x00, 0xF3, 0x00, 0xF3, 0x00, 0xF3, 0x00, 0xF3, 0x00,
        0xF3, 0x00, 0xF3, 0x00, 0xC3, 0x00, 0xC3, 0x00, 0xC3, 0x00, 0xC3, 0x00, 0xC3, 0x00, 0xC3, 0x00,
        0xCF, 0x00, 0xCF, 0x00, 0xCF, 0x00, 0xCF, 0x00, 0xCF, 0x00, 0xCF, 0x00, 0xCF, 0x00, 0xCF, 0x00,
        0x3C, 0x00, 0x3C, 0x00, 0x3F, 0x00, 0x3F, 0x00, 0x3C, 0x00, 0x3C, 0x00, 0x0F, 0x00, 0x0F, 0x00,
        0x3C, 0x00, 0x3C, 0x00, 0xFC, 0x00, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x00, 0xFC, 0x00,
        0xFC, 0x00, 0xFC, 0x00, 0xF0, 0x00, 0xF0, 0x00, 0xF0, 0x00, 0xF0, 0x00, 0xF0, 0x00, 0xF0, 0x00,
        0xF3, 0x00, 0xF3, 0x00, 0xF3, 0x00, 0xF3, 0x00, 0xF3, 0x00, 0xF3, 0x00, 0xF0, 0x00, 0xF0, 0x00,
        0xC3, 0x00, 0xC3, 0x00, 0xC3, 0x00, 0xC3, 0x00, 0xC3, 0x00, 0xC3, 0x00, 0xFF, 0x00, 0xFF, 0x00,
        0xCF, 0x00, 0xCF, 0x00, 0xCF, 0x00, 0xCF, 0x00, 0xCF, 0x00, 0xCF, 0x00, 0xC3, 0x00, 0xC3, 0x00,
        0x0F, 0x00, 0x0F, 0x00, 0x0F, 0x00, 0x0F, 0x00, 0x0F, 0x00, 0x0F, 0x00, 0xFC, 0x00, 0xFC, 0x00,
        0x3C, 0x00, 0x42, 0x00, 0xB9, 0x00, 0xA5, 0x00, 0xB9, 0x00, 0xA5, 0x00, 0x42, 0x00, 0x3C, 0x00,
    };

    BYTE tile_map[] = {
        0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C,
        0x19, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
    };

    for(WORD i = 0x8000; i < 0x81A0; i++) {
        mmu.write(i, tile_data[i - 0x8000]);
    }

    for(WORD i = 0x9900; i < 0x9930; i++) {
        mmu.write(i, tile_map[i - 0x9900]);
    }
    
}

void PPU::render_line() {
    init_palletes();
    /*
        VRAM Tile Data

        Block	VRAM Address
        0	    $8000–$87FF
        1	    $8800–$8FFF
        2	    $9000–$97FF

        16 x 24 Tiles

        2 Addressing Methods:
        8000 method
        - 8000 as its base pointer and unsigned addressing
        - tiles 0 - 127 are in block 0
        - tiles 128 - 255 are in block 1

        8800 method
        - 9000 as its base pointer and signed addressing
        - tiles 0 - 127 are in block 2
        - tiles -128 to -1 are in block 1

        each tile 8x8 is 16 bytes:
        - each line represents 2 bytes
        - bit 7 is leftmost pixel bit 0 is rightmost pixel
        
        VRAM Tile Maps
        32 x 32 tile maps in VRAM at
            9800 - 9BFF and 9C00 - 9FFF
        - these maps are used to display background or window

        Tile Indexes
        - Each tile map contains 1 byte
            indexes of the tiles to be displayed
        - Tiles obtained from Tile Data Table using the address mode
            determined by the LCDC register
        
        LCDC 3 - BG Tile Map Area
        - bit is reset --> BG uses tilemap 9800
        - bit is set --> BG uses 9C00


        Sprites
        Tile data 0x8000 - 0x8FFF
        Attribute Table (OAM) 0xFE00 - 0xFE9F (40 entries of 4 bytes, 160 bytes total)
            byte 0 - vertical position on screen + 16
                Sprite's vertical position + 16 (Y = 0 hides a sprite, Y = 2 hides an 8x8 sprite)
                Y = 160 hides a sprite
            byte 1 - horizontal position on screen + 8
                Width of sprite is always 8
                X = 0, X = 168 hides sprite
                X sprites still affect priority even if hidden (better way to hide sprite is set Y)
            byte 2 - tile index
                8x8 mode (LCDC bit 2 = 0) --> byte specifies sprite's only tile index
                    selects tile from memory area at 8000 - 8FFF
                
                8x16 mode (LCDC bit 2 = 1) --> byte specifies 2 tiles
                    selects tile from memory area at 8000 - 8FFF
                    first tile is top of sprite, second tile is bottom
            byte 3 - attributes/flags
                https://gbdev.io/pandocs/OAM.html

            Sprite priority while drawing
                Smaller X coordinate has higher priority
                    object first in OAM has higher prio if X is equal
                
                1. OBJ prio is first chosen
                2. OBJ pixel has BG over OBJ attribute checked to determine
                    whether is should be drawn over the background
    */

    // 0b1001 0001
    lcdc = mmu.read(0xFF40);
    scy = mmu.read(0xFF42);
    scx = mmu.read(0xFF43);

    WORD tile_address_start = get_bit(lcdc, 4) ? 0x8000 : 0x9000;
    
    WORD bg_tile_map_start = get_bit(lcdc, 3) ? 0x9C00 : 0x9800;
    bg_tile_map_start += (scy + ly) / 8 * 32; // (scy + ly) / 8 rows * 32 tiles / row

    // render window
    BYTE wy = mmu.read(0xFF4A);
    BYTE wx = mmu.read(0xFF4B);

    // window visibility 
    // https://gbdev.io/pandocs/Scrolling.html#ff4aff4b--wy-wx-window-y-position-x-position-plus-7
    bool window_visible = get_bit(lcdc, 5) && 
                            (wx >= 0 && wx <= 166) && 
                            (wy >= 0 && wy < 144);

    WORD window_tile_map_start = get_bit(lcdc, 6) ? 0x9C00 : 0x9800;
    window_tile_map_start += window_ly / 8 * 32; // window_ly rows * 32 tiles / row

    int j = 0;
    // std::cout << std::dec << (int) window_ly << ' ' << (int) (ly - wy) << '\n';
    if(get_bit(lcdc, 0)) {
        while(j < 160) {
            if(window_visible && ly >= wy && (j + 7) >= wx) {
                // window
                int tile_pixel = (j + 7 - wx);
                BYTE tile_number = mmu.read(window_tile_map_start + tile_pixel / 8);
                WORD tile_address;

                if(get_bit(lcdc, 4) || tile_number < 128) {
                    tile_address = tile_address_start + tile_number * 16 + (window_ly % 8) * 2; // tile_address_start + tile_number * 16 bits / tiles + window_ly % 8 * 2 bits (individual lines of tile offset)
                } else {
                    tile_address = 0x8800 + (-128 + tile_number) * 16 + (window_ly % 8) * 2;
                }

                BYTE line_lsb = mmu.read(tile_address);
                BYTE line_msb = mmu.read(tile_address + 1);

                for(int p = tile_pixel % 8; p < 8; p++) {
                    if(j >= 160) {
                        break;
                    }

                    line[j][0] = ((line_msb >> (7 - p) & 0x01) << 1) | (line_lsb >> (7 - p) & 0x01);
                    line[j][1] = 0; // 0 for bg

                    j += 1;
                }
            } else {
                // background
                int tile_pixel = ((scx + j) % 256);
                BYTE tile_number = mmu.read(bg_tile_map_start + tile_pixel / 8);
                WORD tile_address;

                if(get_bit(lcdc, 4) || tile_number < 128) {
                    tile_address = tile_address_start + tile_number * 16 + ((scy + ly) % 8) * 2; // tile_address_start + tile_number * 16 bits / tiles + (scy + ly) % 8 * 2 bits (individual lines of tile offset)
                } else {
                    tile_address = 0x8800 + (-128 + tile_number) * 16 + ((scy + ly) % 8) * 2;
                }

                BYTE line_lsb = mmu.read(tile_address);
                BYTE line_msb = mmu.read(tile_address + 1);
                
                for(int p = tile_pixel % 8; p < 8; p++) {
                    if(j >= 160)
                        break;

                    line[j][0] = ((line_msb >> (7 - p) & 0x01) << 1) | (line_lsb >> (7 - p) & 0x01);
                    line[j][1] = 0; // 0 for bg

                    j += 1;
                }
            }
        }
    }

    if(get_bit(lcdc, 1)) {
        // render sprites
        for(int i = 0; i < 10; i++) {
            WORD address = sprites[i].first; // first tile is top of sprite for h == 16

            if(address == 0)
                break;

            BYTE y = mmu.read(address),
                x = sprites[i].second,
                tile_index = mmu.read(address + 2),
                flags = mmu.read(address + 3);
            
            if(x == 0 || x >= 168)
                continue;

            if(get_bit(lcdc, 2)) {
                tile_index &= 0xFE;
            } 

            UINT offset = ly - (y - 16); // ly - (y - 16)
            UINT h = get_bit(lcdc, 2) ? 16 : 8;
            if(get_bit(flags, 6)) {
                offset = h - offset - 1;
            }

            BYTE line_lsb = mmu.read(0x8000 + tile_index * 16 + offset * 2);
            BYTE line_msb = mmu.read(0x8000 + tile_index * 16 + offset * 2 + 1);

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
            video[ly][j] = palette[line[j][0]];
            // video[ly][j] = 0;
        } else {
            // bg
            video[ly][j] = bg_palette[line[j][0]];
        }
    }
    
    // std::cout << '\n';
}

void PPU::increment_y() {
    // window ly
    BYTE wy = mmu.read(0xFF4A);
    BYTE wx = mmu.read(0xFF4B);
    bool window_visible = get_bit(lcdc, 0) &&
                            get_bit(lcdc, 5) && 
                            (wx >= 0 && wx <= 166) && 
                            (wy >= 0 && wy < 144) &&
                            ly >= wy &&
                            ly < wy + 144;

    if(window_visible) {
        window_ly += 1;
    }

    // ly
    ly += 1;

    BYTE lyc = mmu.read(0xFF45);
    BYTE lcd = mmu.read(0xFF41);

    if(ly == lyc) {
        // Set bit 2 of STAT: LCD status register
        mmu.write(
            0xFF41,
            set_bit(lcd, 2, 1)
        );

        if( get_bit(lcd, 6) ) {
            // Send STAT interrupt
            mmu.write(
                0xFF0F,
                mmu.read(0xFF0F) | 0b00010
            );
        }
    } else {
        mmu.write(
            0xFF41,
            set_bit(lcd, 2, 0)
        );
    }
}

void PPU::tick() {
    BYTE lcdc = mmu.read(0xFF40),
        lcd = mmu.read(0xFF41);
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
    // std::cout << ticks << '\n';
    ticks += 1;
    switch(state) {
        case oam_search:
            if(ticks == 80) {
                // scan sprites from 0xFE00 to 0xFE9F, picking the first 10 suitable ones

                /*
                for each object, check:
                    1. Y coordinate --> if in range, render sprite
                        ly >= Y && ly <= Y + 16 if lcdc2 else Y + 8
                */
                int counter = 0;
                for(WORD address = 0xFE00; counter < 10 && address < 0xFEA0; address += 4) {
                    BYTE y = mmu.read(address),
                        x = mmu.read(address + 1);
                    UINT h = get_bit(lcdc, 2) ? 16 : 8;
                    
                    if(y - 16 <= ly && ly < y - 16 + h) {

                        sprites[counter].first = address;
                        sprites[counter].second = x;
                        if(h == 16 && ly > 72) {
                            // std::cout << ' ' << std::dec << (int) ly << " 8 x 16 sprite\n";
                            // std::cout << std::hex << (int) address << ' ' << std::dec << (int) x << '\n';
                        }
                        counter += 1;
                    }
                }

                for(; counter < 10; counter++) {
                    sprites[counter].first = 0;
                    sprites[counter].second = 0;
                }

                // pixel fetcher
                state = pixel_transfer;
                // set pixel transfer mode
                mmu.write(
                    0xFF41,
                    (mmu.read(0xFF41) & ~0b11) | 0b11
                );
            }
            break;
        case pixel_transfer:
            // push pixel data to display

            // don't need to render every time, only 60 fps

            
            if(ticks == 172) {
                if(*mmu.write_to_tile) {
                    // render_tiles();
                    render_line();
                }
                state = h_blank;
                // set h blank mode
                mmu.write(
                    0xFF41,
                    (mmu.read(0xFF41) & ~0b11)
                );

                if(get_bit(lcd, 3)) {
                    // send stat interrupt
                    mmu.write(
                        0xFF0F,
                        mmu.read(0xFF0F) | 0b00010
                    );
                }
            }

            break;
        case h_blank:

            if(ticks == 456) {
                ticks = 0;
                
                increment_y();

                if(ly == 144) {
                    // std::cout << "vblank sent\n";
                    state = v_blank;
                    // set vblank mode
                    mmu.write(
                        0xFF41,
                        (mmu.read(0xFF41) & ~0b11) | 0b01
                    );

                    // send v blank interrupt
                    mmu.write(
                        0xFF0F,
                        mmu.read(0xFF0F) | 0b00001
                    );

                    if(get_bit(lcd, 4)) {
                        // send stat interrupt
                        mmu.write(
                            0xFF0F,
                            mmu.read(0xFF0F) | 0b00010
                        );
                    }
                } else {
                    state = oam_search;
                    // set OAM mode
                    mmu.write(
                        0xFF41,
                        (mmu.read(0xFF41) & ~0b11) | 0b10
                    );
                }
            }

            break;
        case v_blank:
            mmu.write(
                0xFF41,
                (mmu.read(0xFF41) & ~0b11) | 0b01
            );

            // wait for 10 lines, go back to oam search
            if(ticks == 456) {
                ticks = 0;
                
                increment_y();

                if(ly == 154) {
                    // frame finished --> reset all values
                    ly = 0;
                    window_ly = 0;
                    
                    state = oam_search;
                    // set OAM mode
                    mmu.write(
                        0xFF41,
                        (mmu.read(0xFF41) & ~0b11) | 0b10
                    );
                }



            }

            break;
        default:
            throw "Unknown PPU State";
    }
    mmu.write(0xFF44, ly);
}


void PPU::render_tiles() {
    // bool flag = false;
    // for(int i = 0x8000; i < 0x9800; i++) {
    //     if(mmu.read(i) > 0) {
    //         flag = true;
    //         break;
    //     }
    // }

    // if(flag) {
    //     std::cout << "print non black\n";
    // }

    UINT total_tiles = 24 * 16;

    WORD tile_address_start = 0x8000;

    UINT offset = 0;
    for(int i = 0; i < 24; i++) {
        for(int j = 0; j < 16; j++) {
            WORD tile_address = mmu.read(tile_address_start + offset * 2);
            
            for(int l = 0; l < 8; l++) {
                BYTE line_lsb = mmu.read(tile_address);
                BYTE line_msb = mmu.read(tile_address + 1);
                
                for(int p = 0; p < 8; p++) {
                    /*
                        for a tile fill in each bit from top left to bottom right
                            for each byte in the tyle, leftmost pixel is bit 7 and rightmost pixel is bit 0
                    */
                    UINT c = ((line_msb >> (7 - p) & 0x01) << 1) | (line_lsb >> (7 - p) & 0x01);

                    tiles[(i * 8) + l][(j * 8) + p] = bg_palette[c];
                }

                tile_address += 2;
            }

            offset += 1;
        }
    }
}

void PPU::init_palletes() {
    BYTE bg = mmu.read(0xFF47);
    BYTE obj0 = mmu.read(0xFF48);
    BYTE obj1 = mmu.read(0xFF49);

    bg_palette[3] = color[ get_bit(bg, 7) << 1 | get_bit(bg, 6) ];
    bg_palette[2] = color[ get_bit(bg, 5) << 1 | get_bit(bg, 4) ];
    bg_palette[1] = color[ get_bit(bg, 3) << 1 | get_bit(bg, 2) ];
    bg_palette[0] = color[ get_bit(bg, 1) << 1 | get_bit(bg, 0) ];
    
    obj_palette0[3] = color[ get_bit(obj0, 7) << 1 | get_bit(obj0, 6) ];
    obj_palette0[2] = color[ get_bit(obj0, 5) << 1 | get_bit(obj0, 4) ];
    obj_palette0[1] = color[ get_bit(obj0, 3) << 1 | get_bit(obj0, 2) ];

    obj_palette1[3] = color[ get_bit(obj1, 7) << 1 | get_bit(obj1, 6) ];
    obj_palette1[2] = color[ get_bit(obj1, 5) << 1 | get_bit(obj1, 4) ];
    obj_palette1[1] = color[ get_bit(obj1, 3) << 1 | get_bit(obj1, 2) ];
}