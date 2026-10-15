#ifndef CPU_HPP
#define CPU_HPP

#include "types.hpp"
#include "decode.hpp"
#include "memory.hpp"

class CPU {
    private:
        u64 gpr[31] = {};
        u64 sp = {};
        u64 pc = {};
        u64 nzcv = {};

        bool running = true;

        static constexpr u64 N = (1u << 31);
        static constexpr u64 Z = (1u << 30);
        static constexpr u64 C = (1u << 29);
        static constexpr u64 V = (1u << 28);

        void write_x(u8 x, u64 val); 

        void write_w(u8 w, u32 val); 

        u64 read_sp() const; 

        void write_sp(u64 val); 

        u64 read_pc() const; 

        void write_pc(u64 val); 

        void print_all_w() const; 

        void print_all_x() const; 

        void print_gpr() const; 

        void print_reg_binary(bool sf, u8 reg); 

        void print_sp() const;

        void print_pc() const;

        void print_nzcv() const;

        void increment_pc(); 

        void set_negative_flag(bool b);

        void set_zero_flag(bool b); 

        void set_carry_flag(bool b); 

        void set_overflow_flag(bool b); 

        void execute_immediate_move_wide_instruction(
                u32 Rd,
                u32 imm16,
                u32 hw,
                u32 opc,
                bool sf);

        u64 shift_reg(bool sf, u8 reg, Shift_Type shift_type, u32 shift_amount); 

        u64 perform_add_sub(u64 a, u64 b, bool op, bool sf, bool S); 

        u64 extend_register(u32 reg, Extend_Type extend_type, u32 shift, bool m_width);

        void execute_extended_register_add_sub_instruction(
                u32 Rd,
                u32 Rn,
                u32 imm3,
                u32 option,
                u32 Rm,
                u32 opt,
                bool S,
                bool op,
                bool sf
                );

        void execute_shifted_register_add_sub_instruction(
                u32 Rd,
                u32 Rn,
                u32 imm6,
                u32 Rm,
                u32 shift,
                bool S,
                bool op,
                bool sf
                );

        void execute_immediate_add_sub_instruction(
                u32 Rd,
                u32 Rn,
                u32 imm12,
                bool sh,
                bool S,
                bool op,
                bool sf
                );

        void decode_data_processing_immediate(u32 inst);

        void decode_register_instruction(u32 inst);

        void decode_immediate_instruction(u32 inst);

    public:
        u64 read_x(u8 n) const; 

        u32 read_w(u8 n) const; 

        bool get_negative_flag() const;

        bool get_zero_flag() const; 

        bool get_carry_flag() const; 

        bool get_overflow_flag() const;

        void print_state() const;

        void halt();

        bool is_running() const;

        u32 fetch(Memory& mem); 

        void decode(u32 inst);

        void execute(Memory& mem); 
};

#endif
