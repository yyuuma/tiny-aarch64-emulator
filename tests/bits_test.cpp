#include <gtest/gtest.h>
#include "bits.hpp"

TEST(BitsTest, GetBitsProducesCorrectResult) {
    EXPECT_EQ(get_bit(0x1, 0), true); 
    EXPECT_EQ(get_bit(0x0000'0040'0000'0000, 38), true);
    EXPECT_EQ(get_bit(0x8000'0000'0000'0000, 63), true);
    EXPECT_EQ(get_bitrange(0x0123'4567'89ab'cdef, 0, 16), 0xcdef);
    EXPECT_EQ(get_bitrange(0x0123'4567'89ab'cdef, 16, 32), 0x89ab);
    EXPECT_EQ(get_bitrange(0x0123'4567'89ab'cdef, 32, 48), 0x4567);
    EXPECT_EQ(get_bitrange(0x0123'4567'89ab'cdef, 48, 64), 0x0123);
    EXPECT_EQ(get_bitrange(0x0123'4567'89ab'cdef, 63, 64), 0x00);
}

TEST(BitsTest, GetBitsInvalidInputProducesError) {
    EXPECT_EQ(get_bit(0x0, 0), false);
    EXPECT_EQ(get_bit(0x8000'0000'0000'0000, 0), false);
    EXPECT_THROW(get_bit(0x0, 64), std::out_of_range);
}
