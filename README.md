# Gameboy Emulator

## Mar 31
Start with emulating the CPU

## Apr 2
PPU how to draw lines?

Screen
- 160 x 144 pixels
- 4 shades of color
- 8 x 8 pixel tile based, 20 x 18 tiles
- 40 sprites per line
- 8 KB VRAM

Tile
- 8 x 8 pixels
- each pixel can be 4 shades of color
- 2 bits per pixel
- bit mapping is in pallete

Background Tile Data
- 256 tiles stored in memory
- 32 x 32 tiles backgorund map

Window


8 KB VRAM Memory Map
- 4 KB Sprite Tiles
- 4 KB BG Tiles
- 1 KB BG Map
- 1 KB Window Map
- Overlap addresses for memory map to fit:
    - LCDC FF40 bit decides the config 


Write Per Line
OAM Search --> Pixel Transfer --> H-Blank
- CPU can't access VRAM in certain modes

OAM Search (20 cycles)
- must decide which sprites visible in **current line**
- put visible sprites into array of **max 10 sprites**
- logic for checking is x can't be 0 and current line must be in between sprite's start line and end line
- CPU can access VRAM
- CPU can't access OAM RAM

Pixel Transfer
- CPU can't access VRAM
- CPU can't access OAM RAM
    Pixel FIFO
    - must contain more than 8 pixels to pop a pixel out
    - Push and Fetch steps done at the same time
    - 2 pushes (1 pixel each) per 1 fetch
    - 3 fetches needed to get tile data
    - 8 pushes per 3 fetces (3 reads and 1 idle)

H Blank
