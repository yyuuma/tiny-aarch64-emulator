#include <iostream>
#include <stdexcept>
#include <format>

#include "cpu.hpp"

void CPU::increment_pc() {
    pc += 4;
}

bool CPU::get_negative_flag() const {
    return nzcv & N;
}

bool CPU::get_zero_flag() const {
    return nzcv & Z;
}

bool CPU::get_carry_flag() const {
    return nzcv & C;
}

bool CPU::get_overflow_flag() const {
    return nzcv & V;
} 

void CPU::set_negative_flag(bool b) {
    b ? nzcv |= N : nzcv &= ~N;
}

void CPU::set_zero_flag(bool b) {
    b ? nzcv |= Z : nzcv &= ~Z;
}

void CPU::set_carry_flag(bool b) {
    b ? nzcv |= C : nzcv &= ~C;
}

void CPU::set_overflow_flag(bool b) {
    b ? nzcv |= V : nzcv &= ~V;
}

u64 CPU::shift_reg(bool sf, u8 reg, Shift_Type shift_type, u32 shift_amount) {
    u64 result = sf ? read_x(reg) : read_w(reg);
    switch(shift_type) {
        case Shift_Type::LSL:
            return result << shift_amount;
            break;
        case Shift_Type::LSR:
            return result >> shift_amount;
            break;
        case Shift_Type::ASR:
            return sf
                ? static_cast<i64>(result) >> shift_amount
                : static_cast<i32>(result) >> shift_amount;
            break;
        default:
            throw std::runtime_error("Invalid shift register operation attempted, make sure you have a valid shift type.");
    }
}

u64 CPU::read_x(const u8 n) const {
    if (n > 30) {
        throw std::out_of_range("Tried to access register address larger than 30.");
    }
    return gpr[n];
}

void CPU::write_x(const u8 x, const u64 val) {
    if (x > 30) {
        throw std::out_of_range("Tried to access register address larger than 30.");
    }
    gpr[x] = val;
}

u32 CPU::read_w(const u8 n) const {
    if (n > 30) {
        throw std::out_of_range("Tried to access register address larger than 30.");
    }
    return static_cast<u32>(gpr[n]);
}

void CPU::write_w(const u8 w, const u32 val) {
    if (w > 30) {
        throw std::out_of_range("Tried to access register address larger than 30.");
    }
    gpr[w] = val;
}

u64 CPU::read_sp() const {
    return sp;
}

void CPU::write_sp(u64 val) {
    sp = val;
}

u64 CPU::read_pc() const {
    return pc;
}

void CPU::write_pc(u64 val) {
    pc = val;
}

void CPU::print_all_w() const {
    std::cout << "W Registers:" << std::endl;
    for (int i = 0; i < 31; i++) {
        std::cout << "W" << std::format("{:02}: ", i) << std::format("{:10}", static_cast<u32>(gpr[i]));
        if (i % 4 == 3) {
            std::cout << std::endl;
        } else {
            std::cout << " ";
        }
    }
    std::cout << std::endl;
}

void CPU::print_all_x() const {
    std::cout << "X Registers:" << std::endl;
    for (int i = 0; i < 31; i++) {
        std::cout << "X" << std::format("{:02}: ", i) << std::format("{:016x}", gpr[i]);
        if (i % 4 == 3) {
            std::cout << std::endl;
        } else {
            std::cout << " ";
        }
    }
    std::cout << std::endl;
}

void CPU::print_gpr() const {
    print_all_w();
    std::cout << std::endl;
    print_all_x();
}

void CPU::print_sp() const {
    std::cout << "SP: " << read_sp() << std::endl;
}

void CPU::print_pc() const {
    std::cout << "PC: " << read_pc() << std::endl;
}

void CPU::print_nzcv() const {
    std::cout << "NZCV Flags:" << std::endl;
    std::cout << "N: " << get_negative_flag() << std::endl;
    std::cout << "Z: " << get_zero_flag() << std::endl;
    std::cout << "C: " << get_carry_flag() << std::endl;
    std::cout << "V: " << get_overflow_flag() << std::endl;
}

void CPU::print_state() const {
    print_all_x();
    std::cout << '\n';
    print_sp();
    std::cout << '\n';
    print_pc();
    std::cout << '\n';
    print_nzcv();
}

u32 CPU::fetch(Memory& mem) {
    u32 inst = mem.read_32(read_pc());
    increment_pc();
    return inst;
}

void CPU::print_reg_b(bool sf, u8 reg) {
    if (reg > 30) {
        throw std::runtime_error("Trying to access general purpose register out of maximum limits (30)");
    }
    std::cout << (sf
            ? std::format("{:064b}\n", gpr[reg]) 
            : std::format("{:032b}\n", gpr[reg]));
}

void CPU::execute(Memory& mem) {
    decode(fetch(mem));
}
