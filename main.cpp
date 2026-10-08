// Implemented instructions: ADD

#include <variant>
#include <fstream>
#include <iostream>
#include <cstdint>
#include <format>

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

class CPU {
    private:
        u64 gpr[31]{};
        u64 sp{};
        u64 pc{};
        u64 nzcv{};
        // add SIMD registers later.

        static constexpr u64 N = (1u << 31);
        static constexpr u64 Z = (1u << 30);
        static constexpr u64 C = (1u << 29);
        static constexpr u64 V = (1u << 28);

        bool negative_flag() const {
            return nzcv & N;
        }

        bool zero_flag() const {
            return nzcv & Z;
        }

        bool carry_flag() const {
            return nzcv & C;
        }

        bool overflow_flag() const {
            return nzcv & V;
        } 

    public:
        u64 read_x(u8 n) const {
            if (n > 30) {
                throw std::out_of_range("Tried to access register address larger than 30.");
            }
            return gpr[n];
        }

        u32 read_w(u8 n) const {
            if (n > 30) {
                throw std::out_of_range("Tried to access register address larger than 30.");
            }
            return (u32) gpr[n];
        }

        u64 read_sp() const {
            return sp;
        }

        u64 read_pc() const {
            return pc;
        }

        void increment_pc() {
            pc += 4;
        }

        void set_negative(bool b) {
            if (b) {
                nzcv |= N;
            } else {
                nzcv &= ~N;
            }
        }

        void set_zero(bool b) {
            if (b) {
                nzcv |= Z;
            } else {
                nzcv &= ~Z;
            }
        }

        void set_carry(bool b) {
            if (b) {
                nzcv |= C;
            } else {
                nzcv &= ~C;
            }
        }

        void set_overflow(bool b) {
            if (b) {
                nzcv |= V;
            } else {
                nzcv &= ~V;
            }
        }

};

class Memory {
    private:
        static constexpr std::size_t MEMORY_SIZE = 256;
        u8 memory[MEMORY_SIZE]{};

    public:
        Memory(){}

        Memory(const std::string& filename) {
            std::ifstream file(filename, std::ios::binary);
            if (!file) {
                throw std::runtime_error("Could not initialise memory from file. Check if the file exists.");
            }

            // Calculate the size of file
            file.seekg(0, std::ios::end);
            const size_t size = file.tellg();
            file.seekg(0, std::ios::beg);

            if (size > MEMORY_SIZE) {
                throw std::runtime_error("Inputted file is too large. The maximum file size is 256 bytes.");
            }

            file.read(reinterpret_cast<char*>(memory), size);
        }

        void print_hex() const {
            for (size_t i = 0; i < MEMORY_SIZE; i++) {
                if (i % 16 == 0) {
                    std::cout << std::format("{:08x}: ", i);
                }
                std::cout << std::format("{:02x}", memory[i]);
                if (i % 2 == 1) {
                    std::cout << " ";
                }
                if (i % 16 == 15) {
                    std::cout << "\n";
                }
            }
        }

        // Memory is stored in little-endian.
        u8 read_8(u64 address) const {
            if (address > MEMORY_SIZE - 1) {
                throw std::out_of_range("read_8 tried to access a memory address large than the maximum memory size.");
            }
            return memory[address];
        }

        u16 read_16(u64 address) const {
            if (address > MEMORY_SIZE - 2) {
                throw std::out_of_range("read_16 tried to access a memory address large than the maximum memory size.");
            }
            return (u16) memory[address] | 
                (u16) memory[address + 1] << 8;
        }

        u32 read_32(u64 address) const {
            if (address > MEMORY_SIZE - 4) {
                throw std::out_of_range("read_32 tried to access a memory address large than the maximum memory size.");
            }
            return (u32) memory[address] |
                (u32) memory[address+1] << 8 |
                (u32) memory[address+2] << 16 |
                (u32) memory[address+3] << 24;
        }

        u64 read_64(u64 address) const {
            if (address > MEMORY_SIZE - 8) {
                throw std::out_of_range("read_64 tried to access a memory address large than the maximum memory size.");
            }
            return (u64) memory[address] |
                (u64) memory[address+1] << 8 |
                (u64) memory[address+2] << 16 |
                (u64) memory[address+3] << 24 |
                (u64) memory[address+4] << 32 |
                (u64) memory[address+5] << 40 |
                (u64) memory[address+6] << 48 |
                (u64) memory[address+7] << 56;
        }

        void write_8(u64 address, u64 value) {
            if (address > MEMORY_SIZE - 1) {
                throw std::out_of_range("write_8 tried to access a memory address large than the maximum memory size.");
            }
            memory[address] = value & 0xff;
        }

