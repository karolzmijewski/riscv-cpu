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

namespace kz::riscv::core {
    void riscv_cpu::set_program_counter(logical_address_t pc) {
        pc_ = static_cast<uint32_t>(pc);
    }

    logical_address_t riscv_cpu::get_program_counter() {
        return static_cast<logical_address_t>(pc_);
    }

    tuple_int_string_t riscv_cpu::disassemble(generic_address_t address, attr_value_t instruction_data, int sub_operation) {
        // For simplicity, we return a fixed instruction "nop" (no operation)
        if (!SIM_attr_is_data(instruction_data))
            return {0, const_cast<char *>("")};

        // unsigned size = SIM_attr_data_size(instruction_data);
        // const uint8 *bytes = SIM_attr_data(instruction_data);

        return {4, const_cast<char *>("nop")};
    }

    physical_block_t riscv_cpu::logical_to_physical(logical_address_t address, access_t access_type) {
        physical_block_t block = {};
        block.valid = 0;
        block.address = 0;
        block.block_start = 0;
        block.block_end = 0;
        return block;
    }

    processor_mode_t riscv_cpu::get_processor_mode() {
        return Sim_CPU_Mode_User;
    }

    int riscv_cpu::enable_processor() {
        return 1;
    }

    int riscv_cpu::disable_processor() {
        return 1;
    }

    int riscv_cpu::get_enabled() {
        return 0;
    }

    cpu_endian_t riscv_cpu::get_endian() {
        return Sim_Endian_Little;
    }

    conf_object_t * riscv_cpu::get_physical_memory() {
        return nullptr;
    }

    int riscv_cpu::get_logical_address_width() {
        return 32;
    }

    int riscv_cpu::get_physical_address_width() {
        return 32;
    }

    const char *riscv_cpu::architecture() {
        return "riscv";
    }
}
