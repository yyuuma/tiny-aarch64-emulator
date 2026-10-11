#include <fstream>
#include <iostream>
#include <cstdint>
#include <format>

#include "types.hpp"
#include "bits.hpp"
#include "decode.hpp"
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
    Memory mem;
    cpu.print_gpr();
    mem.print_hex();
    cpu.write_x(0, 512);
    mem.write_32(0, 0x91005c01); // ADD r1 r0 #23;
    mem.write_32(4, 0xd1003008); // SUB r8 r0 #12;
    cpu.execute(cpu.fetch(mem), mem);
    mem.print_hex();
    cpu.print_gpr();
    cpu.execute(cpu.fetch(mem), mem);
    mem.print_hex();
    cpu.print_gpr();
}
