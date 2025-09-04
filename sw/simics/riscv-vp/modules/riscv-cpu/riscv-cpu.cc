#include <iostream>
#include <simics/cc-api.h>
#include <simics/c++/model-iface/transaction.h>
#include "include/riscv-cpu.hpp"

riscv_cpu::riscv_cpu(simics::ConfObjectRef conf_obj): simics::ConfObject(conf_obj) {
    regs.fill(0);
    pc = 0;
    mstatus = 0;
    mepc = 0;
    mcause = 0;
    mtvec = 0;
    value = 0;
}

riscv_cpu::riscv_cpu::~riscv_cpu() {}

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
        return regs[reg];
    }
    switch (reg) {
        case 32: return pc;
        case 33: return mstatus;
        case 34: return mepc;
        case 35: return mcause;
        case 36: return mtvec;
        default:
            throw std::out_of_range("Invalid register number");
    }
}

void riscv_cpu::write(int reg, uint64 val) {
    if (reg >= 0 && reg < 32) {
        if (reg != 0) { // x0 is hardwired to zero
            regs[reg] = static_cast<uint32_t>(val);
        }
        return;
    }
    switch (reg) {
        case 32: pc = static_cast<uint32_t>(val); break;
        case 33: mstatus = static_cast<uint32_t>(val); break;
        case 34: mepc = static_cast<uint32_t>(val); break;
        case 35: mcause = static_cast<uint32_t>(val); break;
        case 36: mtvec = static_cast<uint32_t>(val); break;
        default:
            throw std::out_of_range("Invalid register number");
    }
}

attr_value_t riscv_cpu::all_registers() {
    attr_value_t result = SIM_alloc_attr_dict(36 + 1);
    for (int i = 0; i < 32; ++i) {
        SIM_attr_dict_set_item(&result, i, SIM_make_attr_string(get_name(i)), SIM_make_attr_uint64(regs[i]));
    }
    SIM_attr_dict_set_item(&result, 32, SIM_make_attr_string("pc"), SIM_make_attr_uint64(pc));
    SIM_attr_dict_set_item(&result, 33, SIM_make_attr_string("mstatus"), SIM_make_attr_uint64(mstatus));
    SIM_attr_dict_set_item(&result, 34, SIM_make_attr_string("mepc"), SIM_make_attr_uint64(mepc));
    SIM_attr_dict_set_item(&result, 35, SIM_make_attr_string("mcause"), SIM_make_attr_uint64(mcause));
    SIM_attr_dict_set_item(&result, 36, SIM_make_attr_string("mtvec"), SIM_make_attr_uint64(mtvec));
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

exception_type_t
riscv_cpu::issue(transaction_t *p_trans, uint64 addr) {
    // TODO: Handle accesses to the device here
    if (SIM_transaction_is_read(p_trans)) {
        SIM_LOG_INFO(2, obj(), 0, "read from offset %lld", addr);
        SIM_set_transaction_value_le(p_trans, 0);
    } else {
        SIM_LOG_INFO(2, obj(), 0, "write to offset %lld", addr);
    }
    return Sim_PE_No_Exception;
}

// init_local() is called once when the device module is loaded into Simics
// It is responsible to initialize device, and register the device class in module
// The function is declared 'extern "C"' to prevent C++ name mangling
// The function is wrapped in a try/catch block to catch and report exceptions
// that may be thrown during device registration
extern "C" void
init_local() try
{
    simics::make_class<riscv_cpu>(
            "riscv_cpu",
            "a C++ device template",
            "This is a documentation string describing the"
            " riscv_cpu class.");
} catch(const std::exception& e) {
    std::cerr << e.what() << std::endl;
}
