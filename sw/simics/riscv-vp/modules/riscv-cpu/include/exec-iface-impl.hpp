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

#include <simics/c++/model-iface/execute.h>

namespace kz::riscv::cpu::iface {
    class exec_iface_impl: public simics::iface::ExecuteInterface {
    public:
        /**
         * Starts execution of the CPU. This function is called to start/restart the CPU executing.
         */
        void run() override;
        /**
         * Stops execution of the CPU. This function is called to stop the CPU from executing.
         * Having received stop, the processor model must stop the simulation as soon as possible
         * and return to the Simics core. This is typically done as soon as the currently
         * executing instruction is committed.
         */
        void stop() override;
        /**
         * Called when this CPU is switched in (becomes the current CPU).
         * It means that if gain control of the simulation from some other execute object
         * in the cell. This can be used to perform any necessary setup before the CPU
         * starts executing. For example, it can be used to set up the CPU state or to
         * allocate resources. This function is called from the simulation thread.
         * Note that this function may be called multiple times if the CPU is switched in
         * and out multiple times.
         * It is guaranteed that switch_out() will be called before switch_in() is called again.
         */
        void switch_in() override;
        /**
         * Called when this CPU is switched out (loses control of the simulation).
         * It means that some other execute object in the cell is taking control of the simulation.
         */
        void switch_out() override;
    };
} // namespace kz::riscv::cpu::iface
