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
    class riscv_cpu_decoder {
    private:
        using instr_t = kz::riscv::types::instr_t;
        using dec_instr_t = kz::riscv::types::dec_instr_t;
        using dec_imm_t = kz::riscv::types::dec_imm_t;
        using operation_type_t = kz::riscv::types::operation_type_t;
        static void dec_instr_(instr_t instr, dec_instr_t *p_dec_instr);
        static void dec_imm_(instr_t instr, dec_instr_t *p_dec_instr);
    public:
        /**
         * Decode the given instruction into its components and immediate value.
         * The function extracts fields such as opcode, destination register (rd),
         * source registers (rs1, rs2), function codes (func3, func7),
         * and determines the instruction type. It also decodes the immediate value
         * based on the instruction type.
         * M/O - Mandatory/Optional, In/Out - Input/Output.
         * @param instr [M][In] The instruction to be decoded.
         * @return pointer to the decoded instruction components.
         */
        static void decode(instr_t instr, dec_instr_t *p_dec_instr);
    };
} /* ! kz::riscv::core ! */
