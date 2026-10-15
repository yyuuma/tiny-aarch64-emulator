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

TEST(AddSubTest, 32BitAddImmediateSettingFlags) {
    CPU cpu;

    // Check for Zero Flag
    // ADDS, r0, r1, #0
    cpu.decode(0b0011'0001'0000'0000'0000'0000'0010'0000);
    EXPECT_EQ(cpu.get_zero_flag(), true);

    // Check for No Zero Flag
    // ADDS, r0, r1, #0
    cpu.decode(0b0011'0001'0000'0000'0000'0100'0010'0000);
    EXPECT_EQ(cpu.get_zero_flag(), false);
    
    // Check for Carry & Overflow Flag after 32-bit overflow
    // MOV r3, #18446744073709551615 (0xffff'00000)
    cpu.decode(0b0101'0010'1011'1111'1111'1111'1110'0011);

    // MOV r4, #65535 (0xffff)
    cpu.decode(0b0101'0010'1001'1111'1111'1111'1110'0100);

    // ADD r3, r3, r4
    cpu.decode(0b0000'1011'0000'0100'0000'0000'0110'0011);

    // ADDS r5, r3, #1
    cpu.decode(0b0011'0001'0000'0000'0000'0100'0110'0101);
    EXPECT_EQ(cpu.get_carry_flag(), true);
    EXPECT_EQ(cpu.get_overflow_flag(), false);
}
