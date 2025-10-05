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

#include "fde_fetch.hpp"
#include "fde_core.hpp"
#include "fde_dbg.hpp"

#ifndef __SYNTHESIS__
#ifdef DBG_FDE_IP
#include <stdio.h>
#endif /* !DBG_FDE_IP! */
#endif /* !__SYNTHESIS__! */

void fetch(addr_t pc, instr_t *p_ram, instr_t *p_instr) {
// The INLINE pragma instructs the synthesizer on how to handle this function.
// When set to "on," the synthesizer inlines the function, eliminating the overhead 
// of transmitting arguments before the call and returning results afterward.
// When set to "off," the function is treated as a separate hardware block 
// (a subcomponent within the parent component where it is invoked). 
// This can improve design readability — especially in analysis tools such as 
// the Schedule Viewer — since the function appears as a distinct component. 
// However, disabling inlining requires additional resources for argument 
// transmission and may lead to slower performance in some cases.
// Choosing between "on" and "off" depends on whether clarity of hierarchy 
// or performance optimization is the primary goal for the design.
#pragma HLS INLINE off

    *p_instr = p_ram[pc];

#ifndef __SYNTHESIS__
#ifdef DBG_FETCH
    printf("[fde_fetch.fetch] %04d: %08x\n", (int)(pc << 2), *p_instr);
#ifndef DBG_DISASSEMBLE
    printf("\n");
#endif /* !DBG_DISASSEMBLE! */
#endif /* !DBG_FETCH! */
#endif /* !__SYNTHESIS__! */
}
