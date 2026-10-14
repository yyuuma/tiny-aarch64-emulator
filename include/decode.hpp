#ifndef DECODE_HPP
#define DECODE_HPP

#include "types.hpp"

enum class Shift_Type : u32 {
    LSL = 0b00,
    LSR = 0b01,
    ASR = 0b10,
};

enum class Extend_Type : u32 {
    UXTB = 0b00,
    UXTH = 0b01,
    UXTW = 0b010,
    UXTX = 0b011,
    SXTB = 0b100,
    SXTH = 0b101,
    SXTW = 0b110,
    SXTX = 0b111,
};

bool get_instruction_bit(u32 inst, u32 bit);

u32 get_instruction_bitrange(u32 inst, u32 lo, u32 hi);

void unimplemented();

void unallocated(); 

void undefined(); 

// [N] indicates Nth bit of instruction.
// [N-M] indicates inclusive bitrange [N-M] of instruction.

// Instruction Group
// Checks op0 [31] & op1 [25-28]
// https://www.scs.stanford.edu/~zyedidia/arm64/encodingindex.html

/* ------------------------------------------------------ */

// Decodes which kind of add/sub immediate instruction it is.
// https://www.scs.stanford.edu/~zyedidia/arm64/encodingindex.html#addsub_imm
// void decode_immediate_add_sub_instruction(u32 inst);
//
// // Data-Processing - Immediate instructions
// // Checks op0 [29-30] & op1 [22-25]
// // https://www.scs.stanford.edu/~zyedidia/arm64/encodingindex.html#dpimm
// void decode_immediate_instruction(u32 inst);
//
// /* ------------------------------------------------------ */
//
// void decode_shifted_register_add_sub_instruction(u32 inst); 
//
// /* ------------------------------------------------------ */
//
// void decode_extended_register_add_sub_instruction(u32 inst); 
//
// /* ------------------------------------------------------ */
//
// void decode_register_instruction(u32 inst);
#endif
