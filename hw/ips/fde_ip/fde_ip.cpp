#include "fde_core.hpp"
#include "fde_ip.hpp"
#include "fde_fetch.hpp"
#include "fde_execute.hpp"

static void update_status(instr_t instr, bit_t *p_is_running) {
#pragma HLS INLINE off
    *p_is_running = (instr != RET_OPCODE);
}

void fde_ip(unsigned int start_pc, unsigned int ram[RAM_SIZE]) {
#pragma HLS INTERFACE s_axilite port=start_pc
#pragma HLS INTERFACE s_axilite port=ram
#pragma HLS INTERFACE s_axilite port=return
    instr_t instr;
    bit_t is_running;
    addr_t pc = start_pc;
    do {
#pragma HLS PIPELINE off
        fetch(pc, ram, &instr);
        execute(pc, &pc);
        update_status(instr, &is_running);
    } while(is_running);
}