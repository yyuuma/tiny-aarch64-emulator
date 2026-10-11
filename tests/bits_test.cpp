#include <gtest/gtest.h>
#include "bits.hpp"

TEST(BitsTest, BitExtraction) {
    EXPECT_EQ(get_bit(0b0001, 0), true);
    EXPECT_EQ(get_bit(0b1000, 3), true);
}
