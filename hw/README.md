# RISCV (RV32I) - Hardware
This section of the repository contains a RISC-V (RV32I) single-hart, 32-bit, in-order, non-pipelined,
not-hyperthreading, simple FPGA (HSL for Xilinx) CPU model.

The model executes the same binaries as the CPU model created for Simics simulator. Development is
still in a very early stage. So far, instruction fetch-decode-execute path, along with instruction
decoding and disassembly logic, have been implemented. Most of the interfaces and functionalities
required to make the model functional at a very basic level are still in progress.
