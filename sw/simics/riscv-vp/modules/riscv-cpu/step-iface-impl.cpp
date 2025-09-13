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

    pc_step_t riscv_cpu::get_step_count() {
        return 1; // Each step corresponds to one instruction
    }

    void riscv_cpu::post_step(
        event_class_t *evclass,
        conf_object_t *obj,
        pc_step_t steps,
        lang_void *user_data) {
        // Post a step event after 'steps' instructions
        // This is a placeholder implementation
    }

    void riscv_cpu::cancel_step(
        event_class_t *evclass,
        conf_object_t *obj,
        int (*pred)(lang_void *data, lang_void *match_data),
        lang_void *match_data) {
        // Cancel step events matching the predicate
        // This is a placeholder implementation
    }

    pc_step_t riscv_cpu::find_next_step(
        event_class_t *evclass,
        conf_object_t *obj,
        int (*pred)(lang_void *data, lang_void *match_data),
        lang_void *match_data) {
        // Find the next step event matching the predicate
        // This is a placeholder implementation
        return 0; // No matching step event found
    }

    attr_value_t riscv_cpu::events() {
        // Return the list of pending step events
        // This is a placeholder implementation
        return SIM_alloc_attr_list(0);
    }

    pc_step_t riscv_cpu::advance(pc_step_t steps) {
        // Advance the CPU by 'steps' instructions
        // This is a placeholder implementation
        return steps;
    }
} /* ! kz::riscv::core ! */
