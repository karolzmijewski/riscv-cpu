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

If the signal interface is manually triggered, the CPU fetches the first instruction from memory, decodes it,
and disassembles it. This approach was implemented only for testing. In the future, a proper cycle mechanism
will be created, and all helper functions will be registered as **CLI** methods associated with the CPU model.

```bash
simics> @conf.rcpu.iface.signal.signal_raise()
[rcpu info] direct memory: data[0]='0x93', data[1]='0x05', data[2]='0x50', data[3]='0x00'
[rcpu info] instr: '0x500593'
[rcpu info] dec_instr: opcode='0x04', rd='0x0b', func3='0x0', rs1='0x00', rs2='0x05', func7='0x00', type='0x2'
[rcpu info] disassembled: li a1, 5
```
