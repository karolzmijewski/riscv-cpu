#ifndef FDE_DECODE_H
#define FDE_DECODE_H

#include "fde_core.hpp"

/**
 * Decode the given instruction into its components and immediate value.
 * The function extracts fields such as opcode, destination register (rd),
 * source registers (rs1, rs2), function codes (func3, func7),
 * and determines the instruction type. It also decodes the immediate value
 * based on the instruction type.
 * M/O - Mandatory/Optional, In/Out - Input/Output. 
 * @param instr [M][In] The instruction to be decoded.
 * @param p_dec_instr [M][Out] Pointer to store the decoded instruction components.
 */
void decode(instr_t instr, dec_instr_t *p_dec_instr);

#endif /* !FDE_DECODE_H! */