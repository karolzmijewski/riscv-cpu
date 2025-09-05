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

#pragma once

#include <array>
#include <cstdint>
#include <simics/cc-api.h>

#include "reg-iface-impl.hpp"
#include "exec-iface-impl.hpp"
#include "dmem-iface-impl.hpp"

class riscv_cpu:
    public simics::ConfObject,
    public kz::riscv::cpu::iface::reg_iface_impl,
    public kz::riscv::cpu::iface::exec_iface_impl,
    public kz::riscv::cpu::iface::dmem_iface_impl {
public:
    explicit riscv_cpu(simics::ConfObjectRef conf_obj);
    virtual ~riscv_cpu();

    static void init_class(simics::ConfClass *cls) {
        // IntRegister interface is used to expose CPU registers to debugger and other tools
        cls->add(kz::riscv::cpu::iface::reg_iface_impl::Info());
        // Execute interface is used to control execution of the CPU
        cls->add(kz::riscv::cpu::iface::exec_iface_impl::Info());
        // ProcessorInfo interface is used to provide information about the CPU architecture
        cls->add(kz::riscv::cpu::iface::proc_iface_impl::Info());
        // Attributes
        cls->add(
            simics::Attribute(
                "phys_mem", "o", "Physical memory space.",
                ATTR_CLS_VAR(riscv_cpu, phys_mem)
            )
        );
    }
};