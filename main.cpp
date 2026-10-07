#include <fstream>
#include <iostream>
#include <cstdint>
#include <format>

class CPU {
    private:
        uint64_t reg[31]{};
        uint64_t sp[4]{};
        uint64_t elr[3]{};
        uint64_t spsr[3]{};
        uint64_t pc{};
        uint64_t nzcv{};
        // add SIMD registers later.

        bool negative_flag() const {
            return nzcv & (1 << 31);
        }

        bool zero_flag() const {
            return nzcv & (1 << 30);
        }

        bool carry_flag() const {
            return nzcv & (1 << 29);
        }

        bool overflow_flag() const {
            return nzcv & (1 << 28);
        }

    public:
        uint64_t read_x(uint8_t n) const {
            if (n > 30) {
                throw std::out_of_range("Tried to access register address larger than 30.");
            }
            return reg[n];
        }

        uint32_t read_w(uint8_t n) const {
            if (n > 30) {
                throw std::out_of_range("Tried to access register address larger than 30.");
            }
            return (uint32_t) reg[n];
        }
};

class Memory {
    private:
        static constexpr std::size_t MEMORY_SIZE = 256;
        uint8_t memory[MEMORY_SIZE]{};

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
        uint8_t read_8(uint8_t address) const {
            return memory[address];
        }

        uint16_t read_16(uint8_t address) const {
            if (address > MEMORY_SIZE - 2) {
                throw std::out_of_range("read_16 to access a memory address larger than 255.");
            }
            return (uint16_t) memory[address] | 
                (uint16_t) memory[address + 1] << 8;
        }

        uint32_t read_32(uint8_t address) const {
            if (address > MEMORY_SIZE - 4) {
                throw std::out_of_range("read_32 to access a memory address larger than 255.");
            }
            return (uint32_t) memory[address] |
                (uint32_t) memory[address+1] << 8 |
                (uint32_t) memory[address+2] << 16 |
                (uint32_t) memory[address+3] << 24;
        }

        uint64_t read_64(uint8_t address) const {
            if (address > MEMORY_SIZE - 8) {
                throw std::out_of_range("read_64 tried to access a memory address larger than 255.");
            }
            return (uint64_t) memory[address] |
                (uint64_t) memory[address+1] << 8 |
                (uint64_t) memory[address+2] << 16 |
                (uint64_t) memory[address+3] << 24 |
                (uint64_t) memory[address+4] << 32 |
                (uint64_t) memory[address+5] << 40 |
                (uint64_t) memory[address+6] << 48 |
                (uint64_t) memory[address+7] << 56;
        }
};

int main(void) {
    CPU cpu;
    Memory mem;
    mem.print_hex();
}
