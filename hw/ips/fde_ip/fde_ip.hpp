#ifndef FDE_IP_HPP
#define FDE_IP_HPP

#include "fde_core.hpp"

/**
 * The top-level function for the Fetch-Decode-Execute (FDE) IP core.
 * It orchestrates the fetching of instructions from memory, decoding them,
 * executing them, and updating the program counter (PC) and instruction
 * counter. The function interfaces with an external RAM for instruction
 * storage and maintains a register file for the CPU.
 * M/O - Mandatory/Optional, In/Out - Input/Output.
 * @param start_pc [M][In] The starting program counter (PC) value.
 * @param ram [M][In] The instruction memory (RAM) array.
 * @param p_instr_counter [M][Out] Pointer to store the instruction execution count.
 */
void fde_ip(unsigned int start_pc, unsigned int ram[RAM_SIZE], unsigned int *p_instr_counter);

#endif /* !FDE_IP_HPP! */