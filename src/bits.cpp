#include <stdexcept>
#include "bits.hpp"

u64 generate_right_bitmask(u32 n) {
  if (n > 64) {
    throw std::out_of_range("Tried to generate a bitmask that is larger than "
                            "the size of the 32 bit-width.");
  }
  if (n == 64) {
    return UINT64_MAX;
  }
  return (static_cast<u64>(1u) << n) - 1;
}

u64 insert_bits_at_pos(u64 dst, u64 src, u32 n, u32 pos) {
    if (n == 0) {
        throw std::runtime_error("Invalid number of bits selected for function insert_bits_at_pos");
    }

    if (pos + n > 64) {
        throw std::out_of_range("Tried to insert bits past the size of the data (64).");
    }
    u64 bitmask = generate_right_bitmask(n);
    src = (src & bitmask) << pos;
    dst &= ~(bitmask << pos);
    return dst |= src;
}

bool get_bit(u64 data, u32 bit) {
  if (bit > 63) {
    throw std::out_of_range(
        "Tried to retrieve bits out of the range of a 32 bit-width data type.");
  }
  return (data >> bit) & 1u;
}

u32 get_bitrange(u64 data, u32 lo, u32 hi) {
  if (lo > 63 || hi > 64) {
    throw std::out_of_range(
        "Tried to retrieve bits out of the range of a 64 bit-width data type.");
  }
  if (lo >= hi) {
    throw std::runtime_error(
        "Nonsensical lo and hi inputs to get_bitrange(). Make sure that the "
        "beginning value is smaller than the end value.");
  }
  return (data >> lo) & generate_right_bitmask(hi - lo);
}