        void write_16(u64 address, u64 value) {
            if (address > MEMORY_SIZE - 2) {
                throw std::out_of_range("write_16 tried to access a memory address large than the maximum memory size.");
            }
            memory[address] = value & 0xff;
            memory[address + 1] = (value >> 8) & 0xff;
        }

        void write_32(u64 address, u64 value) {
            if (address > MEMORY_SIZE - 4) {
                throw std::out_of_range("write_32 tried to access a memory address large than the maximum memory size.");
            }
            memory[address] = value & 0xff;
            memory[address + 1] = (value >> 8) & 0xff;
            memory[address + 2] = (value >> 16) & 0xff;
            memory[address + 3] = (value >> 24) & 0xff;
        }

        void write_64(u64 address, u64 value) {
            if (address > MEMORY_SIZE - 8) {
                throw std::out_of_range("write_64 tried to access a memory address large than the maximum memory size.");
            }
            memory[address] = value & 0xff;
            memory[address + 1] = (value >> 8) & 0xff;
            memory[address + 2] = (value >> 16) & 0xff;
            memory[address + 3] = (value >> 24) & 0xff;
            memory[address + 4] = (value >> 32) & 0xff;
            memory[address + 5] = (value >> 40) & 0xff;
            memory[address + 6] = (value >> 48) & 0xff;
            memory[address + 7] = (value >> 56) & 0xff;
        }
};

// Rd     - Destination Register
// Rn/Rm  - Input Registers
// Rt     - Transfer Registers
// sf = 0 - 32-bit Variant
// sf = 1 - 64-bit Variant
// sh     - Left Shift applying to Immediate
// S = 1  - Instruction Updates the Condition Flags

struct AddImmediate {
    u8 Rd;
    u8 Rn;
    bool sf;
    bool sh;
    bool S;
};

using Instruction = std::variant<AddImmediate>;

u32 fetch(CPU& cpu, Memory& mem) {
    u32 inst = mem.read_32(cpu.read_pc());
    cpu.increment_pc();
    return inst;
}

enum class Instruction_Group : u8 {
    RESERVED = 0b0000, // not implemented
    UNALLOCATED = 0b0001, // not implemented
    IMMEDIATE = 0b1000, // TODO: implement
    BRANCH = 0b1010, // TODO: implement
    REGISTER = 0b0101, // TODO: implement
    SIMD = 0b0111, // not implemented
    LOADSTORE = 0b0100, /// TODO: implement
};

// n is the number of 1's from the right
u32 generate_right_bitmask(u32 n) {
    if (n > 32) {
        throw std::out_of_range("Tried to generate a bitmask that is larger than the size of the 32 bit-width.");
    } 
    if (n == 32) {
        return 0xffffffff;
    }
    return (1u << n) - 1;
}

// possible bit values are [0-31]
u32 get_bit(u32 inst, u32 bit) {
    if (bit > 31) {
        throw std::out_of_range("Tried to retrieve bits out of the range of a 32 bit-width data type.");
    }
    return (inst >> bit) & 1u;
}

// possible bit ranges are [0-31]
u32 get_bitrange(u32 inst, u32 beg, u32 end) {
    if (beg > 31 || end > 31) {
        throw std::out_of_range("Tried to retrieve bits out of the range of a 32 bit-width data type.");
    }
    if (beg >= end) {
        throw std::runtime_error("Nonsensical beginning and end inputs to get_bits(). Make sure that the beginning value is smaller than the end value.");
    }
    return (inst >> beg) & generate_right_bitmask(end - beg);
}

// Checks bits [25-28] for which instruction group it belongs to.
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

Instruction decode_immediate(u32 inst) {
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
        //TODO: Implement!!

    // 0b0110 - Add/Subtract Immediate w/ Tags
    } else if (op1 == 0b0110) {
        //TODO: Implement!!

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
        throw std::runtime_error("Invalid instruction opcode in decode_immediate.");
    }
}

Instruction decode(u32 inst) {
    switch(decode_instruction_group(inst)) {
        case Instruction_Group::RESERVED:
            // not implemented
            break;
        case Instruction_Group::UNALLOCATED:
            // should be an error
            break;
        case Instruction_Group::IMMEDIATE:
            return decode_immediate(inst);
        case Instruction_Group::BRANCH:
            // TODO: implement
            break;
        case Instruction_Group::REGISTER:
            //TODO: implement
            break;
        case Instruction_Group::SIMD:
            //TODO: implement
        case Instruction_Group::LOADSTORE:
            //TODO: implement
    }
}

int main(void) {

}
