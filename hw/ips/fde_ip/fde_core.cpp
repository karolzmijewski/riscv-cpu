#include "fde_core.hpp"

i_imm_t enc_i_imm(dec_imm_t dec_imm) {
#pragma HLS INLINE
    i_imm_t enc_i_imm_val = ((i_imm_t)dec_imm.instr_31 << 11);
    enc_i_imm_val |= ((i_imm_t)dec_imm.instr_30_25 << 5);
    enc_i_imm_val |= ((i_imm_t)dec_imm.instr_24_21 << 1);
    enc_i_imm_val |= ((i_imm_t)dec_imm.instr_20);
    return enc_i_imm_val;
}

s_imm_t enc_s_imm(dec_imm_t dec_imm) {
#pragma HLS INLINE
    s_imm_t enc_s_imm_val = ((s_imm_t)dec_imm.instr_31 << 11);
    enc_s_imm_val |= ((s_imm_t)dec_imm.instr_30_25 << 5);
    enc_s_imm_val |= ((s_imm_t)dec_imm.instr_11_8 << 1);
    enc_s_imm_val |= ((s_imm_t)dec_imm.instr_7);
    return enc_s_imm_val;
}

b_imm_t enc_b_imm(dec_imm_t dec_imm) {
#pragma HLS INLINE
    b_imm_t enc_b_imm_val = ((b_imm_t)dec_imm.instr_31 << 11);
    enc_b_imm_val |= ((b_imm_t)dec_imm.instr_7 << 10);
    enc_b_imm_val |= ((b_imm_t)dec_imm.instr_30_25 << 4);
    enc_b_imm_val |= ((b_imm_t)dec_imm.instr_11_8);
    return enc_b_imm_val;
}

u_imm_t enc_u_imm(dec_imm_t dec_imm) {
#pragma HLS INLINE
    u_imm_t enc_u_imm_val = ((u_imm_t)dec_imm.instr_31 << 19);
    enc_u_imm_val |= ((u_imm_t)dec_imm.instr_30_25 << 13);
    enc_u_imm_val |= ((u_imm_t)dec_imm.instr_24_21 << 9);
    enc_u_imm_val |= ((u_imm_t)dec_imm.instr_20 << 8);
    enc_u_imm_val |= ((u_imm_t)dec_imm.instr_19_12);
    return enc_u_imm_val;
}

j_imm_t enc_j_imm(dec_imm_t dec_imm) {
#pragma HLS INLINE
    j_imm_t enc_j_imm_val = ((j_imm_t)dec_imm.instr_31 << 19);
    enc_j_imm_val |= ((j_imm_t)dec_imm.instr_19_12 << 11);
    enc_j_imm_val |= ((j_imm_t)dec_imm.instr_20 << 10);
    enc_j_imm_val |= ((j_imm_t)dec_imm.instr_30_25 << 4);
    enc_j_imm_val |= ((j_imm_t)dec_imm.instr_24_21);
    return enc_j_imm_val;
}