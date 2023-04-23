#include <vector>
#include <string>
#include "mmu.h"
#include "cpu.h"

int main() {
    
    std::vector<std::string> v {
        "01-special.gb",
        "03-o-b",
    };

    int instr_count[] = {
        10000000,
        1500000,
        2000000,
        2000000,
        2000000,
        500000,
        300000,
        5000000,
        7000000,
        8000000,
    };

    for(int i = 0; i < v.size(); i++) {
        MMU mmu;
        mmu.read_rom(("gb-test-roms/cpu_instrs/individual/" + v[i]).c_str());
        // mmu.print_rom();
        CPU cpu = CPU(mmu, v[i]);
        
        int end;
        // std::cout << "How many instructions?\n";
        // std::cin >> end;
        int j = 0;
        while(j < instr_count[i]) { // 31463
            cpu.run_fde();
            j += 1;
        }

        // bool ok = true;

        // while(ok) {
        //     char c;
        //     std::cin >> c;

        //     if(c == 'y') {
        //         cpu.run_fde();
        //     } else {
        //         std::cout << "Enter in 2 byte address to see\n";

        //         UINT x;
        //         std::cin >> std::hex >> x;

        //         std::cout << std::hex << (int) mmu.read(x) << '\n';
        //     }
        // }

        cpu.out.close();
    }

    MMU mmu;
    mmu.read_rom("gb-test-roms/cpu_instrs/individual/1.gb");

    CPU cpu = CPU(mmu, "test");

    while(true) {
        // TO ADD PPU
        // ppu.render();

        cpu.run_fde();
    }

    return 0;
}

