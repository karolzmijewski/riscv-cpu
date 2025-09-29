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

namespace kz::riscv::core {
    void riscv_cpu::run() {
        // Called by Simics to start execution.
        running_ = true;
        // Optionally, schedule the first step or event if needed.
    }

    void riscv_cpu::stop() {
        // Called by Simics to stop execution.
        running_ = false;
    }

    void riscv_cpu::switch_in() {
        // Called when this CPU becomes the active one in the simulation.
        // Set up state or resources here if needed.
    }

    void riscv_cpu::switch_out() {
        // Called when this CPU is no longer the active one.
        // Clean up or save state if needed.
    }
} /* ! kz::riscv::core ! */
