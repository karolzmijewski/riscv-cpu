# RISCV (RV32I) - Sotfware
This section of the repository contains a RISC-V (RV32I) single-hart, 32-bit, in-order, non-pipelined,
not-hyperthreading, simple Simics (software) CPU model, implemented at the TLM (Transaction-Level
Modeling) level.

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

To run the CPU:

```bash
simics> rcpu.log-level 4
[rcpu] Changing log level: 1 -> 4
simics> ptime
┌─────────┬─────┬──────┬────────┐
│Processor│Steps│Cycles│Time (s)│
├─────────┼─────┼──────┼────────┤
│rcpu     │    0│     0│   0.000│
└─────────┴─────┴──────┴────────┘
simics> rcpu.disassemble
[rcpu info] direct memory: data[0]='0x93', data[1]='0x05', data[2]='0x50', data[3]='0x00'
[rcpu info] instr: '0x500593'
[rcpu info] dec_instr: opcode='0x04', rd='0x0b', func3='0x0', rs1='0x00', rs2='0x05', func7='0x00', type='0x2'
[rcpu info] addr: 0x00000000 disassembled: li a1, 5
li a1, 5
simics> run count = 1
[rcpu info] Start execution
[rcpu info] Fetching instruction from address 0x00000000
[rcpu info] Fetched instruction 0x00500593 from address 0x00000000
[rcpu info] Decoding instruction 0x00500593
[rcpu info] Executing OP_IMM instruction
[rcpu info] Stop execution
simics> pregs
x0 (zero) = 0x00000000
x1 (ra) = 0x00000000
x2 (sp) = 0x00000000
x3 (gp) = 0x00000000
x4 (tp) = 0x00000000
x5 (t0) = 0x00000000
x6 (t1) = 0x00000000
x7 (t2) = 0x00000000
x8 (s0) = 0x00000000
x9 (s1) = 0x00000000
x10 (a0) = 0x00000000
x11 (a1) = 0x00000005
x12 (a2) = 0x00000000
x13 (a3) = 0x00000000
x14 (a4) = 0x00000000
x15 (a5) = 0x00000000
x16 (a6) = 0x00000000
x17 (a7) = 0x00000000
x18 (s2) = 0x00000000
x19 (s3) = 0x00000000
x20 (s4) = 0x00000000
x21 (s5) = 0x00000000
x22 (s6) = 0x00000000
x23 (s7) = 0x00000000
x24 (s8) = 0x00000000
x25 (s9) = 0x00000000
x26 (s10) = 0x00000000
x27 (s11) = 0x00000000
x28 (t3) = 0x00000000
x29 (t4) = 0x00000000
x30 (t5) = 0x00000000
x31 (t6) = 0x00000000

simics> ptime
┌─────────┬─────┬──────┬────────┐
│Processor│Steps│Cycles│Time (s)│
├─────────┼─────┼──────┼────────┤
│rcpu     │    1│     1│   0.000│
└─────────┴─────┴──────┴────────┘
simics> rcpu.disassemble
[rcpu info] direct memory: data[0]='0x13', data[1]='0x86', data[2]='0x15', data[3]='0x00'
[rcpu info] instr: '0x158613'
[rcpu info] dec_instr: opcode='0x04', rd='0x0c', func3='0x0', rs1='0x0b', rs2='0x01', func7='0x00', type='0x2'
[rcpu info] addr: 0x00000004 disassembled: addi a2, a1, 1
addi a2, a1, 1
simics> run count = 1
[rcpu info] Start execution
[rcpu info] Fetching instruction from address 0x00000004
[rcpu info] Fetched instruction 0x00158613 from address 0x00000004
[rcpu info] Decoding instruction 0x00158613
[rcpu info] Executing OP_IMM instruction
[rcpu info] Stop execution
simics> pregs
x0 (zero) = 0x00000000
x1 (ra) = 0x00000000
x2 (sp) = 0x00000000
x3 (gp) = 0x00000000
x4 (tp) = 0x00000000
x5 (t0) = 0x00000000
x6 (t1) = 0x00000000
x7 (t2) = 0x00000000
x8 (s0) = 0x00000000
x9 (s1) = 0x00000000
x10 (a0) = 0x00000000
x11 (a1) = 0x00000005
x12 (a2) = 0x00000006
x13 (a3) = 0x00000000
x14 (a4) = 0x00000000
x15 (a5) = 0x00000000
x16 (a6) = 0x00000000
x17 (a7) = 0x00000000
x18 (s2) = 0x00000000
x19 (s3) = 0x00000000
x20 (s4) = 0x00000000
x21 (s5) = 0x00000000
x22 (s6) = 0x00000000
x23 (s7) = 0x00000000
x24 (s8) = 0x00000000
x25 (s9) = 0x00000000
x26 (s10) = 0x00000000
x27 (s11) = 0x00000000
x28 (t3) = 0x00000000
x29 (t4) = 0x00000000
x30 (t5) = 0x00000000
x31 (t6) = 0x00000000

simics> ptime
┌─────────┬─────┬──────┬────────┐
│Processor│Steps│Cycles│Time (s)│
├─────────┼─────┼──────┼────────┤
│rcpu     │    2│     2│   0.000│
└─────────┴─────┴──────┴────────┘
```