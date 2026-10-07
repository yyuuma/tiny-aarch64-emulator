#include <fstream>
#include <iostream>
#include <cstdint>

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
        uint64_t read_x(uint8_t n) {
            if (n > 30) {
                throw std::out_of_range("Tried to access register address larger than 30.");
            }
            return reg[n];
        }

        uint32_t read_w(uint8_t n) {
            if (n > 30) {
                throw std::out_of_range("Tried to access register address larger than 30.");
            }
            return (uint32_t) reg[n];
        }
};

class Memory {
    private:
        uint8_t memory[256]{};

    public:
        Memory from_file(std::string filename) {
            std::ifstream file(filename, std::ios::binary);
            Memory res;
            file.read(
                    reinterpret_cast<char*>(&res.memory),
                    256
                    );
            return res;
        }

        // Memory is stored in little-endian.
        uint8_t read_8(uint8_t address) const {
            return memory[address];
        }

        uint16_t read_16(uint8_t address) const {
            if (address == 255) {
                throw std::out_of_range("Tried to access a memory address larger than 255.");
            }
            return (uint16_t) memory[address] | 
                (uint16_t) memory[address+1] << 8;
        }

        uint32_t read_32(uint8_t address) const {
            if (address > 252) {
                throw std::out_of_range("Tried to access a memory address larger than 255.");
            }
            return (uint32_t) memory[address] |
                (uint32_t) memory[address+1] << 8 |
                (uint32_t) memory[address+2] << 16 |
                (uint32_t) memory[address+3] << 24;
        }

        uint64_t read_64(uint8_t address) const {
            if (address > 248) {
                throw std::out_of_range("Tried to access a memory address larger than 255.");
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
    CPU example;
    std::cout << example.read_x(0) << '\n';

    Memory ram;
    std::cout << ram.read_64(54) << '\n';
}
