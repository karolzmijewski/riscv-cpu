#ifndef FDE_PRINT_HPP
#define FDE_PRINT_HPP

#include "fde_core.hpp"

#define SYMB_REG

void print_opcode(opcode_t opcode);
void print_reg_name(reg_nr_t reg_nr);
void print_reg_file(int *p_reg_file);
void print_dec_instr(dec_instr_t dec_instr);
void print_operation_name(opcode_t opcode, dec_instr_t dec_instr);

#endif /* !FDE_PRINT_HPP! */