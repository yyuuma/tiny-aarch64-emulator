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
  return (1u << n) - 1;
}

bool get_bit(u32 inst, u32 bit) {
  if (bit > 31) {
    throw std::out_of_range(
        "Tried to retrieve bits out of the range of a 32 bit-width data type.");
  }
  return (inst >> bit) & 1u;
}

u32 get_bitrange(u32 inst, u32 lo, u32 hi) {
  if (lo > 31 || hi > 31) {
    throw std::out_of_range(
        "Tried to retrieve bits out of the range of a 32 bit-width data type.");
  }
  if (lo >= hi) {
    throw std::runtime_error(
        "Nonsensical loinning and hi inputs to get_bits(). Make sure that the "
        "beginning value is smaller than the end value.");
  }
  return (inst >> lo) & generate_right_bitmask(hi - lo);
}
