#ifndef BITS_HPP
#define BITS_HPP

#include "types.hpp"

// n = [0, 63]
// Returns a 64 bit value with n number of ones from the right.
u64 generate_right_bitmask(u32 n);

// dst - Where you want to insert the bits.
// src - Where you want to insert the bits from.
// n - How many bits you want to insert from the src to dst.
// pos - Where in the dst position you want to insert from.
// Inserts n number of bits from the right of src into a specified
// position in dst.
u64 insert_bits_at_pos(u64 dst, u64 src, u32 n, u32 pos);

// bit = [0, 31]
// Returns true if the instruction's `bit` value is 1, and false otherwise.
bool get_bit(u64 data, u32 bit);

// hi, lo = [0, 31] && hi > lo. lo is inclusive, hi is exclusive.
// Returns the bitmask of the specified high and low values, and right shifts to
// bring the bitmask to the LSB.
// e.g. get_bitrange(0b01101001, 3, 6) = (0b00000101)
u32 get_bitrange(u64 data, u32 lo, u32 hi);

#endif
