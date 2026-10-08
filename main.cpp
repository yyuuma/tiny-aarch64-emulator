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

int main(void) {
    CPU cpu;
    Memory mem;
    mem.print_hex();
}
