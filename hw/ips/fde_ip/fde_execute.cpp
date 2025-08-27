#include "fde_core.hpp"
#include "fde_execute.hpp"

static addr_t get_next_pc(addr_t pc) {
#pragma HLS INLINE
    return (addr_t)(pc + 1);
}

void execute(addr_t pc, addr_t *p_next_pc) {
    #pragma HLS INLINE off
    *p_next_pc = get_next_pc(pc);
}