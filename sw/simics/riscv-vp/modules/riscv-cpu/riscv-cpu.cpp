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

#include <iostream>
#include <simics/cc-api.h>
#include <simics/c++/model-iface/direct-memory.h>
#include "riscv-cpu.hpp"
#include "riscv-cpu-decode.hpp"
#include "riscv-cpu-disasm.hpp"

namespace kz::riscv::core {
    riscv_cpu::riscv_cpu(simics::ConfObjectRef conf_obj): simics::ConfObject(conf_obj) {
        cobj_ = obj().object();
        regs_.fill(0);
        mstatus_ = 0;
        mepc_ = 0;
        mcause_ = 0;
        mtvec_ = 0;
        pc_ = 0;
        subsystem_ = 0;
    }

    riscv_cpu::riscv_cpu::~riscv_cpu() {}

    // FOR TESTS
    void riscv_cpu::signal_raise() {
        using instr_t = kz::riscv::types::instr_t;
        using dec_instr_t = kz::riscv::types::dec_instr_t;
        uint8* data = read_mem_(0x0, 4);
        SIM_LOG_INFO(
            1, cobj_, 0,
            "direct memory: data[0]='0x%02x', data[1]='0x%02x', data[2]='0x%02x', data[3]='0x%02x'",
            data[0], data[1], data[2], data[3]
        );
        instr_t instr = (static_cast<instr_t>(data[3]) << 24);
        instr |= (static_cast<instr_t>(data[2]) << 16);
        instr |= (static_cast<instr_t>(data[1]) << 8);
        instr |= (static_cast<instr_t>(data[0]));
        dec_instr_t dec_instr;
        SIM_LOG_INFO(1, cobj_, 0, "instr: '0x%04x'", instr);
        riscv_cpu_decoder::decode(instr, &dec_instr);
        SIM_LOG_INFO(
            1, cobj_, 0,
            "dec_instr: opcode='0x%02x', rd='0x%02x', func3='0x%01x', "
            "rs1='0x%02x', rs2='0x%02x', func7='0x%02x', type='0x%01x'",
            static_cast<unsigned int>(dec_instr.opcode), static_cast<unsigned int>(dec_instr.rd),
            static_cast<unsigned int>(dec_instr.func3), static_cast<unsigned int>(dec_instr.rs1),
            static_cast<unsigned int>(dec_instr.rs2), static_cast<unsigned int>(dec_instr.func7),
            static_cast<unsigned int>(dec_instr.type)
        );
        std::string disasm_instr = riscv_cpu_disasm::disasm(0, dec_instr);
        SIM_LOG_INFO(1, cobj_, 0, "disassembled: %s", disasm_instr.c_str());
    }

    void riscv_cpu::signal_lower() {
    }
    // END OF TESTS

    void riscv_cpu::finalize() {
    }

    void riscv_cpu::objects_finalized() {
        mem_handler_ = get_mem_handler_(0x10000000, 0x10000000);
    }
} /* ! kz::riscv::core ! */

// init_local() is called once when the device module is loaded into Simics
// It is responsible to initialize device, and register the device class in module
// The function is declared 'extern "C"' to prevent C++ name mangling
// The function is wrapped in a try/catch block to catch and report exceptions
// that may be thrown during device registration
extern "C"
void init_local() try {
    simics::make_class<kz::riscv::core::riscv_cpu>(
            "riscv_cpu",
            "a C++ device template",
            "This is a documentation string describing the"
            " riscv_cpu class.");
} catch(const std::exception& e) {
    std::cerr << e.what() << std::endl;
}
