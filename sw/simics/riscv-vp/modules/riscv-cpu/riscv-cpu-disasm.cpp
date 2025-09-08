/**
 * Copyright © 2025 Karol Zmijewski
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this
 * software and associated documentation files (the “Software”), to deal in the Software
 * without restriction, including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
 * to whom the Software is furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 *
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
 * FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include <array>
#include <string>
#include <sstream>
#include <string_view>
#include "riscv-cpu-conf.hpp"
#include "riscv-cpu-types.hpp"
#include "riscv-cpu-disasm.hpp"

namespace kz::riscv::core {
    std::string riscv_cpu_disasm::get_type(op_type_t type) {
        switch(type) {
            case operation_type_t::UNDEF_TYPE: return "UNDEF_TYPE";
            case operation_type_t::R_TYPE: return "R_TYPE";
            case operation_type_t::I_TYPE: return "I_TYPE";
            case operation_type_t::S_TYPE: return "S_TYPE";
            case operation_type_t::B_TYPE: return "B_TYPE";
            case operation_type_t::U_TYPE: return "U_TYPE";
            case operation_type_t::J_TYPE: return "J_TYPE";
            case operation_type_t::OTHER_TYPE: return "OTHER_TYPE";
        }
        return "UNKNOWN_TYPE";
    }

    std::string riscv_cpu_disasm::get_opcode(opcode_t opcode) {
        switch (opcode) {
            case operation_code_t::LOAD: return "LOAD";
            case operation_code_t::LOAD_FP: return "LOAD_FP";
            case operation_code_t::CUSTOM_0: return "CUSTOM_0";
            case operation_code_t::MISC_MEM: return "MISC_MEM";
            case operation_code_t::OP_IMM: return "OP_IMM";
            case operation_code_t::AUIPC: return "AUIPC";
            case operation_code_t::OP_IMM_32: return "OP_IMM_32";
            case operation_code_t::RV48_0: return "RV48_0";
            case operation_code_t::STORE: return "STORE";
            case operation_code_t::STORE_FP: return "STORE_FP";
            case operation_code_t::CUSTOM_1: return "CUSTOM_1";
            case operation_code_t::AMO: return "AMO";
            case operation_code_t::OP: return "OP";
            case operation_code_t::LUI: return "LUI";
            case operation_code_t::OP_32: return "OP_32";
            case operation_code_t::RV_64: return "RV_64";
            case operation_code_t::MADD: return "MADD";
            case operation_code_t::MSUB: return "MSUB";
            case operation_code_t::NMSUB: return "NMSUB";
            case operation_code_t::NMADD: return "NMADD";
            case operation_code_t::OP_FP: return "OP_FP";
            case operation_code_t::RSVD_0: return "RSVD_0";
            case operation_code_t::CUSTOM_2_RV128: return "CUSTOM_2_RV128";
            case operation_code_t::RV48_1: return "RV48_1";
            case operation_code_t::BRANCH: return "BRANCH";
            case operation_code_t::JALR: return "JALR";
            case operation_code_t::RSVD_1: return "RSVD_1";
            case operation_code_t::JAL: return "JAL";
            case operation_code_t::SYSTEM: return "SYSTEM";
            case operation_code_t::RSVD_2: return "RSVD_2";
            case operation_code_t::CUSTOM_3_RV128: return "CUSTOM_3_RV128";
            case operation_code_t::RV_80: return "RV_80";
            default: return "UNKNOWN";
        }
    }

    std::string riscv_cpu_disasm::get_reg_name(reg_nr_t reg_nr, bool symb) {
        // string_view was used to avoid dynamic allocation for constexpr
        static constexpr std::array<std::string_view, RV32I_GP_REG_NUM> reg_names = {
            "zero", "ra",  "sp",  "gp",  "tp",  "t0",  "t1",  "t2",
            "s0",   "s1",  "a0",  "a1",  "a2",  "a3",  "a4",  "a5",
            "a6",   "a7",  "s2",  "s3",  "s4",  "s5",  "s6",  "s7",
            "s8",   "s9",  "s10", "s11", "t3",  "t4",  "t5",  "t6"
        };
        if (symb && reg_nr < reg_names.size())
            return std::string(reg_names[reg_nr]);
        return "x" + std::to_string(static_cast<unsigned>(reg_nr));
    }

    std::string riscv_cpu_disasm::get_mnemonic(opcode_t opcode, dec_instr_t dec_instr) {
        using namespace std::literals;
        // Lookup tables for LOAD
        static constexpr std::array<const char*, 8> load_table = {
            "lb", "lh", "lw", "ld", "lbu", "lhu", "lwu", "unknown"
        };
        // Lookup tables for STORE
        static constexpr std::array<const char*, 8> store_table = {
            "sb", "sh", "sw", "sd", "unknown", "unknown", "unknown", "unknown"
        };
        switch (opcode) {
            case operation_code_t::LOAD:
                if (dec_instr.func3 < load_table.size())
                    return load_table[dec_instr.func3];
                return "unknown";
            case operation_code_t::STORE:
                if (dec_instr.func3 < store_table.size())
                    return store_table[dec_instr.func3];
                return "unknown";
            case operation_code_t::OP_IMM: {
                switch (dec_instr.func3) {
                    case 0b000: return dec_instr.rs1 == 0 ? "li" : "addi";
                    case 0b010: return "slti";
                    case 0b011: return "sltiu";
                    case 0b100: return "xori";
                    case 0b110: return "ori";
                    case 0b111: return "andi";
                    case 0b001: return dec_instr.func7 == 0b0000000 ? "slli" : "unknown";
                    case 0b101:
                        if (dec_instr.func7 == 0b0000000) return "srli";
                        if (dec_instr.func7 == 0b0100000) return "srai";
                        return "unknown";
                    default: return "unknown";
                }
            }
            case operation_code_t::OP: {
                switch (dec_instr.func3) {
                    case 0b000:
                        if (dec_instr.func7 == 0b0000000) return "add";
                        if (dec_instr.func7 == 0b0100000) return "sub";
                        return "unknown";
                    case 0b001: return dec_instr.func7 == 0b0000000 ? "sll" : "unknown";
                    case 0b010: return dec_instr.func7 == 0b0000000 ? "slt" : "unknown";
                    case 0b011: return dec_instr.func7 == 0b0000000 ? "sltu" : "unknown";
                    case 0b100: return dec_instr.func7 == 0b0000000 ? "xor" : "unknown";
                    case 0b101:
                        if (dec_instr.func7 == 0b0000000) return "srl";
                        if (dec_instr.func7 == 0b0100000) return "sra";
                        return "unknown";
                    case 0b110: return dec_instr.func7 == 0b0000000 ? "or" : "unknown";
                    case 0b111: return dec_instr.func7 == 0b0000000 ? "and" : "unknown";
                    default: return "unknown";
                }
            }
            case operation_code_t::BRANCH: {
                static constexpr std::array<const char*, 8> branch_table = {
                    "beq", "bne", "unknown", "unknown", "blt", "bge", "bltu", "bgeu"
                };
                if (dec_instr.func3 < branch_table.size())
                    return branch_table[dec_instr.func3];
                return "unknown";
            }
            case operation_code_t::JAL:
                return dec_instr.rd == 0 ? "j" : "jal";
            case operation_code_t::JALR:
                if (dec_instr.rd == 0 && dec_instr.rs1 == operation_code_t::RA) return "ret";
                if (dec_instr.rd == 0) return "jr";
                return "jalr";
            case operation_code_t::LUI: return "lui";
            case operation_code_t::AUIPC: return "auipc";
            default: return "unknown";
        }
    }

    std::string riscv_cpu_disasm::disasm(addr_t pc, dec_instr_t dec_instr) {
        std::ostringstream ss;
        std::string mnemonic = get_mnemonic(dec_instr.opcode, dec_instr);
        switch (dec_instr.type) {
            case operation_type_t::R_TYPE: {
                ss << mnemonic << " "
                << get_reg_name(dec_instr.rd) << ", "
                << get_reg_name(dec_instr.rs1) << ", "
                << get_reg_name(dec_instr.rs2);
                break;
            }
            case operation_type_t::I_TYPE: {
                if (dec_instr.opcode == operation_code_t::JALR) {
                    ss << mnemonic;
                    if (dec_instr.rd != 0) {
                        ss << " ";
                        if (dec_instr.rd != operation_code_t::RA) {
                            ss << get_reg_name(dec_instr.rd) << ", ";
                        }
                    } else {
                        ss << " ";
                    }
                    if (dec_instr.imm == 0) {
                        ss << get_reg_name(dec_instr.rs1);
                    } else {
                        ss << (int)dec_instr.imm << "(" << get_reg_name(dec_instr.rs1) << ")";
                    }
                } else if (dec_instr.opcode == operation_code_t::OP_IMM) {
                    ss << mnemonic << " " << get_reg_name(dec_instr.rd) << ", ";
                    if (dec_instr.func3 != 0b000 || dec_instr.rs1 != 0) {
                        ss << get_reg_name(dec_instr.rs1) << ", ";
                    }
                    if (dec_instr.func3 != 0b001 && dec_instr.func3 != 0b101) {
                        ss << (int)dec_instr.imm;
                    } else {
                        ss << (unsigned int)dec_instr.rs2;
                    }
                } else if (dec_instr.opcode == operation_code_t::LOAD) {
                    ss << mnemonic << " "
                    << get_reg_name(dec_instr.rd) << ", "
                    << (int)dec_instr.imm << "(" << get_reg_name(dec_instr.rs1) << ")";
                }
                break;
            }
            case operation_type_t::S_TYPE: {
                ss << mnemonic << " "
                << get_reg_name(dec_instr.rs2) << ", "
                << (int)dec_instr.imm << "(" << get_reg_name(dec_instr.rs1) << ")";
                break;
            }
            case operation_type_t::B_TYPE: {
                ss << mnemonic << " "
                << get_reg_name(dec_instr.rs1) << ", "
                << get_reg_name(dec_instr.rs2) << ", "
                << ((int)pc + (int)(dec_instr.imm >> 1)) * sizeof(kz::riscv::types::instr_t);
                break;
            }
            case operation_type_t::U_TYPE: {
                ss << mnemonic << " "
                << get_reg_name(dec_instr.rd) << ", "
                << (int)(dec_instr.imm << 12);
                break;
            }
            case operation_type_t::J_TYPE: {
                ss << mnemonic << " "
                << get_reg_name(dec_instr.rd) << ", "
                << ((int)pc + (int)(dec_instr.imm >> 1)) * sizeof(kz::riscv::types::instr_t);
                break;
            }
        }
        return ss.str();
    }

} /* ! kz::riscv::core ! */
