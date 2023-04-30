#include <deque>
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
        
    */

    // 0b1001 0001
    lcdc = mmu.read(0xFF40);
    scy = mmu.read(0xFF42);
    scx = mmu.read(0xFF43);

    if(!get_bit(lcdc, 0))
        return;

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

                video[ly][j] = pallete[((line_msb >> (7 - p) & 0x01) << 1) | (line_lsb >> (7 - p) & 0x01)];

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

                video[ly][j] = pallete[((line_msb >> (7 - p) & 0x01) << 1) | (line_lsb >> (7 - p) & 0x01)];

                j += 1;
            }
        }
    }
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

                    tiles[(i * 8) + l][(j * 8) + p] = pallete[c];
                }

                tile_address += 2;
            }

            offset += 1;
        }
    }
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
    BYTE lcd = mmu.read(0xFF41);
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
            // collect sprite data

            if(ticks == 80) {
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

