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

#include "riscv-cpu.hpp"
#include <simics/util/strbuf.h>

namespace kz::riscv::core {
    tuple_int_string_t riscv_cpu::get_disassembly(
        const char *addr_prefix,
        generic_address_t address,
        bool print_cpu,
        const char *mnemonic) {
        // TBD
        static char result[] = "";
        return { 0, result };
    }

    char *riscv_cpu::get_pregs(bool all) {
        strbuf_t sb = sb_new("");
        for (int i = 0; i < RV32I_GP_REG_NUM; ++i) {
            sb_addfmt(&sb, "%s = 0x%08X\n", get_name(i), regs_[i]);
        }
        if (all) {
            sb_addfmt(&sb, "%s = 0x%08X\n", get_name(32), pc_);
            sb_addfmt(&sb, "%s = 0x%08X\n", get_name(33), mstatus_);
            sb_addfmt(&sb, "%s = 0x%08X\n", get_name(34), mepc_);
            sb_addfmt(&sb, "%s = 0x%08X\n", get_name(35), mtvec_);
        }
        // detach the string so Simics owns the memory now
        return sb_detach(&sb);
    }

    attr_value_t riscv_cpu::get_diff_regs() {
        attr_value_t result = SIM_alloc_attr_list(1);
        SIM_attr_list_set_item(&result, 0, SIM_make_attr_int64(1));
        return result;
    }

    char *riscv_cpu::get_pending_exception_string() {
        static char result[] = "";
        return result;
    }

    char *riscv_cpu::get_address_prefix() {
        static char result[] = "p";
        return result;
    }

    physical_block_t riscv_cpu::translate_to_physical(
        const char *prefix,
        generic_address_t address) {
        // TBD
        // typedef struct {
        //         int                valid;
        //         physical_address_t address;
        //         physical_address_t block_start;
        //         physical_address_t block_end;
        // } physical_block_t;
        return { 0, 0x0, 0x0, 0x1 };
    }
} /* ! kz::riscv::core ! */
