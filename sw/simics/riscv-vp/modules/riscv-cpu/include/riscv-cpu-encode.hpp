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

#include "riscv-cpu-types.hpp"

namespace kz::riscv::core {
    class RiscvCpuEncoder {
    private:
        using dec_imm_t = kz::riscv::types::dec_imm_t;
    public:
        /**
         * Encode the given immediate value into the I-type immediate format.
         * The function takes a decoded immediate value and encodes it into
         * the 12-bit I-type immediate format used in RISC-V instructions.
         * M/O - Mandatory/Optional, In/Out - Input/Output.
         * @param dec_imm [M][In] The decoded immediate value to be encoded.
         * @return The encoded 12-bit I-type immediate value.
         */
        static kz::riscv::types::i_imm_t enc_i_imm(dec_imm_t dec_imm);
        /**
         * Encode the given immediate value into the S-type immediate format.
         * The function takes a decoded immediate value and encodes it into
         * the 12-bit S-type immediate format used in RISC-V instructions.
         * M/O - Mandatory/Optional, In/Out - Input/Output.
         * @param dec_imm [M][In] The decoded immediate value to be encoded.
         * @return The encoded 12-bit S-type immediate value.
         */
        static kz::riscv::types::s_imm_t enc_s_imm(dec_imm_t dec_imm);
        /**
         * Encode the given immediate value into the B-type immediate format.
         * The function takes a decoded immediate value and encodes it into
         * the 12-bit B-type immediate format used in RISC-V instructions.
         * M/O - Mandatory/Optional, In/Out - Input/Output.
         * @param dec_imm [M][In] The decoded immediate value to be encoded.
         * @return The encoded 12-bit B-type immediate value.
         */
        static kz::riscv::types::b_imm_t enc_b_imm(dec_imm_t dec_imm);
        /**
         * Encode the given immediate value into the U-type immediate format.
         * The function takes a decoded immediate value and encodes it into
         * the 20-bit U-type immediate format used in RISC-V instructions.
         * M/O - Mandatory/Optional, In/Out - Input/Output.
         * @param dec_imm [M][In] The decoded immediate value to be encoded.
         * @return The encoded 20-bit U-type immediate value.
         */
        static kz::riscv::types::u_imm_t enc_u_imm(dec_imm_t dec_imm);
        /**
         * Encode the given immediate value into the J-type immediate format.
         * The function takes a decoded immediate value and encodes it into
         * the 20-bit J-type immediate format used in RISC-V instructions.
         * M/O - Mandatory/Optional, In/Out - Input/Output.
         * @param dec_imm [M][In] The decoded immediate value to be encoded.
         * @return The encoded 20-bit J-type immediate value.
         */
        static kz::riscv::types::j_imm_t enc_j_imm(dec_imm_t dec_imm);
        /**
         * Sign-extend the given value to the specified number of bits.
         * The function takes a value and sign-extends it to the specified
         * bit width, ensuring that the sign bit is correctly propagated.
         * M/O - Mandatory/Optional, In/Out - Input/Output.
         * @param value [M][In] The value to be sign-extended.
         * @param bits [M][In] The target bit width for sign extension.
         * @return The sign-extended value.
         */
        static int32_t sign_extend_(int32_t value, int bits);
    };
} /* ! kz::riscv::core ! */
