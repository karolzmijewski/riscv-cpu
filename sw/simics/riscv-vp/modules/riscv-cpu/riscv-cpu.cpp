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
