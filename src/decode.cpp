#include <stdexcept>

#include "types.hpp"
#include "cpu.hpp"
#include "bits.hpp"
#include "decode.hpp"

bool get_instruction_bit(u32 inst, u32 bit) {
    if (bit > 31) {
        throw std::out_of_range("Tried to retrieve bit outside of the 32 bit-width instruction size.");
    }
    return get_bit(inst, bit);
}

u32 get_instruction_bitrange(u32 inst, u32 lo, u32 hi) {
    if (lo > 31 || hi > 32) {
        throw std::out_of_range("Tried to retrieve bit outside of the 32 bit-width instruction size.");
    }
    return get_bitrange(inst, lo, hi);
}

// https://www.scs.stanford.edu/~zyedidia/arm64/encodingindex.html#dpimm
void CPU::decode_immediate_instruction(u32 inst) {
    u32 op0 = get_instruction_bitrange(inst, 29, 31);
    u32 op1 = get_instruction_bitrange(inst, 22, 26);

    // op1 = 0b111x - Data Processing (1 Source Immediate)
    if ((op0 == 0b11) && (op1 & 0b1110) == 0b1110) {
        throw std::runtime_error("1 Source Immediate instructions have not been implemented.");

        // op1 = 0b00xx - PC-Relative
    } else if ((op1 & 0b1100) == 0b0000) {
        throw std::runtime_error("PC-Relative instructions have not been implemented");

        // op1 = 0b010x - Add/Subtract Immediate
    } else if ((op1 & 0b1110) == 0b0100) {
        u32 Rd = get_instruction_bitrange(inst, 0, 5);
        u32 Rn = get_instruction_bitrange(inst, 5, 10); 
        u32 imm12 = get_instruction_bitrange(inst, 10, 22);
        bool sh = get_instruction_bit(inst, 22);
        bool S = get_instruction_bit(inst, 29);
        bool op = get_instruction_bit(inst, 30);
        bool sf = get_instruction_bit(inst, 31);
        execute_immediate_add_sub_instruction(Rd, Rn, imm12, sh, S, op, sf);

        // op1 = 0b0110 - Add/Subtract Immediate w/ Tags
    } else if (op1 == 0b0110) {
        throw std::runtime_error("Add/Subtract Immediate w/ Tags instructions have not been implemented.");

        // op1 = 0b0111 - Min/Max Immediate
    } else if (op1 == 0b0111) {
        throw std::runtime_error("Min/Max Immediate instructions have not been implemented");

        // op1 = 0b100x - Logical Immediate
    } else if ((op1 & 0b1110) == 0b1000) {
        throw std::runtime_error("Logical Immediate instructions have not been implemented");

        // op1 = 0b101x - Move Wide Immediate
    } else if ((op1 & 0b1110) == 0b1010) {
        u32 Rd = get_instruction_bitrange(inst, 0, 5);
        u32 imm16 = get_instruction_bitrange(inst, 5, 21);
        u32 hw = get_instruction_bitrange(inst, 21, 23);
        u32 opc = get_instruction_bitrange(inst, 29, 31);
        bool sf = get_instruction_bit(inst, 31);
        execute_immediate_move_wide_instruction(Rd, imm16, hw, opc, sf);

        // op1 = 0b110x - Bitfield
    } else if ((op1 & 0b1110) == 0b1100) {
        throw std::runtime_error("Bitfield instructions have not been implemented");

        // op0 != 0b11 AND op1 = 111x - Extract
    } else if ((op0 != 0b11) && ((op1 & 0b1110) == 0b1110)) {
        throw std::runtime_error("Extract instructions have not been implemented");

        // Error: Invalid Instruction 
    } else {
        throw std::runtime_error("Invalid instruction opcode inputted into decode_immediate_instruction.");
    }
}

