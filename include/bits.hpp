#ifndef BITS_HPP
#define BITS_HPP

#include "types.hpp"

enum class Shift_Type : u8 {
    LSL = 0b00,
    LSR = 0b01,
    ASR = 0b10,
};

// n = [0, 64]
// Returns a 64 bit value with `n` number of ones from the right.
u64 generate_right_bitmask(u32 n);

// bit = [0, 31]
// Returns true if the instruction's `bit` value is 1, and false otherwise.
bool get_bit(u32 inst, u32 bit);

// hi, lo = [0, 31] && hi > lo. lo is inclusive, hi is exclusive.
// Returns the bitmask of the specified high and low values, and right shifts to
// bring the bitmask to the LSB.
// e.g. get_bitrange(0b01101001, 3, 6) = (0b00000101)
u32 get_bitrange(u32 inst, u32 lo, u32 hi);

#endif
