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
#include "riscv-cpu-queue.hpp"
#include <simics/processor-api.h>

namespace kz::riscv::core {

    void riscv_cpu::run() {
        // Called by Simics to start execution.
        state_ = execute_state_t::Running;

        // Check event queue for pending events, timers, etc. on current step and cycle
        handle_events_(&cycle_queue_);
        if (is_enabled_) {
            handle_events_(&step_queue_);
        }

        while (state_ == execute_state_t::Running) {
            if (is_enabled_ && stall_cycles_ == 0) {
                SIM_LOG_INFO(4, cobj_, 0, "Start execution");
                // Fetch instruction at PC
                // Assuming 4-byte instructions (RV32I, without C extension, for compressed instructions)
                instr_t instr = fetch_(pc_);
                // Decode and execute instruction
                dec_instr_t dec_instr = decode_(instr);
                // Execute one instruction
                execute_(dec_instr);
                SIM_LOG_INFO(4, cobj_, 0, "Stop execution");
            } else {
                // If the processor is disabled, we can either halt or just wait
                simtime_t delta = cycle_queue_.get_delta();
                if (stall_cycles_ != 0) {
                    if (delta > stall_cycles_) {
                        delta = stall_cycles_;
                    }
                    stall_cycles_ -= delta;
                    total_stall_cycles_ += delta;
                }
                if (delta > 0) {
                    inc_cycles_(delta);
                }
            }
            if (state_ == execute_state_t::Stopped) {
                SIM_LOG_INFO(4, cobj_, 0, "End processing\n");
                break;
            }
            // Check for interrupts, events, etc.
            handle_events_(&cycle_queue_);
            if (is_enabled_) {
                handle_events_(&step_queue_);
            }
        }
    }

    void riscv_cpu::stop() {
        // Called by Simics to stop execution.
        state_ = execute_state_t::Stopped;
        VT_stop_event_processing(cobj_);
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
