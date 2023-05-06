#!/bin/bash

g++ -std=c++17 -w -lSDL2 -L/opt/homebrew/Cellar/sdl2/2.26.5/lib \
    -I/opt/homebrew/Cellar/sdl2/2.26.5/include \
    main.cpp  interrupts.h interrupts.cpp cartridge.h cartridge.cpp mmu.h mmu.cpp cpu.h timer.h ppu.h \
    cpu.cpp timer.cpp ppu.cpp helpers.h helpers.cpp \
    joypad.h joypad.cpp constants.h \
