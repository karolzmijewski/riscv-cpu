#ifndef FDE_CORE_HPP
#define FDE_CORE_HPP

#include "ap_int.h"

// memory defines
// it is set to 2^16 words, single word is 2^2 (4 bytes) so it will consume
// (2^2)^16) = 2^18 bytes of memory, which is equal to 256 KB, change this
// parameter to extend the memory
#define RAM_ADDR_WIDTH 16
#define RAM_SIZE (1 << RAM_ADDR_WIDTH)
#define REG_FILE_SIZE 5
#define REGISTER_NR (1 << REG_FILE_SIZE)

// general types
typedef ap_uint<1> bit_t;

// basic types
typedef unsigned int instr_t;
typedef ap_uint<RAM_ADDR_WIDTH> addr_t;
typedef ap_uint<3> op_type_t;

// instr frame fileds
typedef ap_uint<5> opcode_t;
typedef ap_uint<5> reg_nr_t;
typedef ap_uint<3> func3_t;
typedef ap_uint<7> func7_t;
typedef ap_int<20> imm_t;

// imm_t - immediate field varieties
typedef ap_int<12> i_imm_t;
typedef ap_int<12> s_imm_t;
typedef ap_int<12> b_imm_t;
typedef ap_int<20> u_imm_t;
typedef ap_int<20> j_imm_t;

typedef struct dec_instr_s {
    opcode_t opcode;
    reg_nr_t rd;
    func3_t func3;
    reg_nr_t rs1;
    reg_nr_t rs2;
    func7_t func7;
    op_type_t type;
    imm_t imm;
} dec_instr_t;

typedef struct dec_imm_s {
    bit_t instr_31;
    ap_uint<6> instr_30_25;
    ap_uint<4> instr_24_21;
    bit_t instr_20;
    ap_uint<8> instr_19_12;
    ap_uint<4> instr_11_8;
    bit_t instr_7;
} dec_imm_t;

i_imm_t enc_i_imm(dec_imm_t dec_imm);
s_imm_t enc_s_imm(dec_imm_t dec_imm);
b_imm_t enc_b_imm(dec_imm_t dec_imm);
u_imm_t enc_u_imm(dec_imm_t dec_imm);
j_imm_t enc_j_imm(dec_imm_t dec_imm);

#endif /* !FDE_CORE_HPP! */