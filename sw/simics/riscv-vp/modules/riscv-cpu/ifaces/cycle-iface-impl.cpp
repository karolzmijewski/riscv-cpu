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
#include <simics/base/time.h>

namespace kz::riscv::core {
    cycles_t riscv_cpu::get_cycle_count() {
        return current_cycle_;
    }

    double riscv_cpu::get_time() {
        return 0.0;
    }

    cycles_t riscv_cpu::cycles_delta(double when) {
        return 0;
    }

    uint64_t riscv_cpu::get_frequency() {
        return 0;
    }

    void riscv_cpu::post_cycle(
        event_class_t *evclass,
        conf_object_t *obj,
        cycles_t cycles,
        lang_void *user_data) {
        // Post a cycle event after 'cycles' cycles
    }

    void riscv_cpu::post_time(
        event_class_t *evclass,
        conf_object_t *obj,
        double seconds,
        lang_void *user_data) {
        // Post a time event after 'seconds' seconds
    }

    void riscv_cpu::cancel(
        event_class_t *evclass,
        conf_object_t *obj,
        int (*pred)(lang_void *data, lang_void *match_data), lang_void *match_data) {
        // Cancel events matching the predicate
    }

    cycles_t riscv_cpu::find_next_cycle(
        event_class_t *evclass,
        conf_object_t *obj,
        int (*pred)(lang_void *data, lang_void *match_data),
        lang_void *match_data) {
        // Return the number of cycles to the next matching event
        return 0;
    }

    double riscv_cpu::find_next_time(
        event_class_t *evclass,
        conf_object_t *obj,
        int (*pred)(lang_void *data, lang_void *match_data),
        lang_void *match_data) {
        // Return the time to the next matching event
        return 0.0;
    }

    attr_value_t riscv_cpu::events() {
        // Return the list of pending cycle events
        return SIM_alloc_attr_list(0);
    }

    local_time_t riscv_cpu::get_time_in_ps() {
        local_time_t lt;
        lt.clock = cobj_;
        lt.t = {}; // zero-initialize lt.t according to its type
        return lt;
    }

    cycles_t riscv_cpu::cycles_delta_from_ps(local_time_t when) {
        return 0;
    }

    void riscv_cpu::post_time_in_ps(
        event_class_t *evclass,
        conf_object_t *obj,
        duration_t picoseconds,
        lang_void *user_data) {
        // Post a time event after 'picoseconds' picoseconds
    }

    duration_t riscv_cpu::find_next_time_in_ps(
        event_class_t *evclass,
        conf_object_t *obj,
        int (*pred)(lang_void *data, lang_void *match_data),
        lang_void *match_data) {
        return {};
    }
}; /* ! kz::riscv::core ! */