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
#include "riscv-cpu-decode.hpp"
#include "riscv-cpu-conf.hpp"

namespace kz::riscv::core {

    static int match_all_(lang_void *data, lang_void *match_data) {
            return 1;
    }

    static void step_event_posted_() {
        /* An event has been posted. We only single step, so we don't need to
           bother. */
    }

    pc_step_t riscv_cpu::get_step_count() {
        return current_step_;
    }

    void riscv_cpu::post_step(
        event_class_t *evclass,
        conf_object_t *obj,
        pc_step_t steps,
        lang_void *user_data) {
        // Post a step event after 'steps' instructions
        if (steps < 0) {
            SIM_LOG_ERROR(cobj_, 0, "can not post on step < 0");
            return;
        }
        step_queue_.post(steps, evclass, obj, user_data);
        step_event_posted_();
    }

    void riscv_cpu::cancel_step(
        event_class_t *evclass,
        conf_object_t *obj,
        int (*pred)(lang_void *data, lang_void *match_data),
        lang_void *match_data) {
        // Cancel step events matching the predicate
        step_queue_.remove(evclass, obj, pred == NULL ? match_all_ : pred, match_data);
    }

    pc_step_t riscv_cpu::find_next_step(
        event_class_t *evclass,
        conf_object_t *obj,
        int (*pred)(lang_void *data, lang_void *match_data),
        lang_void *match_data) {
        // Return the number of steps to the next matching event
        return step_queue_.next(evclass, obj, pred == NULL ? match_all_ : pred, match_data);
    }

    // attr_value_t riscv_cpu::events() {
    //     // Return the list of pending step events
    //     // This is a placeholder implementation
    //     return SIM_alloc_attr_list(0);
    // }

    pc_step_t riscv_cpu::advance(pc_step_t steps) {
        // Advance the CPU by 'steps' instructions
        SIM_LOG_INFO(1, cobj_, 0, "Advancing CPU by %llu steps", steps);
        /* do nothing */
        return steps;
    }
} /* ! kz::riscv::core ! */
