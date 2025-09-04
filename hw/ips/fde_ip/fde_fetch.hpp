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

#ifndef FDE_FETCH_HPP
#define FDE_FETCH_HPP

#include "fde_core.hpp"

/**
 * Fetch the instruction from memory at the given program counter (PC) address.
 * The function reads the instruction from the provided RAM array and
 * stores it in the location pointed to by p_instr.
 * M/O - Mandatory/Optional, In/Out - Input/Output.
 * @param pc [M][In] The program counter (PC) address to fetch the instruction from.
 * @param p_ram [M][In] Pointer to the RAM array containing instructions.
 * @param p_instr [M][Out] Pointer to store the fetched instruction.
 */
void fetch(addr_t pc, instr_t *p_ram, instr_t *p_instr);

#endif /* !FDE_FETCH_HPP! */