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

#ifndef FDE_DISASM
#define FDE_DISASM

#include "fde_core.hpp"

/**
 * Disassemble the given instruction and print its assembly representation.
 * The function takes the program counter (PC), the raw instruction,
 * and the decoded instruction components as input. It formats and outputs
 * the assembly instruction to the console or a log, depending on the
 * environment (synthesis or simulation).
 * M/O - Mandatory/Optional, In/Out - Input/Output.
 * @param pc [M][In] The program counter (PC) value of the instruction.
 * @param instr [M][In] The raw instruction just to verify if the instruction isn't NOP.
 * @param dec_instr [M][In] The decoded instruction to be disassembled.
 */
void disasm(addr_t pc, instr_t instr, dec_instr_t dec_instr);

#endif /* !FDE_DISASM! */