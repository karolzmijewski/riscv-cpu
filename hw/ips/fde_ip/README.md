# FDE IP
FDE stands for Fetch–Decode–Execute Intellectual Property (IP).
This block defines the main execution cycle of the **RISC-V CPU**. It communicates using an **AXI interface**.

## Fetch
The fetch component is responsible for reading instructions from memory. The memory stores instruction words,
and the fetch process continues as long as a given condition is met. In this implementation, the stopping
condition is the `RET` pseudo-instruction.

Execution begins at an initial program counter (`PC`) address. During each **IP cycle**, the PC is incremented by the instruction size — 32 bits (4 bytes, or 1 word) — ensuring compliance with the **RV32I** base ISA (excluding
the optional **RVC** compressed instruction extension).

## Decode
The decode component interprets the binary instruction word fetched from memory. It extracts fields such as the
opcode, source and destination registers, function codes, and immediate values. These fields are then used to
determine the required operation and operands.

## Execute
The execute component performs the operation defined by the decoded instruction. This may involve:
- Arithmetic or logical operations in the **ALU**,
- Memory access (load/store) operations, or
- Control flow changes (branches, jumps, returns).

Results from execution are typically written back to registers or memory, completing one cycle of the
fetch – decode – execute process.