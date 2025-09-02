#ifndef FDE_FETCH_HPP
#define FDE_FETCH_HPP

#include "fde_core.hpp"

/**
 * Fetch the instruction from memory at the given program counter (PC) address.
 * The function reads the instruction from the provided RAM array and
 * stores it in the location pointed to by p_instr.
 * M/O - Mandatory/Optional, In/Out - Input/Output.
 * @param pc [M][In] The program counter (PC) address to fetch the instruction from.
 * @param p_ram [M][In] Pointer to the RAM array containing instructions.
 * @param p_instr [M][Out] Pointer to store the fetched instruction.
 */
void fetch(addr_t pc, instr_t *p_ram, instr_t *p_instr);

#endif /* !FDE_FETCH_HPP! */