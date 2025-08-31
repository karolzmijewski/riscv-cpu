#ifndef __SYNTHESIS__

#include <stdio.h>
#include "fde_emul.hpp"
#include "fde_core.hpp"
#include "fde_type.hpp"
#include "fde_opcode.hpp"
#include "fde_print.hpp"


static void emulate_operation(int *p_reg_file, dec_instr_t dec_instr) {
    if (dec_instr.rd != 0) {
        print_reg_name(dec_instr.rd);
#ifdef SYMB_REG
        if (dec_instr.rd != 26 && dec_instr.rd != 27) { // a0, a1
            printf(" ");
        }
#else
        if (dec_instr.rd < 10) {
            printf(" ");
        }
#endif /* SYMB_REG */
        printf(" = ");
        if ((dec_instr.opcode == OP_IMM && dec_instr.func3 == 0b011) || // SLTIU
            (dec_instr.opcode == OP && dec_instr.func3 == 0b011)) { // SLTU
            printf("%016u (0x%08x)", (unsigned int)p_reg_file[dec_instr.rd], (unsigned int)p_reg_file[dec_instr.rd]);
        } else {
            printf("%016d (0x%08x)", p_reg_file[dec_instr.rd], p_reg_file[dec_instr.rd]);
        }
    }
}

void emulate(int *p_reg_file, dec_instr_t dec_instr, addr_t next_pc) {
    printf("\t");
    switch(dec_instr.type) {
        case R_TYPE: emulate_operation(p_reg_file, dec_instr); break;
        case I_TYPE:
            if (dec_instr.opcode == JALR) {
                printf("pc = %016d (0x%08x)", (int)next_pc * sizeof(instr_t), (unsigned int)next_pc * sizeof(instr_t));
                if (dec_instr.rd != 0) {
                    printf("\n\t");
                }
                emulate_operation(p_reg_file, dec_instr);
            } else if (dec_instr.opcode == OP_IMM) {
                emulate_operation(p_reg_file, dec_instr);
            } break;
        case S_TYPE: break;
        case B_TYPE:
            printf("pc = %016d (0x%08x)", (int)next_pc * sizeof(instr_t), (unsigned int)next_pc * sizeof(instr_t));
            break;
        case U_TYPE:
            emulate_operation(p_reg_file, dec_instr);
            break;
        case J_TYPE:
            printf("pc = %016d (0x%08x)", (int)next_pc * sizeof(instr_t), (unsigned int)next_pc * sizeof(instr_t));
            if (dec_instr.rd != 0) {
                printf("\n\t");
            }
            emulate_operation(p_reg_file, dec_instr);
            break;
        default: break;
    }
    printf("\n");
}

#endif /* !__SYNTHESIS__! */
