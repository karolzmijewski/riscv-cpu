#include "fde_core.hpp"
#include "fde_emul.hpp"
#include "fde_dbg.hpp"
#include "fde_type.hpp"
#include "fde_opcode.hpp"
#include "fde_execute.hpp"

static void reg_read(int *p_reg_file, reg_nr_t rs1, reg_nr_t rs2, int *p_rs1_val, int *p_rs2_val) {
#pragma HLS INLINE
    *p_rs1_val = p_reg_file[rs1];
    *p_rs2_val = p_reg_file[rs2];
}

static void reg_write(int *p_reg_file, reg_nr_t rd, int rd_val) {
#pragma HLS INLINE
    if (rd != 0) {
        p_reg_file[rd] = rd_val;
    }
}

static addr_t get_next_pc(dec_instr_t dec_instr, addr_t pc, int rs1_val, bit_t branch_taken) {
// for all types except J_TYPE,
// the pc was defined as word pointer, so +1 will get
// the net word (next instruction in memory)
#pragma HLS INLINE
    switch(dec_instr.type) {
        case R_TYPE: return (addr_t)(pc + 1);
        case I_TYPE:
            if (dec_instr.opcode == JALR) {
                return (addr_t)(((rs1_val + (int)dec_instr.imm) & 0xFFFFFFFE) >> 2);
            }
            return (addr_t)(pc + 1);
        case S_TYPE: return (addr_t)(pc + 1);
        case B_TYPE:
            if (branch_taken) {
                return (addr_t)(pc + (dec_instr.imm >> 1));
            }
            return (addr_t)(pc + 1);
        case U_TYPE: return (addr_t)(pc + 1);
        case J_TYPE: return (addr_t)(pc + (dec_instr.imm >> 1));
        case OTHER_TYPE: return (addr_t)(pc + 1);
        default: return (addr_t)(pc + 1);
    }
}

static int get_result(int rs1_val, int rs2_val, dec_instr_t dec_instr, addr_t pc) {
#pragma HLS INLINE
    int imm12 = ((int)dec_instr.imm) << 12; // for U_TYPE
    //int imm20 = ((int)dec_instr.imm) << 20; // for J_TYPE
    addr_t pc4 = pc << 2; // pc * 4
    addr_t npc4 = (pc + 1) << 2; // (pc + 1) * 4
    switch(dec_instr.opcode) {
        case LOAD:
            return 0;
        case STORE:
            return 0;
        case OP_IMM:
            switch(dec_instr.func3) {
                case 0b000: return rs1_val + (int)dec_instr.imm; // ADDI
                case 0b010: return (rs1_val < (int)dec_instr.imm) ? 1 : 0; // SLTI
                case 0b011: return ((unsigned int)rs1_val < (unsigned int)dec_instr.imm) ? 1 : 0; // SLTIU
                case 0b100: return rs1_val ^ (int)dec_instr.imm; // XORI
                case 0b110: return rs1_val | (int)dec_instr.imm; // ORI
                case 0b111: return rs1_val & (int)dec_instr.imm; // ANDI
                case 0b001: // SLLI
                    if (dec_instr.func7 == 0b0000000) {
                        return rs1_val << (dec_instr.imm & 0b11111);
                    } else {
                        return 0;
                    }
                case 0b101:
                    if (dec_instr.func7 == 0b0000000) { // SRLI
                        return (unsigned int)rs1_val >> (dec_instr.imm & 0b11111);
                    } else if (dec_instr.func7 == 0b0100000) { // SRAI
                        return rs1_val >> (dec_instr.imm & 0b11111);
                    } else {
                        return 0;
                    }
                default: return 0;
            }
        case OP:
            switch(dec_instr.func3) {
                case 0b000:
                    if (dec_instr.func7 == 0b0000000) { // ADD
                        return rs1_val + rs2_val;
                    } else if (dec_instr.func7 == 0b0100000) { // SUB
                        return rs1_val - rs2_val;
                    }
                    return 0;
                case 0b001:
                    if (dec_instr.func7 == 0b0000000) { // SLL
                        return rs1_val << (rs2_val & 0b11111);
                    }
                    return 0;
                case 0b010:
                    if (dec_instr.func7 == 0b0000000) { // SLT
                        return (rs1_val < rs2_val) ? 1 : 0;
                    }
                    return 0;
                case 0b011:
                    if (dec_instr.func7 == 0b0000000) { // SLTU
                        return ((unsigned int)rs1_val < (unsigned int)rs2_val) ? 1 : 0;
                    }
                    return 0;
                case 0b100:
                    if (dec_instr.func7 == 0b0000000) { // XOR
                        return rs1_val ^ rs2_val;
                    }
                    return 0;
                case 0b101:
                    if (dec_instr.func7 == 0b0000000) { // SRL
                        return (unsigned int)rs1_val >> (rs2_val & 0b11111);
                    } else if (dec_instr.func7 == 0b0100000) { // SRA
                        return rs1_val >> (rs2_val & 0b11111);
                    }
                    return 0;
                case 0b110:
                    if (dec_instr.func7 == 0b0000000) { // OR
                        return rs1_val | rs2_val;
                    }
                    return 0;
                case 0b111:
                    if (dec_instr.func7 == 0b0000000) { // AND
                        return rs1_val & rs2_val;
                    }
                    return 0;
                default: return 0;
            }
            return 0;
        case BRANCH:
            switch(dec_instr.func3) {
                case 0b000: // BEQ
                    return (rs1_val == rs2_val);
                case 0b001: // BNE
                    return (rs1_val != rs2_val);
                case 0b100: // BLT
                    return (rs1_val < rs2_val);
                case 0b101: // BGE
                    return (rs1_val >= rs2_val);
                case 0b110: // BLTU
                    return ((unsigned int)rs1_val < (unsigned int)rs2_val);
                case 0b111: // BGEU
                    return ((unsigned int)rs1_val >= (unsigned int)rs2_val);
                default: return 0;
            }
            return 0;
        case JALR:
            return npc4;
        case JAL:
            return npc4;
        case LUI:
            return imm12;
        case AUIPC:
            return pc4 + imm12;
        default: return 0;
    }
}

void execute(dec_instr_t dec_instr, int *p_reg_file, addr_t pc, addr_t *p_next_pc) {
#pragma HLS INLINE off
    int rs1_val, rs2_val;
    reg_read(p_reg_file, dec_instr.rs1, dec_instr.rs2, &rs1_val, &rs2_val);
    int rd_val = get_result(rs1_val, rs2_val, dec_instr, pc);
    reg_write(p_reg_file, dec_instr.rd, rd_val);
    *p_next_pc = get_next_pc(dec_instr, pc, rs1_val, (bit_t)rd_val);
#ifndef __SYNTHESIS__
#ifdef DBG_EMULATE
    emulate(p_reg_file, dec_instr, *p_next_pc);
#endif /* DBG_EMULATE */
#endif /* !__SYNTHESIS__! */
}