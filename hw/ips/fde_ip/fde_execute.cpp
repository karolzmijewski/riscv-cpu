#include "fde_core.hpp"
#include "fde_type.hpp"
#include "fde_execute.hpp"

static addr_t get_next_pc(dec_instr_t dec_instr, addr_t pc) {
// the pc was defined as word pointer, so +1 will get
// the net word (next instruction in memory)
#pragma HLS INLINE
    switch(dec_instr.type) {
        case R_TYPE: return (addr_t)(pc + 1);
        case I_TYPE: return (addr_t)(pc + 1);
        case S_TYPE: return (addr_t)(pc + 1);
        case B_TYPE: return (addr_t)(pc + 1);
        case U_TYPE: return (addr_t)(pc + 1);
        case J_TYPE: return (addr_t)(pc + (dec_instr.imm >> 1));
        case OTHER_TYPE: return (addr_t)(pc + 1);
        default: return (addr_t)(pc + 1);
    }
}

void execute(dec_instr_t dec_instr, addr_t pc, addr_t *p_next_pc) {
#pragma HLS INLINE off
    *p_next_pc = get_next_pc(dec_instr, pc);
}