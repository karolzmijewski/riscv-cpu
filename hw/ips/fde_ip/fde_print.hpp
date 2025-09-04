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

#ifndef FDE_PRINT_HPP
#define FDE_PRINT_HPP

#include "fde_core.hpp"

#define SYMB_REG

/**
 * Print the opcode name corresponding to the given opcode value.
 * M/O - Mandatory/Optional, In/Out - Input/Output.
 * @param opcode [M][In] The opcode value to be printed.
 */
void print_opcode(opcode_t opcode);
/**
 * Print the register name corresponding to the given register number.
 * M/O - Mandatory/Optional, In/Out - Input/Output.
 * @param reg_nr [M][In] The register number to be printed.
 */
void print_reg_name(reg_nr_t reg_nr);
/**
 * Print the entire register file.
 * M/O - Mandatory/Optional, In/Out - Input/Output.
 * @param p_reg_file [M][In] Pointer to the register file array.
 */
void print_reg_file(int *p_reg_file);
/**
 * Print the decoded instruction components in a human-readable format.
 * M/O - Mandatory/Optional, In/Out - Input/Output.
 * @param dec_instr [M][In] The decoded instruction to be printed.
 */
void print_dec_instr(dec_instr_t dec_instr);
/**
 * Print the operation name (assembler) corresponding to the given opcode
 * and decoded instruction.
 * M/O - Mandatory/Optional, In/Out - Input/Output.
 * @param opcode [M][In] The opcode value.
 * @param dec_instr [M][In] The decoded instruction to determine the operation name.
 */
void print_operation_name(opcode_t opcode, dec_instr_t dec_instr);

#endif /* !FDE_PRINT_HPP! */