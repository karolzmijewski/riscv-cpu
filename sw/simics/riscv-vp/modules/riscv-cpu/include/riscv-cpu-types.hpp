/**
 * Copyright © 2025 Karol Zmijewski
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this
 * software and associated documentation files (the “Software”), to deal in the Software
 * without restriction, including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
 * to whom the Software is furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 *
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
 * FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#pragma once

#include <cstdint>
#include <type_traits>
#include "riscv-cpu-conf.hpp"

namespace kz::riscv::types {
    template<unsigned bits, typename Storage, typename Derived>
    class bits_n {
    protected:
        Storage val;
        using unsigned_storage_t = typename std::make_unsigned<Storage>::type;
        using signed_storage_t = typename std::make_signed<Storage>::type;
        static_assert(bits > 0, "Bit width must be grater then 0");
        static_assert(bits <= sizeof(Storage) * 8, "Bit width cannot be larger then storage type");
        static constexpr Storage mask = (bits == sizeof(Storage) * 8)
            ? ~Storage(0) /* bit mask with 1 on all bits */
            : ((Storage(1) << bits) - 1); /* bit mask with 1 on bits_size -1 */
        static const Storage apply_mask(Storage v) {
            return static_cast<Storage>(v & mask);
        }
        constexpr bits_n(Storage v): val(apply_mask(v)) {}
    public:
        constexpr bits_n(): val(0) {}
        // cast
        constexpr operator Storage() const { return val; }
        // assignment
        constexpr Derived& operator=(Storage v) {
            val = apply_mask(v);
            return static_cast<Derived&>(*this);
        }
        // arthmetic
        constexpr Derived operator+(Storage rhs) const {
            return Derived(val + rhs);
        }
        constexpr Derived operator-(Storage rhs) const {
            return Derived(val - rhs);
        }
        // logic
        constexpr Derived operator|(Storage rhs) const {
            return Derived(apply_mask(val | rhs));
        }
        constexpr Derived operator|(const Derived& rhs) const {
            return Derived(apply_mask(val | rhs.val));
        }
        constexpr Derived& operator|=(Storage rhs) {
            val = apply_mask(val | rhs);
            return static_cast<Derived&>(*this);
        }
        constexpr Derived& operator|=(const Derived& rhs) {
            val = apply_mask(val | rhs.val);
            return static_cast<Derived&>(*this);
        }
        constexpr Derived operator&(Storage rhs) const {
            return Derived(apply_mask(val & rhs));
        }
        constexpr Derived operator&(const Derived& rhs) const {
            return Derived(apply_mask(val & rhs.val));
        }
        constexpr Derived& operator&=(Storage rhs) {
            val = apply_mask(val & rhs);
            return static_cast<Derived&>(*this);
        }
        constexpr Derived& operator&=(const Derived& rhs) {
            val = apply_mask(val & rhs.val);
            return static_cast<Derived&>(*this);
        }
    };

    template<unsigned bits>
    class uint_n: public bits_n<bits, uint64_t, uint_n<bits>> {
    private:
        using base = bits_n<bits, uint64_t, uint_n<bits>>;
    public:
        constexpr uint_n(): base() {}
        constexpr uint_n(uint64_t v): base(v) {}
    };

    template<unsigned bits>
    class int_n: public bits_n<bits, int64_t, int_n<bits>> {
    private:
        using base = bits_n<bits, int64_t, int_n<bits>>;
        using unsigned_storage_t = typename base::unsigned_storage_t;
    protected:
        static constexpr int64_t sign_extend(unsigned_storage_t v) {
            if constexpr (bits == 64) {
                return static_cast<int64_t>(v);
            } else {
                unsigned shift = 64 - bits;
                // first move value to the most significant bits and get back to propagate sign
                return (static_cast<int64_t>(v << shift)) >> shift;
            }
        }
        static constexpr int64_t apply_mask(int64_t v) {
            return sign_extend(static_cast<unsigned_storage_t>(v) & base::mask);
        }
    public:
        constexpr int_n(): base() {}
        constexpr int_n(int64_t v): base(apply_mask(v)) {}
        constexpr int_n& operator=(int64_t v) {
            this->val = apply_mask(v);
            return *this;
        }
    };

    // general types
    using bit_t = uint_n<1>;

    // basic types
    using instr_t = unsigned int;
    using addr_t = uint_n<kz::riscv::core::RAM_ADDR_WIDTH>;
    using op_type_t = uint_n<3>;

    // instr frame fileds
    using opcode_t = uint_n<5>;
    using reg_nr_t = uint_n<5>;
    using func3_t = uint_n<3>;
    using func7_t = uint_n<7>;
    using imm_t = int_n<20>;

    // imm_t - immediate field varieties
    using i_imm_t = int_n<12>;
    using s_imm_t = int_n<12>;
    using b_imm_t = int_n<12>;
    using u_imm_t = int_n<20>;
    using j_imm_t = int_n<20>;

    class dec_instr {
    public:
        opcode_t opcode;
        reg_nr_t rd;
        func3_t func3;
        reg_nr_t rs1;
        reg_nr_t rs2;
        func7_t func7;
        op_type_t type;
        imm_t imm;
    };
    using dec_instr_t = dec_instr;

    class dec_imm {
    public:
        bit_t instr_31;
        uint_n<6> instr_30_25;
        uint_n<4> instr_24_21;
        bit_t instr_20;
        uint_n<8> instr_19_12;
        uint_n<4> instr_11_8;
        bit_t instr_7;
    };
    using dec_imm_t = dec_imm;

    class operation_type {
    private:
        static op_type_t get_subtype_00_(uint_n<3> opcl);
        static op_type_t get_subtype_01_(uint_n<3> opcl);
        static op_type_t get_subtype_10_(uint_n<3> opcl);
        static op_type_t get_subtype_11_(uint_n<3> opcl);
    public:
        static const uint8_t UNDEF_TYPE = 0;
        static const uint8_t R_TYPE = 1;
        static const uint8_t I_TYPE = 2;
        static const uint8_t S_TYPE = 3;
        static const uint8_t B_TYPE = 4;
        static const uint8_t U_TYPE = 5;
        static const uint8_t J_TYPE = 6;
        static const uint8_t OTHER_TYPE = 7;
        /**
         * It's a representation of main MUX (multiplexer 3*4 -> 3) logic,
         * used to decode opch, and call the appropriate sub-MUX representation
         * function to decode opcl and determine the operation type.
         * M/O - Mandatory/Optional, In/Out - Input/Output.
         * @param opcode [M][In] The opcode value to determine the operation type.
         * @return The operation type corresponding to the given opcode.
         */
        static op_type_t get_op_type(opcode_t opcode);
    };
    using operation_type_t = operation_type;

    class operation_code {
    public:
        // opcodes special values
        static const uint8_t NOP = 0x13;
        static const uint8_t RA = 0x1;
        // opcodes 00
        static const uint8_t LOAD = 0b00000;
        static const uint8_t LOAD_FP = 0b00001;
        static const uint8_t CUSTOM_0 = 0b00010;
        static const uint8_t MISC_MEM = 0b00011;
        static const uint8_t OP_IMM = 0b00100;
        static const uint8_t AUIPC = 0b00101;
        static const uint8_t OP_IMM_32 = 0b00110;
        static const uint8_t RV48_0 = 0b00111;
        // opcodes 01
        static const uint8_t STORE = 0b01000;
        static const uint8_t STORE_FP = 0b01001;
        static const uint8_t CUSTOM_1 = 0b01010;
        static const uint8_t AMO = 0b01011;
        static const uint8_t OP = 0b01100;
        static const uint8_t LUI = 0b01101;
        static const uint8_t OP_32 = 0b01110;
        static const uint8_t RV_64 = 0b01111;
        // opcodes 10
        static const uint8_t MADD = 0b10000;
        static const uint8_t MSUB = 0b10001;
        static const uint8_t NMSUB = 0b10010;
        static const uint8_t NMADD = 0b10011;
        static const uint8_t OP_FP = 0b10100;
        static const uint8_t RSVD_0 = 0b10101;
        static const uint8_t CUSTOM_2_RV128 = 0b10110;
        static const uint8_t RV48_1 = 0b10111;
        // opcodes 11
        static const uint8_t BRANCH = 0b11000;
        static const uint8_t JALR = 0b11001;
        static const uint8_t RSVD_1 = 0b11010;
        static const uint8_t JAL = 0b11011;
        static const uint8_t SYSTEM = 0b11100;
        static const uint8_t RSVD_2 = 0b11101;
        static const uint8_t CUSTOM_3_RV128 = 0b11110;
        static const uint8_t RV_80 = 0b11111;
    };
    using operation_code_t = operation_code;
} /* ! kz::riscv::types ! */
