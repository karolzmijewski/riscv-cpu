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

#ifndef FDE_EMUL_HPP
#define FDE_EMUL_HPP

#include "fde_core.hpp"

/**
 * Emulate is a debugging function that simulates the effect of executing
 * a decoded instruction on the register file. It is intended for use
 * in non-synthesis environments (e.g., software simulation) to help
 * visualize and verify the changes made to the register file by each
 * instruction. It gives similar output to the SPIKE RISC-V ISA simulator.
 * M/O - Mandatory/Optional, In/Out - Input/Output.
 * @param p_reg_file [M][Out] Pointer to the register file array.
 * @param dec_instr [M][In] The decoded instruction to be "emulated".
 * @param next_pc [M][In] The next program counter (PC) value after execution.
 */
void emulate(int *p_reg_file, dec_instr_t dec_instr, addr_t next_pc);

#endif /* !FDE_EMUL_HPP! */