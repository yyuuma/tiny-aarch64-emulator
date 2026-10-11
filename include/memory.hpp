#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <string>

#include "types.hpp"
#include "bits.hpp"

class Memory {
    private:
        static constexpr std::size_t MEMORY_SIZE = 256;
        u8 memory[MEMORY_SIZE]{};

    public:
        Memory(){}

        Memory(const std::string& filename); 

        void print_hex() const; 

        // Memory is stored in little-endian.
        u8 read_8(const u64 address) const; 

        u16 read_16(u64 address) const; 

        u32 read_32(u64 address) const; 

        u64 read_64(u64 address) const; 

        void write_8(u64 address, u64 value); 

        void write_16(u64 address, u64 value); 

        void write_32(u64 address, u64 value); 

        void write_64(u64 address, u64 value); 
};
#endif
