#ifndef FDE_EMUL_HPP
#define FDE_EMUL_HPP

#include "fde_core.hpp"

/**
 * Emulate is a debugging function that simulates the effect of executing
 * a decoded instruction on the register file. It is intended for use
 * in non-synthesis environments (e.g., software simulation) to help
 * visualize and verify the changes made to the register file by each
 * instruction. It gives similar output to the SPIKE RISC-V ISA simulator.
 * M/O - Mandatory/Optional, In/Out - Input/Output.
 * @param p_reg_file [M][Out] Pointer to the register file array.
 * @param dec_instr [M][In] The decoded instruction to be "emulated".
 * @param next_pc [M][In] The next program counter (PC) value after execution.
 */
void emulate(int *p_reg_file, dec_instr_t dec_instr, addr_t next_pc);

#endif /* !FDE_EMUL_HPP! */