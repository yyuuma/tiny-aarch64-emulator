#include <gtest/gtest.h>
#include "cpu.hpp"

TEST(AddSubTest, 32BitAddImmediate) {
    CPU cpu; 

    // 0 + 26
    // ADD r1, r0, #26
    cpu.decode(0b0001'0001'0000'0000'0110'1000'0000'0001); 
    EXPECT_EQ(cpu.read_w(1), 26);

    // 26 + 16773120
    // ADD r30, r1, #16773120
    cpu.decode(0b0001'0001'0111'1111'1111'1100'0011'1110);
    EXPECT_EQ(cpu.read_w(30), 16773146);

    // Check for 32-bit overflow
    // MOV r4 #18446744073709551615
    // ADD r5, r4, #1
    cpu.decode(0b1101'0010'1111'1111'1111'1111'1110'0100);
    cpu.decode(0b0001'0001'0000'0000'0000'0100'1000'1001);
    EXPECT_EQ(cpu.read_x(5), 0);
}
