#include "types.hpp"
#include "cpu.hpp"
#include "bits.hpp"
#include "decode.hpp"

u64 CPU::extend_register(u32 reg, Extend_Type extend_type, u32 shift, bool m_width) {
    u64 value = m_width ? read_x(reg) : read_w(reg);
    switch(extend_type) {
        case Extend_Type::UXTB:
            value &= UINT8_MAX;
            break;
        case Extend_Type::UXTH:
            value &= UINT16_MAX;
            break;
        case Extend_Type::UXTW:
            value &= UINT32_MAX;
            break;
        case Extend_Type::UXTX:
            value &= UINT64_MAX;
            break;
        case Extend_Type::SXTB:
            get_bit(value, 7) 
                ? value |= ~(static_cast<i64>(UINT8_MAX))
                : value &= UINT8_MAX; 
            break;
        case Extend_Type::SXTH:
            get_bit(value, 15) 
                ? value |= ~(static_cast<i64>(UINT16_MAX))
                : value &= UINT16_MAX; 
            break;
        case Extend_Type::SXTW:
            get_bit(value, 31) 
                ? value |= ~(static_cast<i64>(UINT32_MAX))
                : value &= UINT32_MAX; 
            break;
        case Extend_Type::SXTX:
            get_bit(value, 63) 
                ? value |= ~(static_cast<i64>(UINT64_MAX))
                : value &= UINT64_MAX; 
            break;
    }

    return value << shift;
}

void CPU::execute_extended_register_add_sub_instruction(u32 Rd, u32 Rn, u32 imm3, u32 option, u32 Rm, u32 opt, bool S, bool op, bool sf) {
    if (imm3 > 4) {
        undefined();
    }
    u32 d = Rd;
    u32 n = Rn;
    u32 m = Rm;
    u32 shift = imm3;
    Extend_Type extend_type = static_cast<Extend_Type>(option);

    u32 operand1 = n == 31 ? read_sp() : read_x(n);
    u32 operand2 = extend_register(m, extend_type, shift, (opt & 0b011) == 0b011);

    u64 result = perform_add_sub(operand1, operand2, op, sf, S);

    d == 31 ? write_sp(result) : (sf ? write_x(d, result) : write_w(d, result));
}

void CPU::execute_shifted_register_add_sub_instruction(u32 Rd, u32 Rn, u32 imm6, u32 Rm, u32 shift, bool S, bool op, bool sf) {
    if (shift == 0b11) {
        undefined();
    }
    if ((sf == 0) && get_bit(imm6, 5) == 1) {
        undefined();
    }

    u32 d = Rd;
    u32 n = Rn;
    u32 m = Rm;
    Shift_Type shift_type = static_cast<Shift_Type>(shift);
    u32 shift_amount = imm6;

    u64 operand1 = sf ? read_x(n) : read_w(n);
    u64 operand2 = shift_reg(sf, m, shift_type, shift_amount);
    u64 result;

    result = perform_add_sub(operand1, operand2, op, sf, S);

    sf ? write_x(d, result) : write_w(d, result);
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


void CPU::execute_immediate_add_sub_instruction(u32 Rd, u32 Rn, u32 imm12, bool sh, bool S, bool op, bool sf) {
    u32 d = Rd;
    u32 n = Rn;
    u32 imm = imm12 << (12 * sh);

    u64 operand1 = (n == 31 ? read_sp() : read_x(n));
    u64 operand2 = imm;
    if (!sf) {
        operand1 &= UINT32_MAX;
        operand2 &= UINT32_MAX;
    }

    u64 result = perform_add_sub(operand1, operand2, op, sf, S);

    if (d == 31) {
        sf ? write_sp(result) : write_sp(result & UINT32_MAX);
    } else {
        sf ? write_x(Rd, result) : write_w(Rd, result);
    }
}

// https://www.scs.stanford.edu/~zyedidia/arm64/encodingindex.html#movewide
void CPU::execute_immediate_move_wide_instruction(u32 Rd, u32 imm16, u32 hw, u32 opc, bool sf) {
    if ((sf == 0) && get_instruction_bit(hw, 1) == 1) {
        undefined();
    }
    u32 d = Rd;
    u32 imm = imm16;
    u32 pos = hw << 4;
    u64 result;

    switch(opc) {
        // opc = 0b00 - MOVN
        case 0b00:
            result = 0;
            result = insert_bits_at_pos(result, imm, 16, pos);
            sf ? write_x(d, ~result) : write_w(d, ~result);
            break;

            // opc = 0b01 - UNALLOCATED
        case 0b01:
            unallocated();
            break;

            // opc = 0b10 - MOVZ
        case 0b10:
            result = 0;
            result = insert_bits_at_pos(result, imm, 16, pos);
            sf ? write_x(d, result) : write_w(d, result);
            break;

            // opc = 0b11 - MOVK
        case 0b11:
            result = read_x(d);
            result = insert_bits_at_pos(result, imm, 16, pos);
            sf ? write_x(d, result) : write_w(d, result);
            break;
    }
}

