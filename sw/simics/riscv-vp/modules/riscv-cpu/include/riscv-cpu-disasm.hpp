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

#include <string>
#include "riscv-cpu-types.hpp"

namespace kz::riscv::core {
    class RiscvCpuDisasm {
    private:
        using op_type_t = kz::riscv::types::op_type_t;
        using opcode_t = kz::riscv::types::opcode_t;
        using reg_nr_t = kz::riscv::types::reg_nr_t;
        using dec_instr_t = kz::riscv::types::dec_instr_t;
        using operation_type_t = kz::riscv::types::operation_type_t;
        using operation_code_t = kz::riscv::types::operation_code_t;
        using addr_t = kz::riscv::types::addr_t;
    public:
        /**
         * Get a string representation of the operation type.
         * M/O - Mandatory/Optional, In/Out - Input/Output.
         * @param type [M][In] The operation type to be converted to a string.
         * @return A string representation of the operation type.
         */
        static std::string get_type(op_type_t type);
        /**
         * Get a string representation of the opcode.
         * M/O - Mandatory/Optional, In/Out - Input/Output.
         * @param opcode [M][In] The opcode to be converted to a string.
         * @return A string representation of the opcode.
         */
        static std::string get_opcode(opcode_t opcode);
        /**
         * Get the name of the register corresponding to the given register number.
         * M/O - Mandatory/Optional, In/Out - Input/Output.
         * @param reg_nr [M][In] The register number (0-31 for x0-x31).
         * @param symb [O][In] If true, return symbolic names (e.g., "zero", "ra").
         *                     If false, return numeric names (e.g., "x0", "x1").
         *                     Default is true.
         * @return The name of the register as a string.
         */
        static std::string get_reg_name(reg_nr_t reg_nr, bool symb=true);
        /**
         * Get the mnemonic for the given opcode and decoded instruction.
         * M/O - Mandatory/Optional, In/Out - Input/Output.
         * @param opcode [M][In] The opcode of the instruction.
         * @param dec_instr [M][In] The decoded instruction structure.
         * @return The mnemonic of the instruction as a string.
         */
        static std::string get_mnemonic(opcode_t opcode, dec_instr_t dec_instr);
        /**
         * Disassemble the given decoded instruction into a human-readable assembly
         * instruction string.
         * M/O - Mandatory/Optional, In/Out - Input/Output.
         * @param pc [M][In] The program counter (address) of the instruction.
         * @param dec_instr [M][In] The decoded instruction structure.
         * @return A string representing the disassembled instruction.
         */
        static std::string disasm(addr_t pc, dec_instr_t dec_instr);
    };
} /* ! kz::riscv::core ! */
