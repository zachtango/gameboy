#include "mmu.h"
#include "cpu.h"

int main() {
    
    MMU mmu;

    mmu.read_rom("DMG_ROM.bin");
    
    CPU cpu = CPU(mmu);

    cpu.mmu.print_rom();

    int i = 0;
    while(i < 256) {
        cpu.run_fde(false);
        i += 1;
    }

    return 0;
}

