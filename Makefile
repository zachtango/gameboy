
INCLUDE = -Iinclude -lSDL2 -L/opt/homebrew/Cellar/sdl2/2.26.5/lib -I/opt/homebrew/Cellar/sdl2/2.26.5/include
SRC = src

CFLAGS = -std=c++20 -w -g 

gameboy: main.o timer.o ppu.o interrupts.o joypad.o helpers.o cartridge.o cpu.o mmu.o
	g++ ${CFLAGS} ${INCLUDE} main.o timer.o ppu.o interrupts.o joypad.o helpers.o cartridge.o cpu.o mmu.o -o gameboy.exe

main.o: $(SRC)/main.cpp
	g++ ${CFLAGS} -c ${INCLUDE} $(SRC)/main.cpp -o main.o

mmu.o: $(SRC)/mmu.cpp
	g++ ${CFLAGS} -c ${INCLUDE} $(SRC)/mmu.cpp -o mmu.o

cpu.o: $(SRC)/cpu.cpp
	g++ ${CFLAGS} -c ${INCLUDE} $(SRC)/cpu.cpp -o cpu.o

cartridge.o: $(SRC)/cartridge.cpp
	g++ ${CFLAGS} -c ${INCLUDE} $(SRC)/cartridge.cpp -o cartridge.o

timer.o: $(SRC)/timer.cpp
	g++ ${CFLAGS} -c ${INCLUDE} $(SRC)/timer.cpp -o timer.o

ppu.o: $(SRC)/ppu.cpp
	g++ ${CFLAGS} -c ${INCLUDE} $(SRC)/ppu.cpp -o ppu.o

interrupts.o: $(SRC)/interrupts.cpp
	g++ ${CFLAGS} -c ${INCLUDE} $(SRC)/interrupts.cpp -o interrupts.o

joypad.o: $(SRC)/joypad.cpp
	g++ ${CFLAGS} -c ${INCLUDE} $(SRC)/joypad.cpp -o joypad.o

helpers.o: $(SRC)/helpers.cpp
	g++ ${CFLAGS} -c ${INCLUDE} $(SRC)/helpers.cpp -o helpers.o

clean:
	rm *.o