# Dealing with code

- [Compile](#compile)
    - [SPIKE simulator](#spike-simulator)
    - [FPGA](#fpga)
- [Dump](#dump)
- [Run](#run)
    - [SPIKE simulator](#spike-simulator-1)
    - [FPGA](#fpga-1)
- [Debug](#debug)
    - [SPIKE simulator](#spike-simulator-2)
        - [Internal Debugger](#internal-debugger)
        - [GDB](#gdb)

## Compile
### SPIKE simulator
The linker automatically adds the `_start` entry point, which invokes the `main` function and
places the code at an address compatible with the operating system’s memory mapping (in this
case, `0x000101b0`).

Compiling C code:
```bash
riscv32-unknown-elf-gcc test_op_imm.c -o spike_test_op_imm
```

Generating Assembly code:
```bash
riscv32-unknown-elf-gcc -O0 -S test_op_imm.c
```

Compiling Assembly code (release build):
```bash
riscv32-unknown-elf-gcc test_op_imm.s -o spike_test_op_imm
```

Compiling Assembly code (debug build):
```bash
riscv32-unknown-elf-gcc -g -nostartfiles -T ../spike.lds test_op_imm.s -o spike_test_op_imm_debug
```
This produces an ELF file with debug symbols. The command requires a linker script for
Spike: [splike.lds](../spike.lds)

### FPGA
The code section must be set to address 0 because the program runs without an operating
system (bare-metal execution).

Compiling Assembly to ELF:
```bash
riscv32-unknown-elf-gcc -nostartfiles -Ttext 0 test_op_imm.s -o fpga_test_op_imm.elf
```

Converting ELF to BIN:
```bash
riscv32-unknown-elf-objcopy -O binary fpga_test_op_imm.elf fpga_test_op_imm.bin
# to verify:
od -t x4 fpga_test_op_imm.bin
```

Converting BIN to HEX:
```bash
hexdump -v -e '"0x" /4 "%08x" ",\n"' test_op_imm_0.bin > test_op_imm_0.hex
# to verify:
cat test_op_imm_0.hex
```
The resulting HEX file provides a suitable structure for use as a C array initializer,
listing the values required to populate a code RAM array.

## Dump
To generate an assembly dump:
```bash
riscv32-unknown-elf-objdump -d spike_test_op_imm > spike_test_op_imm.dump
riscv32-unknown-elf-objdump -d fpga_test_op_imm.elf > fpga_test_op_imm.dump
```

## Run
Follow the steps below to execute the code.

### SPIKE simulator
The `pk` (proxy kernel) acts as a lightweight substitute for the operating system kernel.
It provides access to a limited set of system calls (primarily I/O) and includes `bbl`
(Berkeley Boot Loader), which loads the application.
```bash
spike --isa=RV32I /opt/riscv/riscv32-unknown-elf/bin/pk spike_test_op_imm
```

### FPGA
TBD

## Debug

### SPIKE simulator
#### Internal Debugger
1. Generate a symbol table to verify the `main` function address (starting address).
2. Skip the `_start` section and jump directly to the application using the resolved address.
```bash
riscv32-unknown-elf-objdump -t test_op_imm | grep main
000101b0 g       .text  00000000 main
spike --isa=RV32I -d /opt/riscv/riscv32-unknown-elf/bin/pk test_op_imm
(spike) untiln pc 0 000101b0
(spike) reg 0 a2
```

#### GDB
In simulation, SPIKE maps code execution to memory address `0x10010000` (with the `.data`
section immediately following `.text`). To debug with GCC, `OpenOCD` middleware is required.

- Run SPIKE
```bash
spike --isa=RV32I --rbb-port=9824 -m0x0010010000:0x10000 test_op_imm_debug
```
- Run OpenOCD
```bash
openocd -f ../spike.cfg
```
- Run GDB
```
riscv32-unknown-elf-gdb
(gdb) target remote localhost:3333
(gdb) file test_op_imm_debug
(gdb) load
(gdb) list
(gdb) p $pc
(gdb) si
(gdb) info reg a1
(gdb) b 16
(gdb) c
```
