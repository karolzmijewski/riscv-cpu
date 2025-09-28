# RISC-V: CPU

> ⚠️ **Warning**: This project is currently under development and not yet fully functional.
> Stay tuned for updates!

---

## Overview

The primary objective of this project is to develop a simulation environment for a RISC-V CPU.
This is a basic implementation of a RISC-V **RV32I** CPU model. It supports a subset of the RISC-V
instruction set architecture (ISA) and is intended for educational purposes.

The model is single-core, 32-bit, in-order, non-pipelined, and does not support hyperthreading.
It provides basic functionalities such as instruction fetch, decode, execute, memory access, and
write-back stages. The model will also include a simple memory management unit (MMU - currently
no available) for address translation and supports basic exception handling. Note that this is a
simplified model and may not include all features of a full-fledged RISC-V CPU implementation. For
more advanced features and optimizations, please refer to more comprehensive RISC-V CPU models or
implementations.

The design may be further extended in the future to support additional RISC-V extensions and
features.

The repository will provide two types of simulation environments:
- **Software simulation** – leveraging Intel’s Simics simulator for functional modeling and software
    development. Intel provides a simple
    [RISC-V](https://intel.github.io/tsffs/simics/reference-manual-risc-v-cpu/index.html)
    processor as a reference implementation; however, because the source code is not open source,
    introducing architectural modifications is difficult.
- **Hardware simulation** – implemented using High-Level Synthesis (HLS) with AMD Vitis, targeting
    Xilinx-based FPGAs for hardware prototyping and performance evaluation.

The goal is to create a flexible framework that supports both early-stage software testing and
hardware-accelerated exploration of RISC-V designs.

## Documentation
- [Environment setup](./docs/sw-env-setup.md) - IDE / CAD setup instruction
- [Coding tips](./docs/sw-code-op.md) - RV32I apps code compilation, dump, run, and debug
- [Coding guide](./docs/sw-code-guide.md) - RISCV IP coding best practices, rules, and limitations
- [RV32I details](./docs/hw-riscv32i-basis.md) - RV32I ISA notes
