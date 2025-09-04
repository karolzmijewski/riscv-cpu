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

#ifndef FDE_IP_HPP
#define FDE_IP_HPP

#include "fde_core.hpp"

/**
 * The top-level function for the Fetch-Decode-Execute (FDE) IP core.
 * It orchestrates the fetching of instructions from memory, decoding them,
 * executing them, and updating the program counter (PC) and instruction
 * counter. The function interfaces with an external RAM for instruction
 * storage and maintains a register file for the CPU.
 * M/O - Mandatory/Optional, In/Out - Input/Output.
 * @param start_pc [M][In] The starting program counter (PC) value.
 * @param ram [M][In] The instruction memory (RAM) array.
 * @param p_instr_counter [M][Out] Pointer to store the instruction execution count.
 */
void fde_ip(unsigned int start_pc, unsigned int ram[RAM_SIZE], unsigned int *p_instr_counter);

#endif /* !FDE_IP_HPP! */