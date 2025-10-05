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

#ifndef __SYNTHESIS__

#include <stdio.h>
#include "fde_core.hpp"
#include "fde_opcode.hpp"
#include "fde_disasm.hpp"
#include "fde_print.hpp"
#include "fde_type.hpp"

void disasm(addr_t pc, instr_t instr, dec_instr_t dec_instr) {
    printf("[fde_disasm.disasm]:\n");
    switch (dec_instr.type) {
        case R_TYPE:
            print_operation_name(dec_instr.opcode, dec_instr);
            printf(" ");
            print_reg_name(dec_instr.rd);
            printf(", ");
            print_reg_name(dec_instr.rs1);
            printf(", ");
            print_reg_name(dec_instr.rs2);
            printf("\n");
            break;
        case I_TYPE:
            if (dec_instr.opcode == JALR) {
                print_operation_name(dec_instr.opcode, dec_instr);
                if (dec_instr.rd == 0) {
                    printf(" ");
                } else {
                    printf(" ");
                    if (dec_instr.rd != RA) {
                        print_reg_name(dec_instr.rd);
                        printf(", ");
                    }
                }
                if (dec_instr.imm == 0) {
                    print_reg_name(dec_instr.rs1);
                } else {
                    printf("%d(", (int)dec_instr.imm);
                    print_reg_name(dec_instr.rs1);
                    printf(")");
                }
            } else if (dec_instr.opcode == OP_IMM) {
                if (instr == NOP) {
                    printf("NOP");
                } else {
                    print_operation_name(dec_instr.opcode, dec_instr);
                    printf(" ");
                    print_reg_name(dec_instr.rd);
                    printf(", ");
                    if (dec_instr.func3 != 0b000 || dec_instr.rs1 != 0) {
                        print_reg_name(dec_instr.rs1);
                        printf(", ");
                    }
                    if (dec_instr.func3 != 0b001 && dec_instr.func3 != 0b101) {
                        printf("%d", (int)dec_instr.imm);
                    } else {
                        printf("%u", (unsigned int)dec_instr.rs2);
                    }
                }
            } else if (dec_instr.opcode == LOAD) {
                print_operation_name(dec_instr.opcode, dec_instr);
                printf(" ");
                print_reg_name(dec_instr.rd);
                printf(", %d(", (int)dec_instr.imm);
                print_reg_name(dec_instr.rs1);
                printf(")");
            }
            break;
        case S_TYPE:
            print_operation_name(dec_instr.opcode, dec_instr);
            printf(" ");
            print_reg_name(dec_instr.rs2);
            printf(", %d(", (int)dec_instr.imm);
            print_reg_name(dec_instr.rs1);
            printf(")\n");
            break;
        case B_TYPE:
            print_operation_name(dec_instr.opcode, dec_instr);
            printf(" ");
            print_reg_name(dec_instr.rs1);
            printf(", ");
            print_reg_name(dec_instr.rs2);
            printf(", ");
            printf("%d", ((int)pc + (int)(dec_instr.imm >> 1)) * sizeof(instr_t));
            break;
        case U_TYPE:
            print_operation_name(dec_instr.opcode, dec_instr);
            printf(" ");
            print_reg_name(dec_instr.rd);
            printf(", %d", (int)(dec_instr.imm << 12));
            break;
        case J_TYPE:
            print_operation_name(dec_instr.opcode, dec_instr);
            printf(" ");
            print_reg_name(dec_instr.rd);
            printf(", ");
            printf("%d", ((int)pc + (int)(dec_instr.imm >> 1)) * sizeof(instr_t));
            break;
    }
    printf("\n");
}

#endif /* !__SYNTHESIS__! */
