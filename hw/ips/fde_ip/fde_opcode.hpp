#ifndef FDE_OPCODE_HPP
#define FDE_OPCODE_HPP

// opcodes special values
#define RET 0x8067
#define NOP 0x13
#define RA 0x1
// opcodes 00
#define LOAD 0b00000
#define LOAD_FP 0b00001
#define CUSTOM_0 0b00010
#define MISC_MEM 0b00011
#define OP_IMM 0b00100
#define AUIPC 0b00101
#define OP_IMM_32 0b00110
#define RV48_0 0b00111
// opcodes 01
#define STORE 0b01000
#define STORE_FP 0b01001
#define CUSTOM_1 0b01010
#define AMO 0b01011
#define OP 0b01100
#define LUI 0b01101
#define OP_32 0b01110
#define RV_64 0b01111
// opcodes 10
#define MADD 0b10000
#define MSUB 0b10001
#define NMSUB 0b10010
#define NMADD 0b10011
#define OP_FP 0b10100
#define RSVD_0 0b10101
#define CUSTOM_2_RV128 0b10110
#define RV48_1 0b10111
//opcodes 11
#define BRANCH 0b11000
#define JALR 0b11001
#define RSVD_1 0b11010
#define JAL 0b11011
#define SYSTEM 0b11100
#define RSVD_2 0b11101
#define CUSTOM_3_RV128 0b11110
#define RV_80 0b11111

#endif /* !FDE_OPCODE_HPP! */