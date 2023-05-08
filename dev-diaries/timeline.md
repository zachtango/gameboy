
March 31 - April 2
- set up basic folder structure with all the devices
- defined MMU and viewed memory as one big array
- defined CPU instructions for executing
- defined CPU instruction mappings for decoding
- defined CPU fetch decode execute cycle

April 3 - April 9
- tested CPU instructions with Blargg tests
- debugged CPU logs with gameboy doctor

April 10 - April 16
- defined Interrupt types
- defined Interrupt request and handle

April 17 - April 23
- defined PPU states
- defined PPU decoding for background, window, and sprite tiles
- defined PPU interrupts (VBLANK, STAT)
- tested PPU with dmg-acid2

April 24 - April 30
- refactor code to be cleaner (looked so much better! should've done a lot of these things originally)
    by changing MMU to direct reads/writes to specific devices rather than storing one big
    internal memory array, so much easier to manage
- defined Joypad to handle user input
- defined Timer for "reacts" to timer oveflows
- defined Cartridge to handle loading ROMs
- defined MBC1 ROMs for Cartridge
- tested MBC1 ROMs with Mooneye MBC1 tests
- debug PPU after refactoring (much pain)

May 1 - May 7
- debug PPU, successfully renders Tetris and DrMario and passes dmg-acid2 test
- problems with Kirby's Dreamland (MBC1 ROM) graphics
    - background flickers between different tilemaps
    - control register of PPU has bit 4 set to on last time I checked the incorrect background flicker
    - I believe the control bit 4 is flickering between on and off, when it should be off the whole time
    - next steps: debug the control state with a correct emulator (SameBoy) and compare it to my state
