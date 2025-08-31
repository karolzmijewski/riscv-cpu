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
        case U_TYPE: printf("U_TYPE"); break;
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
        default: printf("UNKNOWN"); break;
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

void print_reg_name(reg_nr_t reg_nr) {
#ifdef SYMB_REG
    switch(reg_nr) {
        case 0: printf("zero"); break;
        case 1: printf("ra"); break;
        case 2: printf("sp"); break;
        case 3: printf("gp"); break;
        case 4: printf("tp"); break;
        case 5: printf("t0"); break;
        case 6: printf("t1"); break;
        case 7: printf("t2"); break;
        case 8: printf("s0"); break;
        case 9: printf("s1"); break;
        case 10: printf("a0"); break;
        case 11: printf("a1"); break;
        case 12: printf("a2"); break;
        case 13: printf("a3"); break;
        case 14: printf("a4"); break;
        case 15: printf("a5"); break;
        case 16: printf("a6"); break;
        case 17: printf("a7"); break;
        case 18: printf("s2"); break;
        case 19: printf("s3"); break;
        case 20: printf("s4"); break;
        case 21: printf("s5"); break;
        case 22: printf("s6"); break;
        case 23: printf("s7"); break;
        case 24: printf("s8"); break;
        case 25: printf("s9"); break;
        case 26: printf("s10"); break;
        case 27: printf("s11"); break;
        case 28: printf("t3"); break;
        case 29: printf("t4"); break;
        case 30: printf("t5"); break;
        case 31: printf("t6"); break;
        default: printf("UNKNOWN"); break;
    }
#else
    printf("x%d", (unsigned int)reg_nr);
#endif /* SYMB_REG */
}

void print_operation_name(opcode_t opcode, dec_instr_t dec_instr) {
    switch (opcode) {
        case LOAD:
            switch (dec_instr.func3) {
                case 0b000: printf("LB"); break;
                case 0b001: printf("LH"); break;
                case 0b010: printf("LW"); break;
                case 0b011: printf("LD"); break;
                case 0b100: printf("LBU"); break;
                case 0b101: printf("LHU"); break;
                case 0b110: printf("LWU"); break;
                default: printf("UNKNOWN"); break;
            }
            break;
        case STORE:
            switch (dec_instr.func3) {
                case 0b000: printf("SB"); break;
                case 0b001: printf("SH"); break;
                case 0b010: printf("SW"); break;
                case 0b011: printf("SD"); break;
                default: printf("UNKNOWN"); break;
            }
            break;
        case OP_IMM:
            switch (dec_instr.func3) {
                case 0b000: {
                    if (dec_instr.rs1 == 0) {
                        printf("LI");
                    } else {
                        printf("ADDI");
                    }
                    break;
                }
                case 0b010: printf("SLTI"); break;
                case 0b011: printf("SLTIU"); break;
                case 0b100: printf("XORI"); break;
                case 0b110: printf("ORI"); break;
                case 0b111: printf("ANDI"); break;
                case 0b001:
                    if (dec_instr.func7 == 0b0000000) {
                        printf("SLLI");
                    } else {
                        printf("UNKNOWN");
                    }
                    break;
                case 0b101:
                    if (dec_instr.func7 == 0b0000000) {
                        printf("SRLI");
                    } else if (dec_instr.func7 == 0b0100000) {
                        printf("SRAI");
                    } else {
                        printf("UNKNOWN");
                    }
                    break;
                default: printf("UNKNOWN"); break;
            }
            break;
        case OP:
            switch (dec_instr.func3) {
                case 0b000:
                    if (dec_instr.func7 == 0b0000000) {
                        printf("ADD");
                    } else if (dec_instr.func7 == 0b0100000) {
                        printf("SUB");
                    } else {
                        printf("UNKNOWN");
                    }
                    break;
                case 0b001:
                    if (dec_instr.func7 == 0b0000000) {
                        printf("SLL");
                    } else {
                        printf("UNKNOWN");
                    }
                    break;
                case 0b010:
                    if (dec_instr.func7 == 0b0000000) {
                        printf("SLT");
                    } else {
                        printf("UNKNOWN");
                    } break;
                case 0b011:
                    if (dec_instr.func7 == 0b0000000) {
                        printf("SLTU");
                    } else {
                        printf("UNKNOWN");
                    } break;
                case 0b100:
                    if (dec_instr.func7 == 0b0000000) {
                        printf("XOR");
                    } else {
                        printf("UNKNOWN");
                    } break;
                case 0b101:
                    if (dec_instr.func7 == 0b0000000) {
                        printf("SRL");
                    } else if (dec_instr.func7 == 0b0100000) {
                        printf("SRA");
                    } else {
                        printf("UNKNOWN");
                    } break;
                case 0b110:
                    if (dec_instr.func7 == 0b0000000) {
                        printf("OR");
                    } else {
                        printf("UNKNOWN");
                    } break;
                case 0b111:
                    if (dec_instr.func7 == 0b0000000) {
                        printf("AND");
                    } else {
                        printf("UNKNOWN");
                    } break;
                default: printf("UNKNOWN"); break;
            }
            break;
        case BRANCH:
            switch (dec_instr.func3) {
                case 0b000: printf("BEQ"); break;
                case 0b001: printf("BNE"); break;
                case 0b010:
                case 0b011: break; // reserved
                case 0b100: printf("BLT"); break;
                case 0b101: printf("BGE"); break;
                case 0b110: printf("BLTU"); break;
                case 0b111: printf("BGEU"); break;
                default: printf("UNKNOWN"); break;
            }
            break;
        case JAL: {
            if (dec_instr.rd == 0) {
                printf("J");
            } else {
                printf("JAL");
            }
        } break;
        case JALR: {
            if (dec_instr.rd == 0 && dec_instr.rs1 == RA) {
                printf("RET");
            } else if (dec_instr.rd == 0) {
                printf("JR");
            } else {
                printf("JALR");
            }
        } break;
        case LUI: printf("LUI"); break;
        case AUIPC: printf("AUIPC"); break;
        default: printf("UNKNOWN"); break;
    }
}

void print_reg_file(int *p_reg_file) {
    for (unsigned int i = 0; i < REGISTER_NR; i++) {
        print_reg_name(i);
#ifdef SYMB_REG
        if (i != 26 && i != 27) { // a0, a1
            printf(" ");
        }
#else
        if (i < 10) {
            printf(" ");
        }
#endif /* SYMB_REG */
        printf("= %016d (0x%08x)\n", p_reg_file[i], (unsigned int)p_reg_file[i]);
    }
}

#endif /* !__SYNTHESIS__! */