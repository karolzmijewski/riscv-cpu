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

#include "riscv-cpu.hpp"


int riscv_cpu::get_number(const char *name) {
    if (strcmp(name, "pc") == 0) return 32;
    if (strcmp(name, "mstatus") == 0) return 33;
    if (strcmp(name, "mepc") == 0) return 34;
    if (strcmp(name, "mcause") == 0) return 35;
    if (strcmp(name, "mtvec") == 0) return 36;
    if (name[0] == 'x') {
        char *endptr;
        long idx = strtol(name + 1, &endptr, 10);
        if (*endptr == '\0' && idx >= 0 && idx < 32) {
            return static_cast<int>(idx);
        }
    }
    return -1;
}

const char *riscv_cpu::get_name(int reg) {
    static char buf[8];
    if (reg == 32) return "pc";
    if (reg == 33) return "mstatus";
    if (reg == 34) return "mepc";
    if (reg == 35) return "mcause";
    if (reg == 36) return "mtvec";
    if (reg >= 0 && reg < 32) {
        snprintf(buf, sizeof(buf), "x%d", reg);
        return buf;
    }
    return nullptr;
}

uint64 riscv_cpu::read(int reg) {
    if (reg >= 0 && reg < 32) {
        return regs_[reg];
    }
    switch (reg) {
        case 32: return pc_;
        case 33: return mstatus_;
        case 34: return mepc_;
        case 35: return mcause_;
        case 36: return mtvec_;
        default:
            throw std::out_of_range("Invalid register number");
    }
}

void riscv_cpu::write(int reg, uint64 val) {
    if (reg >= 0 && reg < 32) {
        if (reg != 0) { // x0 is hardwired to zero
            regs_[reg] = static_cast<uint32_t>(val);
        }
        return;
    }
    switch (reg) {
        case 32: pc_ = static_cast<uint32_t>(val); break;
        case 33: mstatus_ = static_cast<uint32_t>(val); break;
        case 34: mepc_ = static_cast<uint32_t>(val); break;
        case 35: mcause_ = static_cast<uint32_t>(val); break;
        case 36: mtvec_ = static_cast<uint32_t>(val); break;
        default:
            throw std::out_of_range("Invalid register number");
    }
}

attr_value_t riscv_cpu::all_registers() {
    attr_value_t result = SIM_alloc_attr_dict(36 + 1);
    for (int i = 0; i < 32; ++i) {
        SIM_attr_dict_set_item(&result, i, SIM_make_attr_string(get_name(i)), SIM_make_attr_uint64(regs_[i]));
    }
    SIM_attr_dict_set_item(&result, 32, SIM_make_attr_string("pc"), SIM_make_attr_uint64(pc_));
    SIM_attr_dict_set_item(&result, 33, SIM_make_attr_string("mstatus"), SIM_make_attr_uint64(mstatus_));
    SIM_attr_dict_set_item(&result, 34, SIM_make_attr_string("mepc"), SIM_make_attr_uint64(mepc_));
    SIM_attr_dict_set_item(&result, 35, SIM_make_attr_string("mcause"), SIM_make_attr_uint64(mcause_));
    SIM_attr_dict_set_item(&result, 36, SIM_make_attr_string("mtvec"), SIM_make_attr_uint64(mtvec_));
    return result;
}

int riscv_cpu::register_info(int reg, ireg_info_t info) {
    // For simplicity, we only provide basic info
    switch (info) {
        case Sim_RegInfo_Catchable:
            if (reg >= 0 && reg < 32) return 4; // x0..x31 are 32-bit
            if (reg >= 32 && reg <= 36) return 4; // pc, mstatus, mepc, mcause, mtvec are 32-bit
            return -1;
        default:
            return -1; // Unsupported info type
    }
}
