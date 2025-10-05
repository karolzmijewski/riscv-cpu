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
#include "riscv-cpu-cycle.hpp"
#include "riscv-cpu-queue.hpp"
#include <simics/base/time.h>
#include <simics/base/local-time.h>

namespace kz::riscv::core {

    static int match_all_(lang_void *data, lang_void *match_data) {
            return 1;
    }

    static void cycle_event_posted_() {
        // An event has been posted. We only single step, so we don't need to bother.
    }

    cycles_t riscv_cpu::get_cycle_count() {
        return current_cycle_;
    }

    double riscv_cpu::get_time() {
        return local_time_as_sec(get_time_in_ps());
    }

    cycles_t riscv_cpu::cycles_delta(double when) {
        return cycles_delta_from_ps(local_time_from_sec(cobj_, when));
    }

    uint64_t riscv_cpu::get_frequency() {
        return freq_hz_;
    }

    void riscv_cpu::post_cycle(
        event_class_t *evclass,
        conf_object_t *obj,
        cycles_t cycles,
        lang_void *user_data) {
        // Post a cycle event after 'cycles' cycles
        const char *err = riscv_cpu_cycle::check_post_cycle_params(cycles, evclass);
        if (err) {
            SIM_LOG_ERROR(cobj_, 0, "%s", err);
            return;
        }
        cycle_queue_.post(cycles, evclass, obj, user_data);
        cycle_event_posted_();
    }

    void riscv_cpu::post_time(
        event_class_t *evclass,
        conf_object_t *obj,
        double seconds,
        lang_void *user_data) {
        // Post a time event after 'seconds' seconds
        const char *err = riscv_cpu_cycle::generic_post_time(
            cobj_, evclass, obj, seconds, user_data,
            [this](event_class_t* evclass, conf_object_t* obj, cycles_t cycles, void* user_data) {
                this->post_cycle(evclass, obj, cycles, user_data);
            }, freq_hz_
        );
        if (err) {
                SIM_LOG_ERROR(cobj_, 0, "%s", err);
        }
        cycle_event_posted_();
    }

    void riscv_cpu::cancel(
        event_class_t *evclass,
        conf_object_t *obj,
        int (*pred)(lang_void *data, lang_void *match_data), lang_void *match_data) {
        // Cancel events matching the predicate
        cycle_queue_.remove(evclass, obj, pred == NULL ? match_all_ : pred, match_data);
    }

    cycles_t riscv_cpu::find_next_cycle(
        event_class_t *evclass,
        conf_object_t *obj,
        int (*pred)(lang_void *data, lang_void *match_data),
        lang_void *match_data) {
        // Return the number of cycles to the next matching event
        return cycle_queue_.next(evclass, obj, pred == NULL ? match_all_ : pred, match_data);
    }

    double riscv_cpu::find_next_time(
        event_class_t *evclass,
        conf_object_t *obj,
        int (*pred)(lang_void *data, lang_void *match_data),
        lang_void *match_data) {
        // Return the time to the next matching event
        return riscv_cpu_cycle::generic_find_next_time(
            cobj_, evclass, obj, pred, match_data,
            [this](event_class_t *evclass, conf_object_t *obj, int (*pred)(lang_void*, lang_void*), lang_void *match_data) -> cycles_t {
                return this->find_next_cycle(evclass, obj, pred, match_data);
            }, freq_hz_);
    }

    attr_value_t riscv_cpu::events() {
        // Return the list of pending cycle events
        attr_value_t evs = SIM_alloc_attr_list(static_cast<int>(cycle_queue_.get_events().size()));
        cycles_t t = 0;
        for (size_t i = 0; i < cycle_queue_.get_events().size(); ++i) {
            const auto& e = cycle_queue_.get_events()[i];
            // Get event description if available
            std::string desc;
            if (e.evclass && e.evclass->describe) {
                char* d = e.evclass->describe(e.obj, e.param);
                if (d) {
                    desc = d;
                    MM_FREE(d);
                }
            }
            t += e.delta;
            SIM_attr_list_set_item(&evs, static_cast<int>(i),
                SIM_make_attr_list(
                    4,
                    SIM_make_attr_object(e.obj),
                    SIM_make_attr_string(e.evclass ? e.evclass->name : ""),
                    SIM_make_attr_uint64(t),
                    SIM_make_attr_string(desc.c_str())
                )
            );
        }
        return evs;
    }

    local_time_t riscv_cpu::get_time_in_ps() {
        return riscv_cpu_cycle::generic_get_time_in_ps(cobj_, time_offset_, current_cycle_, freq_hz_);
    }

    cycles_t riscv_cpu::cycles_delta_from_ps(local_time_t when) {
        cycles_t delta;
        if (!riscv_cpu_cycle::generic_delta_from_ps(when, time_offset_, current_cycle_, freq_hz_, &delta)) {
                char r[LOCAL_TIME_STR_MAX_SIZE];
                local_time_as_string(when, r);
                SIM_LOG_ERROR(
                    cobj_, 0,
                    "cycles_delta_from_ps: time delta to %s is too big to be represented in cycles", r
                );
                return -1;
        } else {
            return delta;
        }
    }

    void riscv_cpu::post_time_in_ps(
        event_class_t *evclass,
        conf_object_t *obj,
        duration_t ps,
        lang_void *user_data) {
        // Post a time event after 'picoseconds' picoseconds
        const char *err = riscv_cpu_cycle::generic_post_time_in_ps(
            cobj_, evclass, obj, ps, user_data,
            [this](event_class_t* evclass, conf_object_t* obj, cycles_t cycles, void* user_data) {
                this->post_cycle(evclass, obj, cycles, user_data);
            }, freq_hz_);
        if (err) {
            SIM_LOG_ERROR(cobj_, 0, "%s", err);
        }
        cycle_event_posted_();
    }

    duration_t riscv_cpu::find_next_time_in_ps(
        event_class_t *evclass,
        conf_object_t *obj,
        int (*pred)(lang_void *data, lang_void *match_data),
        lang_void *match_data) {
        return riscv_cpu_cycle::generic_find_next_time_in_ps(
            cobj_, evclass, obj, pred, match_data,
            [this](
                event_class_t *evclass,
                conf_object_t *obj,
                int (*pred)(lang_void*, lang_void*),
                lang_void *match_data) -> cycles_t {
                return this->find_next_cycle(evclass, obj, pred, match_data);
            }, freq_hz_);
    }
}; /* ! kz::riscv::core ! */