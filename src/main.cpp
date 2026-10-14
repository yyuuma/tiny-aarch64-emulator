#include <iostream>
#include "cpu.hpp"
#include "memory.hpp"

// Rd     - Destination Register
// Rn/Rm  - Input Registers
// Rt     - Transfer Registers
// sf = 0 - 32-bit Variant
// sf = 1 - 64-bit Variant
// sh     - Left Shift applying to Immediate
// S = 1  - Instruction Updates the Condition Flags

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "Usage: tiny-aarch64-emulator [FILE].bin" << std::endl;
        return 0;
    }

    CPU cpu;
    Memory mem(argv[1]);
    while (cpu.is_running()) {
        cpu.execute(mem);
    }

    cpu.print_state();
}
