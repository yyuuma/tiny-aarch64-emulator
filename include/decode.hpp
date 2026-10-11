#ifndef DECODE_HPP
#define DECODE_HPP

#include "bits.hpp"
#include "types.hpp"

enum class Instruction_Group : u8 {
    RESERVED = 0b0000, 
    UNALLOCATED = 0b0001, 
    IMMEDIATE = 0b1000, 
    BRANCH = 0b1010, 
    REGISTER = 0b0101, 
    SIMD = 0b0111, 
    LOADSTORE = 0b0100, 
};


enum class Instruction : u32 {
    ADD_IMMEDIATE,
    ADD_EXTENDREG,
    ADD_SHIFTREG,
    ADDS_IMMEDIATE,
    ADDS_EXTENDREG,
    ADDS_SHIFTREG,
    SUB_IMMEDIATE,
    SUB_EXTENDREG,
    SUB_SHIFTREG,
    SUBS_IMMEDIATE,
    SUBS_SHIFTREG,
    SUBS_EXTENDREG,
};

// [N] indicates Nth bit of instruction.
// [N-M] indicates inclusive bitrange [N-M] of instruction.

// Instruction Group
// Checks op0 [31] & op1 [25-28]
// https://www.scs.stanford.edu/~zyedidia/arm64/encodingindex.html
Instruction_Group decode_instruction_group(u32 inst);

/* ------------------------------------------------------ */

// Decodes which kind of add/sub immediate instruction it is.
// https://www.scs.stanford.edu/~zyedidia/arm64/encodingindex.html#addsub_imm
Instruction decode_immediate_add_sub_instruction(u32 inst);

// Data-Processing - Immediate instructions
// Checks op0 [29-30] & op1 [22-25]
// https://www.scs.stanford.edu/~zyedidia/arm64/encodingindex.html#dpimm
Instruction decode_immediate_instruction(u32 inst);

/* ------------------------------------------------------ */

Instruction decode_shifted_register_add_sub_instruction(u32 inst); 

/* ------------------------------------------------------ */

Instruction decode_extended_register_add_sub_instruction(u32 inst); 

/* ------------------------------------------------------ */

Instruction decode_register_instruction(u32 inst);

/* ------------------------------------------------------ */
 
// Main decode function.
Instruction decode(u32 inst);

#endif
