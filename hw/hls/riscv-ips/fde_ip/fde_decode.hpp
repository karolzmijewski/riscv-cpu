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

#ifndef FDE_DECODE_H
#define FDE_DECODE_H

#include "fde_core.hpp"

/**
 * Decode the given instruction into its components and immediate value.
 * The function extracts fields such as opcode, destination register (rd),
 * source registers (rs1, rs2), function codes (func3, func7),
 * and determines the instruction type. It also decodes the immediate value
 * based on the instruction type.
 * M/O - Mandatory/Optional, In/Out - Input/Output.
 * @param instr [M][In] The instruction to be decoded.
 * @param p_dec_instr [M][Out] Pointer to store the decoded instruction components.
 */
void decode(instr_t instr, dec_instr_t *p_dec_instr);

#endif /* !FDE_DECODE_H! */