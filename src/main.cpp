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

int main(void) {
    CPU cpu;
    Memory mem("demo.bin");
    cpu.execute(mem);
    cpu.execute(mem);
    cpu.execute(mem);
    cpu.execute(mem);
    mem.print_hex();
    std::cout << '\n';
    cpu.print_state();
}
