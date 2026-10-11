#ifndef CPU_HPP
#define CPU_HPP

#include "types.hpp"
#include "bits.hpp"
#include "memory.hpp"

class CPU {
    private:
        u64 gpr[31]{};
        u64 sp{};
        u64 pc{};
        u64 nzcv{};

        static constexpr u64 N = (1u << 31);
        static constexpr u64 Z = (1u << 30);
        static constexpr u64 C = (1u << 29);
        static constexpr u64 V = (1u << 28);

        void shift_reg(bool sf, u8 reg, Shift_Type shift_type, u32 shift_amount); 

        bool get_negative_flag() const;

        bool get_zero_flag() const; 

        bool get_carry_flag() const; 

        bool get_overflow_flag() const;

        void set_negative_flag(bool b);

        void set_zero_flag(bool b); 

        void set_carry_flag(bool b); 

        void set_overflow_flag(bool b); 

    public:
        u64 read_x(const u8 n) const; 

        void write_x(const u8 x, const u64 val); 

        u32 read_w(const u8 n) const; 

        void write_w(const u8 w, const u32 val); 

        u64 read_sp() const; 

        void write_sp(u64 val); 

        u64 read_pc() const; 

        void write_pc(u64 val); 

        void increment_pc(); 

        void print_all_w() const; 

        void print_all_x() const; 

        void print_gpr() const; 

        u32 fetch(Memory& mem); 

        void print_reg_b(bool sf, u8 reg); 

        u64 perform_add_sub(u64 a, u64 b, bool op, bool sf, bool S); 

        void execute_add_sub_immediate(u32 inst); 

        void execute_add_shifted_register(u32 inst); 

        void execute(u32 inst, Memory& mem); 
};

#endif
