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

#ifndef FDE_EXECUTE_HPP
#define FDE_EXECUTE_HPP

#include "fde_core.hpp"

/**
 * Execute the decoded instruction by reading the source registers,
 * computing the result, writing back to the destination register,
 * and updating the program counter (PC) for the next instruction.
 * The function uses helper functions to read from and write to
 * the register file, compute the result based on the instruction type,
 * and determine the next PC value.
 * M/O - Mandatory/Optional, In/Out - Input/Output.
 * @param dec_instr [M][In] The decoded instruction to be executed.
 * @param p_reg_file [M][Out] Pointer to the register file array.
 * @param pc [M][In] The current program counter (PC) value.
 * @param p_next_pc [M][Out] Pointer to store the next PC value after execution.
 */
void execute(dec_instr_t dec_instr, int *p_reg_file, addr_t pc, addr_t *p_next_pc);

#endif /* !FDE_EXECUTE_HPP! */