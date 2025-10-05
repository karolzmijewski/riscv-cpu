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
#include "riscv-cpu-conf.hpp"
#include "riscv-cpu-types.hpp"
#include "riscv-cpu-decode.hpp"
#include "riscv-cpu-disasm.hpp"

namespace kz::riscv::core {
    void riscv_cpu::set_program_counter(logical_address_t pc) {
        pc_ = static_cast<uint32_t>(pc);
    }

    logical_address_t riscv_cpu::get_program_counter() {
        return static_cast<logical_address_t>(pc_);
    }

    tuple_int_string_t riscv_cpu::disassemble(
        generic_address_t address,
        attr_value_t instruction_data,
        int sub_operation) {
        strbuf_t result_sb = sb_new("");
        // set type aliases
        using addr_t = kz::riscv::types::addr_t;
        using instr_t = kz::riscv::types::instr_t;
        using dec_instr_t = kz::riscv::types::dec_instr_t;
        using operation_code_t = kz::riscv::types::operation_code_t;
        // check instruction data
        if (!SIM_attr_is_data(instruction_data)) {
            SIM_LOG_INFO(
                4, cobj_, 0,
                "Invalid instruction data at address: 0x%08x",
                static_cast<unsigned int>(address)
            );
            sb_addstr(&result_sb, "");
            return {0, sb_detach(&result_sb)};
        }
        // check instruction data size
        unsigned size = SIM_attr_data_size(instruction_data);
        if (size < INSTR_SIZE) {
            SIM_LOG_INFO(
                4, cobj_, 0,
                "Invalid instruction data size (%u) at address: 0x%08x",
                size, static_cast<unsigned int>(address)
            );
            sb_addstr(&result_sb, "");
            return {0, sb_detach(&result_sb)};
        }
        // read instruction data
        const uint8 *data = SIM_attr_data(instruction_data);
        instr_t instr = (static_cast<instr_t>(data[3]) << 24);
        instr |= (static_cast<instr_t>(data[2]) << 16);
        instr |= (static_cast<instr_t>(data[1]) << 8);
        instr |= (static_cast<instr_t>(data[0]));
        if (instr == operation_code_t::NOP) {
            SIM_LOG_INFO(
                4, cobj_, 0,
                "addr: 0x%08x disassembled: nop",
                static_cast<unsigned int>(address)
            );
            sb_addstr(&result_sb, "nop");
            return {INSTR_SIZE, sb_detach(&result_sb)};
        }
        SIM_LOG_INFO(4, cobj_, 0, "instr: '0x%04x'", instr);
        // decode instruction
        dec_instr_t dec_instr;
        riscv_cpu_decoder::decode(instr, &dec_instr);
        SIM_LOG_INFO(
            4, cobj_, 0,
            "dec_instr: opcode='0x%02x', rd='0x%02x', func3='0x%01x', "
            "rs1='0x%02x', rs2='0x%02x', func7='0x%02x', type='0x%01x'",
            static_cast<unsigned int>(dec_instr.opcode), static_cast<unsigned int>(dec_instr.rd),
            static_cast<unsigned int>(dec_instr.func3), static_cast<unsigned int>(dec_instr.rs1),
            static_cast<unsigned int>(dec_instr.rs2), static_cast<unsigned int>(dec_instr.func7),
            static_cast<unsigned int>(dec_instr.type)
        );
        // disassemble instruction
        addr_t addr = static_cast<addr_t>(address);
        std::string disasm_instr = riscv_cpu_disasm::disasm(addr, dec_instr);
        SIM_LOG_INFO(
            4, cobj_, 0,
            "addr: 0x%08x disassembled: %s",
            static_cast<unsigned int>(addr), disasm_instr.c_str()
        );
        sb_addstr(&result_sb, disasm_instr.c_str());
        return {INSTR_SIZE, sb_detach(&result_sb)};
    }

    physical_block_t riscv_cpu::logical_to_physical(logical_address_t address, access_t access_type) {
        // So far no MMu no paging, direct mapping. The logical and physical address are the same.
        // The block is a single address, no range. This is standard for simple CPUs or when running
        // in machine mode without virtual memory.
        physical_block_t block = {};
        block.valid = 1; // Mark as valid mapping
        block.address = address; // Direct mapping
        block.block_start = address; // Start of the block (single address)
        block.block_end = address + 3; // Assuming 4-byte access
        return block;
    }

    processor_mode_t riscv_cpu::get_processor_mode() {
        return Sim_CPU_Mode_User;
    }

    int riscv_cpu::enable_processor() {
        SIM_LOG_INFO(4, cobj_, 0, "Enabling processor (no-op)");
        if (is_enabled_) {
            SIM_LOG_INFO(4, cobj_, 0, "Processor already enabled");
            return 1;
        }
        is_enabled_ = true;
        return 0;
    }

    int riscv_cpu::disable_processor() {
        SIM_LOG_INFO(4, cobj_, 0, "Disabling processor (no-op)");
        if (!is_enabled_) {
            SIM_LOG_INFO(4, cobj_, 0, "Processor already disabled");
            return 1;
        }
        is_enabled_ = false;
        return 0;
    }

    int riscv_cpu::get_enabled() {
        return is_enabled_ ? 1 : 0;
    }

    cpu_endian_t riscv_cpu::get_endian() {
        return Sim_Endian_Little;
    }

    conf_object_t * riscv_cpu::get_physical_memory() {
        return phys_mem_;
    }

    int riscv_cpu::get_logical_address_width() {
        return RAM_ADDR_WIDTH;
    }

    int riscv_cpu::get_physical_address_width() {
        return RAM_ADDR_WIDTH;
    }

    const char *riscv_cpu::architecture() {
        return "riscv";
    }
} /* ! kz::riscv::core ! */
