#ifndef FDE_EMUL_HPP
#define FDE_EMUL_HPP

#include "fde_core.hpp"

void emulate(int *p_reg_file, dec_instr_t dec_instr, addr_t next_pc);

#endif /* !FDE_EMUL_HPP! */