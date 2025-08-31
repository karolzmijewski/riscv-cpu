#include "fde_core.hpp"
#include "fde_opcode.hpp" 
#include "fde_ip.hpp"
#include "fde_fetch.hpp"
#include "fde_decode.hpp"
#include "fde_execute.hpp"
#include "fde_dbg.hpp"
#include "fde_disasm.hpp"

#ifndef __SYNTHESIS__
#ifdef DBG_DISASSEMBLE
#include "fde_disasm.hpp"
#endif /* !DBG_DISASSEMBLE! */
#ifdef DBG_REG_FILE
#include <stdio.h>
#include "fde_print.hpp"
#endif /* !DBG_REG_FILE! */
#endif /* !__SYNTHESIS__! */

static void update_status(instr_t instr, addr_t pc, bit_t *p_is_running) {
#pragma HLS INLINE off
    *p_is_running = (instr != RET || pc != 0); // infinite loop at address 0
}

static void update_statistics(unsigned int *p_instr_counter) {
#pragma HLS INLINE off
    (*p_instr_counter)++;
}

void fde_ip(unsigned int start_pc, unsigned int ram[RAM_SIZE], unsigned int *p_instr_counter) {
#pragma HLS INTERFACE s_axilite port=start_pc
#pragma HLS INTERFACE s_axilite port=ram
#pragma HLS INTERFACE s_axilite port=p_instr_counter
#pragma HLS INTERFACE s_axilite port=return
    bit_t is_running;
    addr_t pc = start_pc;
    instr_t instr;
    dec_instr_t dec_instr;
    int reg_file[REGISTER_NR];
    unsigned int instr_counter = 0;

    for (int i = 0; i < REGISTER_NR; i++) {
#pragma HLS UNROLL
        reg_file[i] = 0;
    }

    do {
// HLS PIPLINE pragma is used to keep the loop running at the specified
// initiation interval (II). The II defines the number of clock cycles
// between the start of consecutive loop iterations. For example, an II of 1
// means that a new iteration starts every clock cycle, while an II of 2
// means that a new iteration starts every two clock cycles.
// The goal of pipelining is to improve performance by overlapping the
// execution of multiple iterations, similar to an assembly line in a factory.
#pragma HLS PIPELINE II=6
        fetch(pc, ram, &instr);
        decode(instr, &dec_instr);

#ifndef __SYNTHESIS__
#ifdef DBG_DISASSEMBLE
        disasm(pc, instr, dec_instr);
#endif /* !DBG_DISASSEMBLE! */
#endif /* !__SYNTHESIS__! */

        execute(dec_instr, reg_file, pc, &pc);
        update_statistics(&instr_counter);
        update_status(instr, pc, &is_running);
    } while(is_running);

    *p_instr_counter = instr_counter;

#ifndef __SYNTHESIS__
#ifdef DBG_REG_FILE
    printf("REGISTERS DUMP:\n");
    print_reg_file(reg_file);
#endif /* DBG_REG_FILE */
#endif /* !__SYNTHESIS__! */
}