# RISC-V: CPU

> ⚠️ **Warning**: This project is currently under development and not yet fully functional. Stay tuned for updates!

---

## Overview

The primary objective of this project is to develop a simulation environment for a RISC-V CPU, initially
based on the **RV32I** core extension. The design may be further extended in the future to support additional
RISC-V extensions and features.

The repository will provide two types of simulation environments:
- **Software simulation** – leveraging the Simics simulator provided by Intel for functional modeling and software development.
    The [RISC-V](https://intel.github.io/tsffs/simics/reference-manual-risc-v-cpu/index.html) simple processor is provided by
    Intel as a reference implementation. However, since the source code is not open-source, introducing architectural changes
    is challenging.
- **Hardware simulation** – implemented using High-Level Synthesis (HLS) with AMD Vitis, targeting Xilinx-based FPGAs
    for hardware prototyping and performance evaluation.

The goal is to create a flexible framework that supports both early-stage software testing and
hardware-accelerated exploration of RISC-V designs.

## Documentation
- [Environment setup](./docs/sw-env-setup.md) - IDE / CAD setup instruction
- [Dealing with code](./docs/sw-code-op.md) - RV32I apps code compilation, dump, run, and debug
- [Coding guide](./docs/sw-code-guide.md) - RISCV IP coding tips, best practices, rules, and limitations
- [RV32I details](./docs/hw-riscv32i-basis.md) - RV32I ISA notes
