#ifndef FDE_EXECUTE_HPP
#define FDE_EXECUTE_HPP

#include "fde_core.hpp"

/**
 * Execute the decoded instruction by reading the source registers,
 * computing the result, writing back to the destination register,
 * and updating the program counter (PC) for the next instruction.
 * The function uses helper functions to read from and write to
 * the register file, compute the result based on the instruction type,
 * and determine the next PC value.
 * M/O - Mandatory/Optional, In/Out - Input/Output.
 * @param dec_instr [M][In] The decoded instruction to be executed.
 * @param p_reg_file [M][Out] Pointer to the register file array.
 * @param pc [M][In] The current program counter (PC) value.
 * @param p_next_pc [M][Out] Pointer to store the next PC value after execution.
 */
void execute(dec_instr_t dec_instr, int *p_reg_file, addr_t pc, addr_t *p_next_pc);

#endif /* !FDE_EXECUTE_HPP! */