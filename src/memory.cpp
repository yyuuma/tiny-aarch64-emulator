#include <fstream>
#include <stdexcept>
#include <iostream>
#include <format>

#include "memory.hpp"

Memory::Memory(const std::string& filename) {
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

void Memory::print_hex() const {
    std::cout << "Memory:" << std::endl;
    for (size_t i = 0; i < MEMORY_SIZE; i++) {
        if (i % 16 == 0) {
            std::cout << std::format("{:08x}: ", i);
        }
        std::cout << std::format("{:02x}", memory[i]);
        if (i % 4 == 3) {
            std::cout << " ";
        }
        if (i % 16 == 15) {
            std::cout << "\n";
        }
    }
}

// Memory is stored in little-endian.
u8 Memory::read_8(const u64 address) const {
    if (address > MEMORY_SIZE - 1) {
        throw std::out_of_range("read_8 tried to access a memory address large than the maximum memory size.");
    }
    return memory[address];
}

u16 Memory::read_16(u64 address) const {
    if (address > MEMORY_SIZE - 2) {
        throw std::out_of_range("read_16 tried to access a memory address large than the maximum memory size.");
    }
    return (u16) memory[address] | 
        (u16) memory[address + 1] << 8;
}

u32 Memory::read_32(u64 address) const {
    if (address > MEMORY_SIZE - 4) {
        throw std::out_of_range("read_32 tried to access a memory address large than the maximum memory size.");
    }
    return (u32) memory[address] |
        (u32) memory[address+1] << 8 |
        (u32) memory[address+2] << 16 |
        (u32) memory[address+3] << 24;
}

u64 Memory::read_64(u64 address) const {
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

void Memory::write_8(u64 address, u64 value) {
    if (address > MEMORY_SIZE - 1) {
        throw std::out_of_range("write_8 tried to access a memory address large than the maximum memory size.");
    }
    memory[address] = value & 0xff;
}

void Memory::write_16(u64 address, u64 value) {
    if (address > MEMORY_SIZE - 2) {
        throw std::out_of_range("write_16 tried to access a memory address large than the maximum memory size.");
    }
    memory[address] = value & 0xff;
    memory[address + 1] = (value >> 8) & 0xff;
}

void Memory::write_32(u64 address, u64 value) {
    if (address > MEMORY_SIZE - 4) {
        throw std::out_of_range("write_32 tried to access a memory address large than the maximum memory size.");
    }
    memory[address] = value & 0xff;
    memory[address + 1] = (value >> 8) & 0xff;
    memory[address + 2] = (value >> 16) & 0xff;
    memory[address + 3] = (value >> 24) & 0xff;
}

void Memory::write_64(u64 address, u64 value) {
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
