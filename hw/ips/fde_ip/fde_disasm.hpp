#ifndef FDE_DISASM
#define FDE_DISASM

#include "fde_core.hpp"

/**
 * Disassemble the given instruction and print its assembly representation.
 * The function takes the program counter (PC), the raw instruction,
 * and the decoded instruction components as input. It formats and outputs
 * the assembly instruction to the console or a log, depending on the
 * environment (synthesis or simulation).
 * M/O - Mandatory/Optional, In/Out - Input/Output.
 * @param pc [M][In] The program counter (PC) value of the instruction.
 * @param instr [M][In] The raw instruction just to verify if the instruction isn't NOP.
 * @param dec_instr [M][In] The decoded instruction to be disassembled.
 */
void disasm(addr_t pc, instr_t instr, dec_instr_t dec_instr);

#endif /* !FDE_DISASM! */