#ifndef FDE_EXECUTE_HPP
#define FDE_EXECUTE_HPP

#include "fde_core.hpp"

void execute(dec_instr_t dec_instr, int *p_reg_file, addr_t pc, addr_t *p_next_pc);

#endif /* !FDE_EXECUTE_HPP! */