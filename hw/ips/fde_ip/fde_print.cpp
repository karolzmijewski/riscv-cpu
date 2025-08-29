#ifndef __SYNTHESIS__

#include <stdio.h>
#include "fde_core.hpp"
#include "fde_type.hpp"
#include "fde_opcode.hpp"
#include "fde_print.hpp"

static void print_type(op_type_t type) {
    switch(type) {
        case UNDEF_TYPE: printf("UNDEF_TYPE"); break;
        case R_TYPE: printf("R_TYPE"); break;
        case I_TYPE: printf("I_TYPE"); break;
        case S_TYPE: printf("S_TYPE"); break;
        case B_TYPE: printf("B_TYPE"); break;
        case J_TYPE: printf("J_TYPE"); break;
        case OTHER_TYPE: printf("OTHER_TYPE"); break;
    }
}

void print_opcode(opcode_t opcode) {
    switch (opcode) {
        case LOAD: printf("LOAD"); break;
        case LOAD_FP: printf("LOAD_FP"); break;
        case CUSTOM_0: printf("CUSTOM_0"); break;
        case MISC_MEM: printf("MISC_MEM"); break;
        case OP_IMM: printf("OP_IMM"); break;
        case AUIPC: printf("AUIPC"); break;
        case OP_IMM_32: printf("OP_IMM_32"); break;
        case RV48_0: printf("RV48_0"); break;
        case STORE: printf("STORE"); break;
        case STORE_FP: printf("STORE_FP"); break;
        case CUSTOM_1: printf("CUSTOM_1"); break;
        case AMO: printf("AMO"); break;
        case OP: printf("OP"); break;
        case LUI: printf("LUI"); break;
        case OP_32: printf("OP_32"); break;
        case RV_64: printf("RV_64"); break;
        case MADD: printf("MADD"); break;
        case MSUB: printf("MSUB"); break;
        case NMSUB: printf("NMSUB"); break;
        case NMADD: printf("NMADD"); break;
        case OP_FP: printf("OP_FP"); break;
        case RSVD_0: printf("RSVD_0"); break;
        case CUSTOM_2_RV128: printf("CUSTOM_2_RV128"); break;
        case RV48_1: printf("RV48_1"); break;
        case BRANCH: printf("BRANCH"); break;
        case JALR: printf("JALR"); break;
        case RSVD_1: printf("RSVD_1"); break;
        case JAL: printf("JAL"); break;
        case SYSTEM: printf("SYSTEM"); break;
        case RSVD_2: printf("RSVD_2"); break;
        case CUSTOM_3_RV128: printf("CUSTOM_3_RV128"); break;
        case RV_80: printf("RV_80"); break;
    }
}

void print_dec_instr(dec_instr_t dec_instr) {
    printf("opcode:\t0x%02x\n", (unsigned int)dec_instr.opcode);
    printf("rd:\t\t0x%02x\n", (unsigned int)dec_instr.rd);
    printf("func3:\t0x%01x\n", (unsigned int)dec_instr.func3);
    printf("rs1:\t0x%02x\n", (unsigned int)dec_instr.rs1);
    printf("rs2:\t0x%02x\n", (unsigned int)dec_instr.rs2);
    printf("func7:\t0x%02x\n", (unsigned int)dec_instr.func7);
    printf("type:\t");
    print_type(dec_instr.type);
    printf("\n");
}

#endif /* !__SYNTHESIS__! */