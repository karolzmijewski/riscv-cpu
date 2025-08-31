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
