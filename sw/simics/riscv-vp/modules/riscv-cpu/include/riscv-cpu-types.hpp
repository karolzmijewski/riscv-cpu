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

namespace kz::riscv::types {
    constexpr unsigned RAM_ADDR_WIDTH = 16;

    template<unsigned bits, typename Storage, typename Derived>
    class bits_n {
    protected:
        using unsigned_storage_t = std::make_unsigned<Storage>;
        using signed_storage_t = std::make_signed<Storage>;
        static_assert(bits > 0, "Bit width must be grater then 0");
        static_assert(bits <= sizeof(Storage) * 8, "Bit width cannot be larger then storage type");
        static constexpr unsigned_storage_t mask = (bits == sizeof(Storage) *8) 
            ? ~unsigned_storage_t(0) /* bit mask with 1 on all bits */
            : ((unsigned_storage_t(1) << bits) - 1); /* bit mask with 1 on bits_size -1 */
        static const Storage apply_mask(Storage v) {
            return static_cast<Storage>(v & mask);
        }
        Storage val;
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
        static constexpr int64_t sign_extend(unsigned_storage_t v) {
            if constexpr (bits == 64) {
                return static_cast<int64_t>(v);
            } else {
                unsigned shift = 64 - bits;
                // first move value to the most significant bits and get back to propagate sign
                return static_cast<int64_t>(static_cast<int64_t>(v << shift) >> shift);
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
    using addr_t = uint_n<RAM_ADDR_WIDTH>;
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
        opcode_t type;
        imm_t imm;
    };
    using dec_instr_t = dec_instr;

    class dec_imm {
        bit_t instr_31;
        uint_n<6> instr_30_25;
        uint_n<4> instr_24_21;
        bit_t instr_20;
        uint_n<8> instr_19_12;
        uint_n<4> instr_11_8;
        bit_t instr_7;
    };
    using dec_imm_t = dec_imm;
}