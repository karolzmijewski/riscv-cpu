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

#ifndef FDE_OP_FORMAT_H
#define FDE_OP_FORMAT_H

#include "fde_core.hpp"

#define UNDEF_TYPE 0
#define R_TYPE 1
#define I_TYPE 2
#define S_TYPE 3
#define B_TYPE 4
#define U_TYPE 5
#define J_TYPE 6
#define OTHER_TYPE 7

/**
 * It's a representation of main MUX (multiplexer 3*4 -> 3) logic,
 * used to decode opch, and call the appropriate sub-MUX representation
 * function to decode opcl and determine the operation type.
 * M/O - Mandatory/Optional, In/Out - Input/Output.
 * @param opcode [M][In] The opcode value to determine the operation type.
 * @return The operation type corresponding to the given opcode.
 */
op_type_t get_op_type(opcode_t opcode);

#endif /* !FDE_OP_FORMAT_H! */