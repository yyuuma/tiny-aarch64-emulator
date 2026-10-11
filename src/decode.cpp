#include <stdexcept>
#include "decode.hpp"

Instruction_Group decode_instruction_group(u32 inst) {
    u32 op0 = get_bit(inst, 31);
    u32 op1 = get_bitrange(inst, 25, 29);

    // 0b0000 - Reserved/SME
    if (op1 == 0b0000) {
        // MSB == 0: Reserved
        // MSB == 1: SME
        if (op0) {
            throw std::runtime_error("SME instructions have not been implemented.");
        } else {
            throw std::runtime_error("Reserved instruction. This should not be run.");
        }

        // 0b0010 - SVE
    } else if (op1 == 0b0010) {
        throw std::runtime_error("SVE instructions have not been implemented.");

        // 0b00x1 - UNALLOCATED
    } else if ((op1 & 0b1101) == 0b0001) {
        throw std::runtime_error("Unallocated instruction. This should not be run.");

        // 0b100x - Immediate Data Processing
    } else if ((op1 & 0b1110) == 0b1000) {
        return Instruction_Group::IMMEDIATE;

        // 0b101x - Branches, Exception Gathering & System Instructions
    } else if ((op1 & 0b1110) == 0b1010) {
        return Instruction_Group::BRANCH;

        // 0bx101 - Register Data Processing
    } else if ((op1 & 0b0111) == 0b0101) {
        return Instruction_Group::REGISTER;

        // 0bx111 - Scalar Floating-Point & Advanced SIMD
    } else if ((op1 & 0b0111) == 0b0111) {
        throw std::runtime_error("Floating-Point and SIMD instructions have not been implemented.");

        // 0bx1x0 - Loads & Stores
    } else if ((op1 & 0b0101) == 0b0100) {
        return Instruction_Group::LOADSTORE;

        // Error - Bad instruction
    } else {
        throw std::runtime_error("Couldn't identify this instruction.");
    }
}

Instruction decode_immediate_add_sub_instruction(u32 inst) {
    u32 op_S = get_bitrange(inst, 29, 31);
    switch(op_S) {
        case 0b00:
            return Instruction::ADD_IMMEDIATE;
        case 0b01:
            return Instruction::ADDS_IMMEDIATE;
        case 0b10:
            return Instruction::SUB_IMMEDIATE;
        case 0b11:
            return Instruction::SUBS_IMMEDIATE;
        default:
            throw std::runtime_error("Could not identify type of Immediate Add/Sub instruction.");
    }
}

Instruction decode_immediate_instruction(u32 inst) {
    u32 op0 = get_bitrange(inst, 29, 31);
    u32 op1 = get_bitrange(inst, 22, 26);

    // op0 == 0b11 - Data Processing
    // op0 != 0b11 - Extract
    if ((op1 & 0b1110) == 0b1110) {
        if (op0 == 0b11) {
            throw std::runtime_error("1 Source Immediate instructions have not been implemented.");
        } else {
            throw std::runtime_error("Extract instructions have not been implemented");
        }

    // 0b00xx - PC-Relative
    } else if ((op1 & 0b1100) == 0b0000) {
        throw std::runtime_error("PC-Relative instructions have not been implemented");

    // 0b010x - Add/Subtract Immediate
    } else if ((op1 & 0b1110) == 0b0100) {
        return decode_immediate_add_sub_instruction(inst);

        // 0b0110 - Add/Subtract Immediate w/ Tags
    } else if (op1 == 0b0110) {
        throw std::runtime_error("Add/Subtract Immediate w/ Tags instructions have not been implemented.");

        // 0b0111 - Min/Max Immediate
    } else if (op1 == 0b0111) {
        throw std::runtime_error("Min/Max Immediate instructions have not been implemented");

        // 0b100x - Logical Immediate
    } else if ((op1 & 0b1110) == 0b1000) {
        throw std::runtime_error("Logical Immediate instructions have not been implemented");

        // 0b101x - Move Wide Immediate
    } else if ((op1 & 0b1110) == 0b1010) {
        throw std::runtime_error("Move Wide Immediate instructions have not been implemented");

        // 0b110x - Bitfield
    } else if ((op1 & 0b1110) == 0b1100) {
        throw std::runtime_error("Bitfield instructions have not been implemented");

        // Error: Invalid Instruction 
    } else {
        throw std::runtime_error("Invalid instruction opcode inputted into decode_immediate_instruction.");
    }
}

Instruction decode_shifted_register_add_sub_instruction(u32 inst) {
    u32 op_S = get_bitrange(inst, 29, 31);
    switch(op_S) {
        case 0b00:
            return Instruction::ADD_SHIFTREG;
        case 0b01:
            return Instruction::ADDS_SHIFTREG;
        case 0b10:
            return Instruction::SUB_SHIFTREG;
        case 0b11:
            return Instruction::SUBS_SHIFTREG;
        default:
            throw std::runtime_error("Could not identify type of Shifted Register Add/Sub instruction.");
    }
}

Instruction decode_extended_register_add_sub_instruction(u32 inst) {
    u32 opt  = get_bitrange(inst, 22, 24);
    u32 op_S = get_bitrange(inst, 29, 31);
    if (opt != 0b00) {
        throw std::runtime_error("Found an unallocated instruction while decoding an Extended Register Add/Sub instruction");
    }
    switch(op_S) {
        case 0b00:
            return Instruction::ADD_EXTENDREG;
        case 0b01:
            return Instruction::ADDS_EXTENDREG;
        case 0b10:
            return Instruction::SUB_EXTENDREG;
        case 0b11:
            return Instruction::SUBS_EXTENDREG;
        default:
            throw std::runtime_error("Could not identify type of Shifted Register Add/Sub instruction.");
    }
}

Instruction decode_register_instruction(u32 inst) {
    //const u32 op0 = get_bit(inst, 30);
    const u32 op1 = get_bit(inst, 28);
    const u32 op2 = get_bitrange(inst, 21, 25);
    //const u32 op3 = get_bitrange(inst, 10, 16);

    // op1 = 0 AND op2 = 0b1xx0 - Add/Subtract Shifted Register
    if (op1 == 0 && (op2 & 0b1001) == 0b1000) {
        return decode_shifted_register_add_sub_instruction(inst);

        // op1 = 0 AND op2 = 0b1xx1 - ADD (Extended Register)
    } else if (op1 == 0 && (op2 & 0b1001) == 0b1001) { 
        return decode_extended_register_add_sub_instruction(inst);

        // Haven't implemented other register processing instructions.
    } else {
        throw std::runtime_error("Attempted to run an unimplemented register processing instruction.");
    }
}

Instruction decode(u32 inst) {
    switch(decode_instruction_group(inst)) {
        case Instruction_Group::RESERVED:
            throw std::runtime_error("Trying to execute a reserved instruction.");
            break;
        case Instruction_Group::UNALLOCATED:
            throw std::runtime_error("Trying to execute an unallocated instruction.");
            break;
        case Instruction_Group::IMMEDIATE:
            return decode_immediate_instruction(inst);
        case Instruction_Group::BRANCH:
            // TODO: implement
            break;
        case Instruction_Group::REGISTER:
            return decode_register_instruction(inst);
            break;
        case Instruction_Group::SIMD:
            //TODO: implement
            break;
        case Instruction_Group::LOADSTORE:
            //TODO: implement
            break;
    }
    throw std::runtime_error("Couldn't decode instruction. Instruction may be corrupt.");
}
