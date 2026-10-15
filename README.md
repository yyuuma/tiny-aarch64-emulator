# Tiny AArch64 Emulator
A small educational project written in C++20 dedicated to learning how A64 instructions are decoded and executed.

The program loads raw A64 instruction bytes into a 256 byte memory, executes the instructions and prints the final CPU state. See [Usage](#Usage).

Inspired by Dave's ["Emulating a 6502 CPU in C++"](https://www.youtube.com/watch?v=qJgsuQoy9bc&list=PLLwK93hM93Z13TRzPx9JqTIn33feefl37) Series.

## Current Support
- `ADD`/`SUB`
- `ADDS`/`SUBS` with proper NZCV condition flags
- `MOVN`/`MOVZ`/`MOVK` including `MOV` immediate aliases
- 31 General Purpose Registers
- 32-bit vs 64-bit register instructions

(It is tiny!)

## Missing Support
- System Registers
- Floating-point Registers
- Load & Store instructions
- Branch instructions
- Floating-point instructions
- SIMD instructions

## How to Build
### Currently only Linux x86_64 is supported.

### Prerequisites
- C++20 Compiler
- [CMake](https://cmake.org/) version >= 3.20

To run ARM assembly files in the emulator:

- AArch64 Linux GNU binutils

### Install Prerequisites
**Arch Linux**:
```sh
sudo pacman -S gcc cmake aarch64-linux-gnu-binutils
```

### Build Instructions
```sh
cmake -S . -B build/
cmake --build ./build/
```

## Usage
#### NOTE: A zero instruction (0x00000000) acts as a HALT instruction and will stop the execution of the program. Alternatively, the program will stop execution once it has reached the end of memory. Memory is zero-initialised, so the CPU will stop once it has executed the final instruction.

For running raw binary files:
```sh
./build/main [FILE].bin
```

For running ARM assembly files:
```sh
./run_emulator.sh [FILE].s
```

There is a demo file in the examples directory.

## Useful Links
[ARM's A64 Architecture Guide](https://support.arm.com/documentation/102374/0103)

[ARM's A64 Base Instruction Set List](https://support.arm.com/documentation/ddi0596/2020-12/Base-Instructions)

[Zachary Yedidia's ARM Instruction Set Encoding](https://www.scs.stanford.edu/~zyedidia/arm64/encodingindex.html)

[Introduction to ARM AArch64 Architecture and Low-Level Programming](https://hrishim.github.io/llvl_prog1_book/arm_exec_state.html)
