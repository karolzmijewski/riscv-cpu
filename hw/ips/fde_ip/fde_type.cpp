#include "ap_int.h"
#include "fde_core.hpp"
#include "fde_type.hpp"

/**
 * This function decodes the opcode to determine the operation type.
 * The opcode is divided into two parts: the upper 2 bits (opch) and
 * the lower 3 bits (opcl). The function uses these parts to identify
 * the specific operation type by calling helper functions for each
 * combination of opch and opcl.
 * It's a representation of MUX (multiplexer 3*8 -> 3) logic, where the
 * selection lines (opch and opcl) determine the output (operation type).
 */
static op_type_t get_subtype_00(ap_uint<3> opcl) {
#pragma HLS INLINE
    switch (opcl) {
        case 0b000: return I_TYPE; // LOAD
        case 0b001: return OTHER_TYPE; // LOAD_FP
        case 0b010: return OTHER_TYPE; // CUSTOM_0
        case 0b011: return OTHER_TYPE; // MISC_MEM
        case 0b100: return I_TYPE; // OP_IMM
        case 0b101: return U_TYPE; // AUIPC
        case 0b110: return OTHER_TYPE; // OP_IMM_32
        case 0b111: return OTHER_TYPE; // RV_48_0 
    }
    return UNDEF_TYPE;
}

/**
 * It's a representation of another MUX (multiplexer 3*8 -> 3) logic, where the
 * selection lines (opch and opcl) determine the output (operation type).
 */
static op_type_t get_subtype_01(ap_uint<3> opcl) {
#pragma HLS INLINE
    switch(opcl) {
        case 0b000: return S_TYPE; // STORE
        case 0b001: return OTHER_TYPE; // STORE_FP
        case 0b010: return OTHER_TYPE; // CUSTOM_1
        case 0b011: return OTHER_TYPE; // AMO
        case 0b100: return R_TYPE; // OP
        case 0b101: return U_TYPE; // LUI
        case 0b110: return OTHER_TYPE; // OP_32
        case 0b111: return OTHER_TYPE; // RV_64
    }
    return UNDEF_TYPE;
}

/**
 * It's a representation of another MUX (multiplexer 3*8 -> 3) logic, where the
 * selection lines (opch and opcl) determine the output (operation type).
 */
static op_type_t get_subtype_10(ap_uint<3> opcl) {
#pragma HLS INLINE
    switch(opcl) {
        case 0b000: return OTHER_TYPE; // MADD
        case 0b001: return OTHER_TYPE; // MSUB
        case 0b010: return OTHER_TYPE; // NMSUB
        case 0b011: return OTHER_TYPE; // NMADD
        case 0b100: return OTHER_TYPE; // OP_FP
        case 0b101: return OTHER_TYPE; // RSVD_0
        case 0b110: return OTHER_TYPE; // CUSTOM_2_RV128
        case 0b111: return OTHER_TYPE; // RV_80
    }
    return UNDEF_TYPE;
}

/**
 * It's a representation of another MUX (multiplexer 3*8 -> 3) logic, where the
 * selection lines (opch and opcl) determine the output (operation type).
 */
static op_type_t get_subtype_11(ap_uint<3> opcl) {
#pragma HLS INLINE
    switch(opcl) {
        case 0b000: return B_TYPE; // BRANCH
        case 0b001: return I_TYPE; // JALR
        case 0b010: return OTHER_TYPE; // RSVD_1
        case 0b011: return J_TYPE; // JAL
        case 0b100: return OTHER_TYPE; // SYSTEM
        case 0b101: return OTHER_TYPE; // RSVD_2
        case 0b110: return OTHER_TYPE; // CUSTOM_3_RV128
        case 0b111: return OTHER_TYPE; // RV_80
    }
    return UNDEF_TYPE;
}

op_type_t get_op_type(opcode_t opcode) {
#pragma HLS INLINE
    ap_uint<2> opch = opcode >> 3;;
    ap_uint<3> opcl = opcode;
    switch(opch) {
        case 0b00: return get_subtype_00(opcl);
        case 0b01: return get_subtype_01(opcl);
        case 0b10: return get_subtype_10(opcl);
        case 0b11: return get_subtype_11(opcl);
    }
    return UNDEF_TYPE;
}