// https://www.scs.stanford.edu/~zyedidia/arm64/encodingindex.html#dpreg
void CPU::decode_register_instruction(u32 inst) {
    u32 op0 = get_instruction_bit(inst, 30);
    u32 op1 = get_instruction_bit(inst, 28);
    u32 op2 = get_instruction_bitrange(inst, 21, 25);
    u32 op3 = get_instruction_bitrange(inst, 10, 16);

    // op0 = 0 AND op1 = 1 AND op2 = 0b0110 - Data-processing (2 source)
    if ((op0 == 0) && (op1 == 1) && (op2 == 0b0110)) {
        throw std::runtime_error("Data processing (2 Source) instructions have not been implemented.");

        // op0 = 1 AND op1 = 1 AND op2 = 0b0110 - Data-processing (1 source)
    } else if ((op0 == 1) && (op1 == 1) && (op2 == 0b0110)) {
        throw std::runtime_error("Data processing (1 Source) instructions have not been implemented.");

        // op1 = 0 AND op2 = 0b0xxx - Logical (Shifted Register)
    } else if ((op1 == 0) && ((op2 & 0b1000) == 0b0000)) {
        throw std::runtime_error("Logical (Shifted Register) instructions have not been implemented.");

        // op1 = 0 AND op2 = 0b1xx0 - Add/Subtract Shifted Register
    } else if (op1 == 0 && (op2 & 0b1001) == 0b1000) {
        u32 Rd = get_instruction_bitrange(inst, 0, 5);
        u32 Rn = get_instruction_bitrange(inst, 5, 10);
        u32 imm6 = get_instruction_bitrange(inst, 10, 16);
        u32 Rm = get_instruction_bitrange(inst, 16, 21);
        u32 shift = get_instruction_bitrange(inst, 22, 24);
        bool S = get_instruction_bit(inst, 29);
        bool op = get_instruction_bit(inst, 30);
        bool sf = get_instruction_bit(inst, 31);
        execute_shifted_register_add_sub_instruction(Rd, Rn, imm6, Rm, shift, S, op, sf);

        // op1 = 0 AND op2 = 0b1xx1 - Add/Subtract (Extended Register)
    } else if (op1 == 0 && (op2 & 0b1001) == 0b1001) { 
        u32 Rd = get_instruction_bitrange(inst, 0, 5);
        u32 Rn = get_instruction_bitrange(inst, 5, 10);
        u32 imm3 = get_instruction_bitrange(inst, 10, 13);
        u32 option = get_instruction_bitrange(inst, 13, 16);
        u32 Rm = get_instruction_bitrange(inst, 16, 21);
        u32 opt = get_instruction_bitrange(inst, 22, 24);
        bool S = get_instruction_bit(inst, 29);
        bool op = get_instruction_bit(inst, 30);
        bool sf = get_instruction_bit(inst, 31);
        if (opt != 0b0000) {
            unallocated();
        }
        execute_extended_register_add_sub_instruction(Rd, Rn, imm3, option, Rm, opt, S, op, sf); 

        // Haven't implemented other register processing instructions.
    } else {
        throw std::runtime_error("Attempted to run an unimplemented register processing instruction.");
    }
}

void unimplemented() {
    throw std::runtime_error("Tried to execute an unimplemented instruction.");
}

void unallocated() {
    throw std::runtime_error("Tried to execute an unallocated instruction");
}

void undefined() {
    throw std::runtime_error("Tried to execute an undefined instruction");
}

// https://www.scs.stanford.edu/~zyedidia/arm64/encodingindex.html
void CPU::decode(u32 inst) {
    u32 op0 = get_instruction_bit(inst, 31);
    u32 op1 = get_instruction_bitrange(inst, 25, 29);

    // 0x0000'0000'0000'0000 is treated as a HALT instruction.
    if (inst == 0x0) {
        running = false;
        return;
    }

    // Reserved
    if (op0 == 0 && op1 == 0b0000) {
        unimplemented();

        // SME Encodings
    } else if (op0 == 1 && op1 == 0b0000) {
        unimplemented();

        // SVE Encodings
    } else if (op1 == 0b0010) {
        unimplemented();

        // Unallocated
    } else if ((op1 & 0b1101) == 0b0001) {
        unallocated();

        // Data Processing - Immediate
    } else if ((op1 & 0b1110) == 0b1000) {
        decode_immediate_instruction(inst);

        // Branches 
    } else if ((op1 & 0b1110) == 0b1010) {
        unimplemented();

        // Data Processing - Register
    } else if ((op1 & 0b0111) == 0b0101) {
        decode_register_instruction(inst);

        // Data Processing - Scalar Floating-Point & SIMD
    } else if ((op1 & 0b0111) == 0b0111) {
        unimplemented();

        // Loads and Stores
    } else if ((op1 & 0b0101) == 0b0100) {
        unimplemented();

    } else {
        throw std::runtime_error("Invalid instruction decoded.");
    }
}
