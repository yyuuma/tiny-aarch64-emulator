#include <iostream>
#include <stdexcept>
#include <format>

#include "cpu.hpp"
#include "bits.hpp"
#include "decode.hpp"

void CPU::shift_reg(bool sf, u8 reg, Shift_Type shift_type, u32 shift_amount) {
    switch(shift_type) {
        case Shift_Type::LSL:
            gpr[reg] = gpr[reg] << shift_amount;
            break;
        case Shift_Type::LSR:
            gpr[reg] = gpr[reg] >> shift_amount;
            break;
        case Shift_Type::ASR:
            if (sf) {
                gpr[reg] = static_cast<u64>(static_cast<i64>(gpr[reg]) >> shift_amount);
            } else {
                gpr[reg] = static_cast<u32>(static_cast<i32>(gpr[reg]) >> shift_amount);
            }
            break;
    }
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

void CPU::increment_pc() {
    pc += 4;
}

void CPU::print_all_w() const {
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
    for (int i = 0; i < 31; i++) {
        std::cout << "X" << std::format("{:02}: ", i) << std::format("{:10}", gpr[i]);
        if (i % 4 == 3) {
            std::cout << std::endl;
        } else {
            std::cout << " ";
        }
    }
    std::cout << std::endl;
}

void CPU::print_gpr() const {
    std::cout << "W Registers:" << std::endl;
    print_all_w();
    std::cout << std::endl;
    std::cout << "X Registers:" << std::endl;
    print_all_x();
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

u64 CPU::perform_add_sub(u64 a, u64 b, bool op, bool sf, bool S) {
    u64 carry = op ? 1 : 0;

    if (op) {
        b = ~b;
    }

    if (!sf) {
        a &= UINT32_MAX;
        b &= UINT32_MAX;
    }

    u64 sum = a + b;
    u64 res = sum + carry;

    // Set flags
    if (S) {
        if (sf) {
            set_negative_flag(get_bit(res, 63));
            set_zero_flag(res == 0);
            set_carry_flag((sum < a) || (res < sum));
            set_overflow_flag(
                    (get_bit(a, 63) == get_bit(b, 63)) 
                    && (get_bit(a, 63) != get_bit(res, 63)));

        } else {
            set_negative_flag(get_bit(res, 31));
            set_zero_flag((res & UINT32_MAX) == 0);
            set_carry_flag(res > UINT32_MAX);
            set_overflow_flag(
                    (get_bit(a, 31) == get_bit(b, 31)) 
                    && (get_bit(a, 31) != get_bit(res, 31)));
        }
    }

    return sf ? res : res & UINT32_MAX;
}

void CPU::execute_add_sub_immediate(u32 inst) {
    u8 Rd = get_bitrange(inst, 0, 5);
    u8 Rn = get_bitrange(inst, 5, 10);
    u32 imm12 = get_bitrange(inst, 10, 22);
    bool sh = get_bit(inst, 22);
    bool S = get_bit(inst, 29);
    bool op = get_bit(inst, 30);
    bool sf = get_bit(inst, 31);

    u64 operand1 = (Rn == 31 ? read_sp() : read_x(Rn));
    u64 operand2 = (sh ? imm12 << 12 : imm12);

    if (!sf) {
        operand1 &= UINT32_MAX;
        operand2 &= UINT32_MAX;
    }

    u64 result = perform_add_sub(operand1, operand2, op, sf, S);

    if (Rd == 31) {
        write_sp(result);
    } else {
        sf ? write_x(Rd, result) : write_w(Rd, result);
    }
}

void CPU::execute_add_shifted_register(u32 inst) {
    u8 Rd = get_bitrange(inst, 0, 5);
    u8 Rn = get_bitrange(inst, 5, 10);
    u32 imm6 = get_bitrange(inst, 10, 16);
    u8 Rm = get_bitrange(inst, 16, 21);
    u8 shift = get_bitrange(inst, 22, 24);
    bool sf = get_bit(inst, 31);

    if (shift == 0b11) {
        throw std::runtime_error("Trying to execute an ADD_SHIFTREG instruction with a reserved option.");
    }

    if (sf == 0 && get_bit(imm6, 5) == 1) {
        throw std::runtime_error("Trying to execute an ADD_SHIFTREG instruction where shifting is more than the maximum amount(31)");
    }

}

void CPU::execute(u32 inst, Memory& mem) {
    switch(decode(inst)) {
        case Instruction::ADD_IMMEDIATE:
            execute_add_sub_immediate(inst);
            break;
        case Instruction::SUB_IMMEDIATE:
            execute_add_sub_immediate(inst);
            break;
        default:
            throw std::runtime_error("This instruction has not been implemented.");
    }
}
