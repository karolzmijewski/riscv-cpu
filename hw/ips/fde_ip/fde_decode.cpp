#include "fde_core.hpp"
#include "fde_decode.hpp"
#include "fde_type.hpp"
#include "fde_dbg.hpp"

#ifndef __SYNTHESIS__
#ifdef DBG_FDE_IP
#include "fde_print.hpp"
#endif /* !DBG_FDE_IP! */
#endif /* !__SYNTHESIS__! */

static void dec_instr(instr_t instr, dec_instr_t *p_dec_instr) {
#pragma HLS INLINE
    p_dec_instr->opcode = (instr >> 2);
    p_dec_instr->rd = (instr >> 7);
    p_dec_instr->func3 = (instr >> 12);
    p_dec_instr->rs1 = (instr >> 15);
    p_dec_instr->rs2 = (instr >> 20);
    p_dec_instr->func7 = (instr >> 25);
    p_dec_instr->type = get_op_type(p_dec_instr->opcode);
}

static void dec_imm(instr_t instr, dec_instr_t *p_dec_instr) {
#pragma HLS INLINE
    dec_imm_t dec_imm;
    dec_imm.instr_31 = (instr >> 31);
    dec_imm.instr_30_25 = (instr >> 25);
    dec_imm.instr_24_21 = (instr >> 21);
    dec_imm.instr_20 = (instr >> 20);
    dec_imm.instr_19_12 = (instr >> 12);
    dec_imm.instr_11_8 = (instr >> 8);
    dec_imm.instr_7 = (instr >> 7);
    switch(p_dec_instr->type) {
        case UNDEF_TYPE: p_dec_instr->imm = 0; break;
        case R_TYPE: p_dec_instr->imm = 0; break;
        case I_TYPE: p_dec_instr->imm = enc_i_imm(dec_imm); break;
        case S_TYPE: p_dec_instr->imm = enc_s_imm(dec_imm); break;
        case B_TYPE: p_dec_instr->imm = enc_b_imm(dec_imm); break;
        case U_TYPE: p_dec_instr->imm = enc_u_imm(dec_imm); break;
        case J_TYPE: p_dec_instr->imm = enc_j_imm(dec_imm); break;
        case OTHER_TYPE: p_dec_instr->imm = 0;
            break;
    }
}

void decode(instr_t instr, dec_instr_t *p_dec_instr) {
#pragma HLS INLINE off

    dec_instr(instr, p_dec_instr);
    dec_imm(instr, p_dec_instr);

#ifndef __SYNTHESIS__
#ifdef DBG_FDE_IP
    print_dec_instr(*p_dec_instr);
#endif /* !DBG_FDE_IP! */
#endif /* !__SYNTHESIS__! */
}
