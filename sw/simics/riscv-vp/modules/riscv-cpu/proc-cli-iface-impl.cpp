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
#include "riscv-cpu.hpp"
#include "riscv-cpu-disasm.hpp"
#include <simics/util/strbuf.h>

namespace kz::riscv::core {
    tuple_int_string_t riscv_cpu::get_disassembly(
        const char *addr_prefix,
        generic_address_t address,
        bool print_cpu,
        const char *mnemonic) {
        strbuf_t result_sb = sb_new("");
        // verify parameters
        if (addr_prefix == nullptr) {
            addr_prefix = "p";
        }
        if (mnemonic != nullptr) {
            sb_addstr(&result_sb, mnemonic);
            return {0, sb_detach(&result_sb)};
        }
        if (addr_prefix[0] != 'p' && addr_prefix[0] != 'l' && addr_prefix[0] != 'v') {
            SIM_LOG_INFO(
                4, cobj_, 0,
                "Invalid address prefix: '%s' at address: 0x%08x",
                addr_prefix, static_cast<unsigned int>(address)
            );
            return { 0, nullptr };
        }
        // read instruction data from memory
        addr_t addr = static_cast<addr_t>(address);
        uint8* data = read_mem_(addr, DATA_SIZE);
        SIM_LOG_INFO(
            4, cobj_, 0,
            "direct memory: data[0]='0x%02x', data[1]='0x%02x', data[2]='0x%02x', data[3]='0x%02x'",
            data[0], data[1], data[2], data[3]
        );
        // disassemble instruction
        attr_value_t instr_data = SIM_make_attr_data(DATA_SIZE, data);
        tuple_int_string_t result = this->disassemble(address, instr_data, 0);
        // add cpu name as a prefix
        if (print_cpu) {
            std::string cpu_disasm_str = std::string(MODULE_NAME) + ": " + std::string(result.string);
            sb_addstr(&result_sb, cpu_disasm_str.c_str());
            return { result.integer, sb_detach(&result_sb) };
        } else {
            return result;
        }
    }

    char *riscv_cpu::get_pregs(bool all) {
        strbuf_t pregs_sb = sb_new("");
        for (int i = 0; i < RV32I_GP_REG_NUM; ++i) {
            sb_addfmt(
                &pregs_sb, "%s (%s) = 0x%08X\n",
                riscv_cpu_disasm::get_reg_name(i, false).c_str(),
                riscv_cpu_disasm::get_reg_name(i, true).c_str(),
                regs_[i]
            );
        }
        if (all) {
            sb_addfmt(&pregs_sb, "%s = 0x%08X\n", get_name(32), pc_);
            sb_addfmt(&pregs_sb, "%s = 0x%08X\n", get_name(33), mstatus_);
            sb_addfmt(&pregs_sb, "%s = 0x%08X\n", get_name(34), mepc_);
            sb_addfmt(&pregs_sb, "%s = 0x%08X\n", get_name(35), mtvec_);
        }
        // detach the string so Simics owns the memory now
        return sb_detach(&pregs_sb);
    }

    attr_value_t riscv_cpu::get_diff_regs() {
        attr_value_t result = SIM_alloc_attr_list(ALL_REGS_NUM);
        // general purpose registers x0..x31
        for (int i = 0; i < RV32I_GP_REG_NUM; ++i) {
            SIM_attr_list_set_item(
                &result, i,
                SIM_make_attr_string(riscv_cpu_disasm::get_reg_name(i, true).c_str())
            );
        }
        // other registers
        for (int i = RV32I_GP_REG_NUM; i < ALL_REGS_NUM; ++i) {
            SIM_attr_list_set_item(&result, i, SIM_make_attr_string(get_name(i)));
        }
        return result;
    }

    char *riscv_cpu::get_pending_exception_string() {
        // Check for pending exception or interrupt
        if (mcause_ != 0) {
            // Format a message describing the exception
            strbuf_t exc_sb = sb_new("");
            sb_addfmt(&exc_sb, "Pending exception: mcause=0x%08X", mcause_);
            return sb_detach(&exc_sb);
        }
        // No pending exception
        return nullptr;
    }

    char *riscv_cpu::get_address_prefix() {
        strbuf_t addr_prefix_sb = sb_new("p");
        return sb_detach(&addr_prefix_sb);
    }

    physical_block_t riscv_cpu::translate_to_physical(const char *prefix, generic_address_t address) {
        physical_block_t block = {};
        // Accept "p" (physical) and "v" (virtual) prefixes and "l" (linear) (no MMU)
        if (prefix && (prefix[0] == 'p' || prefix[0] == 'v' || prefix[0] == 'l')) {
            block.valid = 1;
            block.address = address;
            block.block_start = address;
            block.block_end = address + 3; // 4-byte block for RV32I
        } else {
            // Invalid or unsupported prefix
            block.valid = 0;
            block.address = 0;
            block.block_start = 0;
            block.block_end = 0;
        }
        return block;
    }
} /* ! kz::riscv::core ! */
