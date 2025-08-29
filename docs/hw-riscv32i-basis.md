# RV32I Basis

## ISA Command Formats

Each instruction in the RV32I ISA is **32 bits long**. The architecture defines four major formats
(with some variants).
> **Note:** The last two bits (`[1:0]`) are reserved for the **RVC extension** (Compressed Instructions), which use 16-bit encodings.

---

### Instruction Formats

- **R-TYPE** (Register):
    Used for register-to-register operations (e.g., `add`, `sub`, `and`, `or`).
    ```
    31      25 24   20 19   15 14 12 11    7 6      2 1   0
    ┌─────────┬───────┬───────┬─────┬───────┬────────┬────┐
    │  func7  │  rs2  │  rs1  │ f3  │   rd  │ opcode │ 11 │
    └─────────┴───────┴───────┴─────┴───────┴────────┴────┘
    ```

---

- **I-TYPE** (Immediate):
    Used for operations with an immediate value (e.g., `addi`, loads).
    ```
    31             20 19   15 14 12 11    7 6      2 1   0
    ┌────────────────┬───────┬─────┬───────┬────────┬────┐
    │   imm[11:0]    │  rs1  │ f3  │   rd  │ opcode │ 11 │
    └────────────────┴───────┴─────┴───────┴────────┴────┘
    ```

---

- **S-TYPE** (Store):
    Used for store instructions (e.g., `sw`, `sh`, `sb`).
    ```
    31        25 24   20 19   15 14 12 11       7 6      2 1   0
    ┌───────────┬───────┬───────┬─────┬──────────┬────────┬────┐
    │ imm[11:5] │  rs2  │  rs1  │ f3  │ imm[4:0] │ opcode │ 11 │
    └───────────┴───────┴───────┴─────┴──────────┴────────┴────┘
    ```
    **B-TYPE** (Branch) - variant of **S-TYPE**:

    The 12-bit immediate value is split across the instruction fields as follows:

    **imm[11:5]** - is devided into:
    - a [11:11]
    - b [10:5]

    **imm[4:0]** - is devided into:
    - c [4:1]
    - d [1:0]

    The 12-bit immediate value is assembled in the order: `a + d + b + c`.  
    It is then shifted left (trailing `0`) and sign, extended to 32 bits to form the branch displacement.  
    This displacement is added to the program counter (`pc`) if the branch condition evaluates to true.

---

- **U-TYPE** (Upper Immediate):
    Used for instructions with large immediates (e.g., `lui`, `auipc`).
    ```
    31                           12 11    7 6      2 1   0
    ┌──────────────────────────────┬───────┬────────┬────┐
    │          imm[31:12]          │   rd  │ opcode │ 11 │
    └──────────────────────────────┴───────┴────────┴────┘
    ```

    **J-TYPE** (Jump) - variant of **U-TYPE**:

    The 20-bit immediate value is split across the instruction fields as follows:
    - e [20:20]
    - f [19-9]
    - g [8:8]
    - h [7:0]

    The 20-bit immediate value is assembled in the order: `e + h + g + f`.  
    It is then shifted left by one bit (trailing `0`) and sign-extended to 32 bits to form the jump displacement.  
    This displacement is added to the program counter (`pc`) when the jump instruction is executed.


#### Instructions opcodes and formats (non privileged RISC-V ISA):

| opcode | 000      | 001      | 010             | 011       | 100    | 101      | 110              | 111    |
|--------|----------|----------|-----------------|-----------|--------|----------|------------------|--------|
| 00     | LOAD     | LOD-FP   | CUSTOM-0        | MISC-MEM  | OP-IMM | AUIPC    | OP-IMM-23        | RV48-0 |
|        | I-TYPE   | OTHER    | OTHER           | OTHER     | I-TYPE | U-TYPE   | OTHER            | OTHER  |
| 01     | STORE    | STORE-FP | CUSTOM-1 / AMO  | OP        | LUI    | OP-32    | RV64             |        |
|        | S-TYPE   | OTHER    | OTHER           | OTHER     | R-TYPE | U-TYPE   | OTHER            | OTHER  |
| 10     | MADD     | MSUB     | NMSUB           | NMADD     | OP-FP  | RVSD-0   | CUSTOM2-RV128    | RV48-1 |
|        | OTHER    | OTHER    | OTHER           | OTHER     | OTHER  | OTHER    | OTHER            | OTHER  |
| 11     | BRANCH   | JALR     | RSVD-1          | JAL       | SYSTEM | RSVD-2   | CUSTOM3-RV128    | RV80   |
|        | B-TYPE   | I-TYPE   | OTHER           | J-TYPE    | OTHER  | OTHER    | OTHER            | OTHER  |
