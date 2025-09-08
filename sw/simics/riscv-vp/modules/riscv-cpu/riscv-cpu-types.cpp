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

#include "riscv-cpu-types.hpp"
#include "riscv-cpu-decode.hpp"

namespace kz::riscv::types {
    op_type_t operation_type::get_subtype_00_(uint_n<3> opcl) {
        switch(opcl) {
            case 0b000: return I_TYPE; // LOAD
            case 0b001: return OTHER_TYPE; // LOAD_FP
            case 0b010: return OTHER_TYPE; // CUSTOM_0
            case 0b011: return OTHER_TYPE; // MISC_MEM
            case 0b100: return I_TYPE; // OP_IMM
            case 0b101: return U_TYPE; // AUIPC
            case 0b110: return OTHER_TYPE; // OP_IMM_32
            case 0b111: return OTHER_TYPE; // RV_48_0
        }
        return UNDEF_TYPE;
    }

    op_type_t operation_type::get_subtype_01_(uint_n<3> opcl) {
        switch(opcl) {
            case 0b000: return S_TYPE; // STORE
            case 0b001: return OTHER_TYPE; // STORE_FP
            case 0b010: return OTHER_TYPE; // CUSTOM_1
            case 0b011: return OTHER_TYPE; // AMO
            case 0b100: return R_TYPE; // OP
            case 0b101: return U_TYPE; // LUI
            case 0b110: return OTHER_TYPE; // OP_32
            case 0b111: return OTHER_TYPE; // RV_64
        }
        return UNDEF_TYPE;
    }

    op_type_t operation_type::get_subtype_10_(uint_n<3> opcl) {
        switch(opcl) {
            case 0b000: return OTHER_TYPE; // MADD
            case 0b001: return OTHER_TYPE; // MSUB
            case 0b010: return OTHER_TYPE; // NMSUB
            case 0b011: return OTHER_TYPE; // NMADD
            case 0b100: return OTHER_TYPE; // OP_FP
            case 0b101: return OTHER_TYPE; // RSVD_0
            case 0b110: return OTHER_TYPE; // CUSTOM_2_RV128
            case 0b111: return OTHER_TYPE; // RV_80
        }
        return UNDEF_TYPE;
    }

    op_type_t operation_type::get_subtype_11_(uint_n<3> opcl) {
        switch(opcl) {
            case 0b000: return B_TYPE; // BRANCH
            case 0b001: return I_TYPE; // JALR
            case 0b010: return OTHER_TYPE; // RSVD_1
            case 0b011: return J_TYPE; // JAL
            case 0b100: return OTHER_TYPE; // SYSTEM
            case 0b101: return OTHER_TYPE; // RSVD_2
            case 0b110: return OTHER_TYPE; // CUSTOM_3_RV128
            case 0b111: return OTHER_TYPE; // RV_80
        }
        return UNDEF_TYPE;
    }

    op_type_t operation_type::get_op_type(opcode_t opcode) {
        uint_n<2> opch = opcode >> 3;
        uint_n<3> opcl = opcode >> 0;
        switch(opch) {
            case 0b00: return get_subtype_00_(opcl);
            case 0b01: return get_subtype_01_(opcl);
            case 0b10: return get_subtype_10_(opcl);
            case 0b11: return get_subtype_11_(opcl);
        }
        return UNDEF_TYPE;
    }
} /* ! kz::riscv::types ! */
