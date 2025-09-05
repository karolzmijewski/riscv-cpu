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

#include <simics/c++/model-iface/processor-info.h>

namespace kz::riscv::cpu::iface {
    class proc_iface_impl: public simics::iface::ProcessorInfoV2Interface {
    public:
        /**
         * The disassemble function returns the disassemble string for an instruction at address
         * with opcode according to instruction_data. The instruction_data is an attr_value_t value
         * of data type with the bytes of the opcode. The bytes are in the same order as they are
         * stored in memory.
         * @param address The address of the instruction to disassemble.
         * @param instruction_data The data of the instruction to disassemble.
         * @param sub_operation Used to select which sub-operation to disassemble. The sub-operations
         *     start at zero, and a request for the entire unit including all sub-operations is encoded
         *     with sub-operation -1.
         */
        tuple_int_string_t disassemble(generic_address_t address, attr_value_t instruction_data, int sub_operation) override;
        /**
         * Function sets the program counter in the processor. The get_program_counter function returns
         * the current program counter.
         * @param pc The new value of the program counter.
         */
        void set_program_counter(logical_address_t pc) override;
        /**
         * Function returns the current value of the program counter.
         * @return The current value of the program counter.
         */
        logical_address_t get_program_counter() override;
        /**
         * Function translates a logical address to a physical address of the type defined by access_type.
         * The function returns a physical_block_t struct with valid bit and the address. The address is
         * valid when the valid bit is not 0. The logical_to_physical function also returns block_start
         * and block_end. The start and end of a block has the same logical to physical transformation as
         * the translated address. The range is inclusive, so block_end should be the address of the last
         * byte of the block.
         */
        physical_block_t logical_to_physical(logical_address_t address, access_t access_type) override;
        /**
         * Function to enable the processor. The functions return 0 if the processor changed from
         * enabled to disabled or from disabled to enabled, and 1 if the processor did not change state.
         * The current state is returned by the get_enabled function
         */
        int enable_processor() override;
        /**
         * Function to disable the processor. The functions return 0 if the processor changed from
         * enabled to disabled or from disabled to enabled, and 1 if the processor did not change state.
         * The current state is returned by the get_enabled function
         */
        int disable_processor() override;
        /**
         * Function returns 1 if the processor is enabled and 0 if it is disabled.
         */
        int get_enabled() override;
        /**
         * Function returns the endian of the processor. The return value is one of the cpu_endian_t
         * enum values.
         */
        cpu_endian_t get_endian() override;
        /**
         * Function returns the physical memory object of the processor. The physical memory object
         * is used by Simics to access the memory of the processor, for instance to set breakpoints
         * by the global break command, and to read and write physical memory through set, get,
         * load-binary, load-file, and the default implementation of disassemble.
         * @return The object returned implements the memory_space and breakpoint interfaces.
         *     The memory_space interface for the returned object is only be used in inquiry mode
         *     corresponding to actions by the simulator itself rather than by the simulated software.
         */
        conf_object_t *get_physical_memory() override;
        /**
         * Function returns the number of logical/virtual address bits of the processor.
         */
        int get_logical_address_width() override;
        /**
         * Function returns the number of physical address bits of the processor.
         */
        int get_physical_address_width() override;
        /**
         * Returns the architecture string of the processor.
         * The architecture string is a short string that identifies the architecture of the processor.
         * It is used by Simics to determine which binaries can run on the processor.
         * Examples of architecture strings are "x86", "arm", "mips", "riscv".
         */
        const char *architecture() override;
    };
} // namespace kz::riscv::cpu::iface