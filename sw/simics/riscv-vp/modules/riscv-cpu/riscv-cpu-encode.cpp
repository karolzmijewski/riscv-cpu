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
#include "riscv-cpu-encode.hpp"

namespace kz::riscv::core {
    kz::riscv::types::i_imm_t riscv_cpu_encoder::enc_i_imm(dec_imm_t dec_imm) {
        using i_imm_t = kz::riscv::types::i_imm_t;
        i_imm_t enc_i_imm_val = ((i_imm_t)dec_imm.instr_31 << 11);
        enc_i_imm_val |= ((i_imm_t)dec_imm.instr_30_25 << 5);
        enc_i_imm_val |= ((i_imm_t)dec_imm.instr_24_21 << 1);
        enc_i_imm_val |= ((i_imm_t)dec_imm.instr_20);
        return enc_i_imm_val;
    }
    
    kz::riscv::types::s_imm_t riscv_cpu_encoder::enc_s_imm(dec_imm_t dec_imm) {
        using s_imm_t = kz::riscv::types::s_imm_t;
        s_imm_t enc_s_imm_val = ((s_imm_t)dec_imm.instr_31 << 11);
        enc_s_imm_val |= ((s_imm_t)dec_imm.instr_30_25 << 5);
        enc_s_imm_val |= ((s_imm_t)dec_imm.instr_11_8 << 1);
        enc_s_imm_val |= ((s_imm_t)dec_imm.instr_7);
        return enc_s_imm_val;
    }

    kz::riscv::types::b_imm_t riscv_cpu_encoder::enc_b_imm(dec_imm_t dec_imm) {
        using b_imm_t = kz::riscv::types::b_imm_t;
        b_imm_t enc_b_imm_val = ((b_imm_t)dec_imm.instr_31 << 11);
        enc_b_imm_val |= ((b_imm_t)dec_imm.instr_7 << 10);
        enc_b_imm_val |= ((b_imm_t)dec_imm.instr_30_25 << 4);
        enc_b_imm_val |= ((b_imm_t)dec_imm.instr_11_8);
        return enc_b_imm_val;
    }

    kz::riscv::types::u_imm_t riscv_cpu_encoder::enc_u_imm(dec_imm_t dec_imm) {
        using u_imm_t = kz::riscv::types::u_imm_t;
        u_imm_t enc_u_imm_val = ((u_imm_t)dec_imm.instr_31 << 19);
        enc_u_imm_val |= ((u_imm_t)dec_imm.instr_30_25 << 13);
        enc_u_imm_val |= ((u_imm_t)dec_imm.instr_24_21 << 9);
        enc_u_imm_val |= ((u_imm_t)dec_imm.instr_20 << 8);
        enc_u_imm_val |= ((u_imm_t)dec_imm.instr_19_12);
        return enc_u_imm_val;
    }

    kz::riscv::types::j_imm_t riscv_cpu_encoder::enc_j_imm(dec_imm_t dec_imm) {
        using j_imm_t = kz::riscv::types::j_imm_t;
        j_imm_t enc_j_imm_val = ((j_imm_t)dec_imm.instr_31 << 19);
        enc_j_imm_val |= ((j_imm_t)dec_imm.instr_19_12 << 11);
        enc_j_imm_val |= ((j_imm_t)dec_imm.instr_20 << 10);
        enc_j_imm_val |= ((j_imm_t)dec_imm.instr_30_25 << 4);
        enc_j_imm_val |= ((j_imm_t)dec_imm.instr_24_21);
        return enc_j_imm_val;
    }
} /* ! kz::riscv::core ! */
