#include "fde_fetch.hpp"
#include "fde_core.hpp"
#include "fde_dbg.hpp"

#ifndef __SYNTHESIS__
#ifdef DBG_FDE_IP
#include <stdio.h>
#endif /* !DBG_FDE_IP! */
#endif /* !__SYNTHESIS__! */

void fetch(addr_t pc, instr_t *p_ram, instr_t *p_instr) {
#pragma HLS INLINE off
    *p_instr = p_ram[pc];

#ifndef __SYNTHESIS__
#ifdef DBG_FDE_IP
    printf("[DBG] %04d: %08x\n", (int)(pc << 2), *p_instr);
#endif /* !DBG_FDE_IP! */
#endif /* !__SYNTHESIS__! */
}
