# RISCV (RV32I) - Sotfware
This section of the repository contains a RISC-V (RV32I) single-hart, simple Simics (software) CPU model,
implemented at the TLM (Transaction-Level Modeling) level. It does not support superscalar execution or
superspipelines.

The model executes the same binaries as the CPU model created for FPGA. Development is still in a very
early stage. So far, only instruction fetching from RAM via `direct_memory` and `direct_memory_lookup`
interfaces, along with instruction decoding and disassembly logic, have been implemented. Most of the
interfaces and functionalities required to make the model functional at a very basic level are still
in progress.

# Run
The CPU model is currently running in vacuum mode — an environment provided by Simics for developing
device modules in isolation from the rest of the hardware typically included in a virtual platform.

At this point, no additional modules or components are provided to run the CPU model together with
peripheral devices.

```bash
simics riscv-vacuum.simics
```

This command loads the CPU model and an **ELF** binary built from the `test_op_imm.s` assembly file.
That file was created to verify `I-TYPE` instructions and will be extended in the future.

```bash
[rcpu info] direct memory lookup: target='ram', offs='0', access='1'
┌──────────┬──────┬──┬──────┬──────────┬──────┬────┬─────┬────┐
│      Base│Object│Fn│Offset│    Length│Target│Prio│Align│Swap│
├──────────┼──────┼──┼──────┼──────────┼──────┼────┼─────┼────┤
│       0x0│sdp   │  │   0x0│      0x10│      │   0│    8│    │
│0x10000000│ram   │  │   0x0│0x10000000│      │   0│     │    │
└──────────┴──────┴──┴──────┴──────────┴──────┴────┴─────┴────┘
p:0x10000000  9305 5000 1386 1500 9376 c600 1387 f6ff  ..P......v......
p:0x10000010  9367 5700 13c8 c700 9338 d800 9332 b800  .gW......8...2..
p:0x10000020  1313 c801 9323 63ff 133e 637e 935e c301  .....#c..>c~.^..
p:0x10000030  135f c341 6780 0000 0000 0000 0000 0000  ._.Ag...........
p:0x10000040  0000 0000 0000 0000 0000 0000 0000 0000  ................
p:0x10000050  0000 0000 0000 0000 0000 0000 0000 0000  ................
p:0x10000060  0000 0000                                ....
simics>
```

If the `processor_cli` interface is manually triggered, the CPU fetches instruction pointed by
address from memory, decodes it, and disassembles it. So far no proper cycle mechanism was
created.

```bash
simics> @conf.rcpu.iface.processor_cli.get_disassembly("p", 0, 1, None)
(4, 'riscv-cpu: li a1, 5')
simics> @conf.rcpu.iface.processor_cli.get_disassembly("p", 4, 1, None)
(4, 'riscv-cpu: addi a2, a1, 1')
simics> @conf.rcpu.iface.processor_cli.get_disassembly("p", 8, 1, None)
(4, 'riscv-cpu: andi a3, a2, 12')
```

To see state of registers try:

```bash
simics> rcpu.pregs -all
```
