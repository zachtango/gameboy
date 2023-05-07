#include "interrupts.h"


U32 Interrupts::handle_interrupt(CPU &cpu) {
    // check if interrupt master enable allows interrupts
    if(!cpu.interrupt_master_enable)
        return 0;
    
    BYTE enable_and_flag = interrupt_enable & interrupt_flag;
    // std::cout << (int) enable_and_flag << '\n';
    // https://gbdev.io/pandocs/Interrupts.html#interrupt-priorities
    // handle in order of priority
    if(enable_and_flag & VBLANK_INTERRUPT) {
        // std::cout << "VBLANK INT\n";
        // clear VBLANK interrupt flag
        interrupt_flag &= ~(VBLANK_INTERRUPT);

        // call ISR at address 0x0040
        cpu._call(0x0040);
        
    } else if(enable_and_flag & LCD_STAT_INTERRUPT) {
        // std::cout << "LCD_STAT INT\n";
        // clear LCD_STAT interrupt flag
        interrupt_flag &= ~(LCD_STAT_INTERRUPT);

        // call ISR at address 0x0048
        cpu._call(0x0048);
    } else if(enable_and_flag & TIMER_INTERRUPT) {
        // std::cout << "TIMER INT\n";
        // clear TIMER INT interrupt flag
        interrupt_flag &= ~(TIMER_INTERRUPT);

        // call ISR at address 0x0050
        cpu._call(0x0050);
    } else if(enable_and_flag & SERIAL_INTERRUPT) {
        // std::cout << "SERIAL INT\n";
        // clear SERIAL interrupt flag
        interrupt_flag &= ~(SERIAL_INTERRUPT);

        // call ISR at address 0x0058
        cpu._call(0x0058);
    } else if(enable_and_flag & JOYPAD_INTERRUPT) {
        // std::cout << "JOYPAD INT\n";
        // clear JOYPAD interrupt flag
        interrupt_flag &= ~(JOYPAD_INTERRUPT);

        // call ISR at address 0x0060
        cpu._call(0x0060);
    } else {
        // No interrupt
        return 0;
    }

    // disable interrupts
    // https://gbdev.io/pandocs/Interrupts.html
    cpu.interrupt_master_enable = false;

    // 20 T Cycles
    return 20;
}

void Interrupts::request_interrupt(BYTE interrupt_mask) {
    // set bit of Interrupt request in Interrupt flag
    interrupt_flag |= interrupt_mask;
}

/* MEMORY FUNCTIONS */
BYTE Interrupts::read(WORD address) {
    BYTE *p = memory_map(address);

    // bit 8 - 5 unused --> return 1s for those
    return *p | (0b11100000);
}
void Interrupts::write(WORD address, BYTE value) {
    BYTE *p = memory_map(address);
    *p = value;
}

/* MEMORY FUNCTIONS HELPER */
BYTE *Interrupts::memory_map(WORD address) {
    // std::cout << "address: " << (int) address << '\n';
    switch(address) {
        case 0xFFFF:
            // Interrupt enable register
            return &interrupt_enable;

        case 0xFF0F:
            // Interrupt flag register
            return &interrupt_flag;
    }

    std::cerr << std::hex << address << " Address not in Interrupts range\n";
    throw "Address not in Interrupts range";
}