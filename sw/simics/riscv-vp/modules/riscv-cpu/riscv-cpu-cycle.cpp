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

#include "riscv-cpu-cycle.hpp"

namespace kz::riscv::core {
    bigtime_t RiscvCpuCycle::cc_as_ps(cycles_t cycles, uint64_t freq) {
        // No risk of overflow: ONE_SECOND_AS_PS is about 2^40ps, cycles fits in uint64
        return bigtime_div_floor(
            bigtime_mul(bigtime_from_ps_lo(cycles), ONE_SECOND_AS_PS),
            static_cast<int64_t>(freq)
        );
    }

    bool RiscvCpuCycle::ps_as_cc_floor(bigtime_t time, uint64_t freq, cycles_t* p_cycles) {
    const auto result = bigtime_div_floor(
        bigtime_mul(time, static_cast<int64_t>(freq)),
        ONE_SECOND_AS_PS
    );
    *p_cycles = static_cast<cycles_t>(bigtime_as_ps_lo(result));
    // Ensure the result fits in cycles_t
    return bigtime_as_ps_hi(result) == 0 || bigtime_as_ps_hi(result) == -1;
}

    bool RiscvCpuCycle::ps_as_cc_nearest(bigtime_t time, uint64_t freq, cycles_t *p_cycles) {
        // Round to nearest cycle, rounding up if exactly half-way rather than
        // rounding to an even cycle - event posting is relative and rounding
        // should not depend on the parity of the current cycle.
        bigtime_t result = bigtime_div_floor(
            bigtime_add(
                bigtime_mul(time, (int64)freq),
                duration_from_ps(ONE_SECOND_AS_PS / 2)),
            ONE_SECOND_AS_PS);
        uint64_t lo = bigtime_as_ps_lo(result);
        *p_cycles = static_cast<cycles_t>(lo);
        // make sure the result fits in cycles_t
        return (bigtime_as_ps_hi(result) ==  0 && (lo >> 63) == 0)
            || (bigtime_as_ps_hi(result) == -1 && (lo >> 63) == 1);
    }

    bigtime_t RiscvCpuCycle::time_offset_from_attr(attr_value_t attr) {
        if (SIM_attr_is_floating(attr)) {
            return bigtime_from_sec(SIM_attr_floating(attr));
        } else {
            return bigtime_from_attr(attr);
        }
    }

    attr_value_t RiscvCpuCycle::time_offset_as_attr(bigtime_t time_offset) {
        return bigtime_to_attr(time_offset);
    }

    bigtime_t RiscvCpuCycle::get_time_in_big_ps(bigtime_t time_offset, cycles_t cc, uint64_t freq) {
        if (cc && !freq) {
            return (bigtime_t) BIGTIME_ILLEGAL;
        }
        bigtime_t t = RiscvCpuCycle::cc_as_ps(cc, freq);
        bigtime_t r = bigtime_add_bigtime(time_offset, (freq) ? t : (bigtime_t) BIGTIME_ZERO);
        return r;
    }

    local_time_t RiscvCpuCycle::generic_get_time_in_ps(
        conf_object_t *clock,
        bigtime_t time_offset,
        cycles_t cycle_count,
        uint64_t freq)
    {
        return local_time_from_bigtime(clock, get_time_in_big_ps(time_offset, cycle_count, freq));
    }

    bool RiscvCpuCycle::generic_cycles_from_ps(
        local_time_t when,
        bigtime_t time_offset,
        uint64_t freq,
        cycles_t *cycles)
    {
        return ps_as_cc_floor(
            bigtime_sub_bigtime(
                local_time_as_bigtime(when), 
                time_offset),
            freq, cycles
        );
    }

    bool RiscvCpuCycle::generic_delta_from_ps(
        local_time_t when,
        bigtime_t time_offset,
        cycles_t cycle_count,
        uint64_t freq,
        cycles_t *delta)
    {
        cycles_t cycles;
        bool ret = generic_cycles_from_ps(when, time_offset, freq, &cycles);
        *delta = cycles - cycle_count;
        return ret;
    }

    bigtime_t RiscvCpuCycle::adjusted_time_offset(
        bigtime_t current_offset,
        cycles_t cycle_count,
        uint64_t old_freq,
        uint64_t new_freq)
    {
        bigtime_t current_time = get_time_in_big_ps(current_offset, cycle_count, old_freq);
        bigtime_t raw_cc_time = get_time_in_big_ps((bigtime_t) BIGTIME_ZERO, cycle_count, new_freq);
        return bigtime_sub_bigtime(current_time, raw_cc_time);
    }

    bool RiscvCpuCycle::posting_cycle_delta(bigtime_t ps, uint64_t freq, cycles_t *delta) {
        cycles_t result;
        if (!ps_as_cc_nearest(ps, freq, &result)) {
            return false;
        }
        *delta = (result <= 0) ? 1 : result;
        return true;
    }

    const char *RiscvCpuCycle::check_post_cycle_params(cycles_t cycles, event_class_t *evclass) {
        if (cycles < 0) {
            return "posting on negative time";
        } else if (evclass->flags & Sim_EC_Machine_Sync) {
            return  "machine sync event can not be posted directly on a cpu; use"
                " SIM_event_post_time() or SIM_event_post_cycle() instead.";
        } else {
            return nullptr;
        }
    }

    const char *RiscvCpuCycle::post_time_in_big_ps(
        conf_object_t *NOTNULL queue_obj,
        event_class_t *NOTNULL evclass,
        conf_object_t *NOTNULL poster_obj,
        bigtime_t picoseconds,
        lang_void *user_data,
        post_cycle_callback_t f,
        uint64 freq)
    {
        if (bigtime_lt(picoseconds, (bigtime_t) BIGTIME_ZERO)) {
            return "posting on negative time";
        }
        cycles_t delta;
        if (!posting_cycle_delta(picoseconds, freq, &delta)) {
            return "posting too far ahead";
        }
        f(evclass, poster_obj, delta, user_data);
        return nullptr;
    }

    const char *RiscvCpuCycle::generic_post_time(
        conf_object_t *NOTNULL queue_obj,
        event_class_t *NOTNULL evclass,
        conf_object_t *NOTNULL poster_obj,
        double seconds,
        lang_void *user_data,
        post_cycle_callback_t f,
        uint64 freq)
    {
        bigtime_t ps = bigtime_from_sec(seconds);
        if (bigtime_is_illegal(ps)) {
            return  "post_time(): posting time can not be represented in simulation time";
        }
        return post_time_in_big_ps(queue_obj, evclass, poster_obj, ps, user_data, f, freq);
    }

    const char *RiscvCpuCycle::generic_post_time_in_ps(
        conf_object_t *NOTNULL queue_obj,
        event_class_t *NOTNULL evclass,
        conf_object_t *NOTNULL poster_obj,
        duration_t picoseconds,
        lang_void *user_data,
        post_cycle_callback_t f,
        uint64 freq)
    {
        return post_time_in_big_ps(
            queue_obj, evclass, poster_obj, 
            bigtime_from_ps_lo(duration_as_ps(picoseconds)),
            user_data, f, freq
        );
    }

    bigtime_t RiscvCpuCycle::find_next_time_in_big_ps(
        conf_object_t *NOTNULL queue,
        event_class_t *NOTNULL evclass,
        conf_object_t *NOTNULL obj,
        int (*pred)(lang_void *data, lang_void *match_data),
        lang_void *match_data,
        find_next_cycle_callback_t f,
        uint64 freq)
    {
        cycles_t cycles = f(evclass, obj, pred, match_data);
        return cc_as_ps(cycles, freq);
    }

    double RiscvCpuCycle::generic_find_next_time(
        conf_object_t *NOTNULL queue,
        event_class_t *NOTNULL evclass,
        conf_object_t *NOTNULL obj,
        int (*pred)(lang_void *data, lang_void *match_data),
        lang_void *match_data,
        find_next_cycle_callback_t f,
        uint64 freq)
    {
        bigtime_t ps = find_next_time_in_big_ps(queue, evclass, obj, pred, match_data, f, freq);
        if (bigtime_is_illegal(ps) || bigtime_lt(ps, BIGTIME_ZERO)) {
            return -1.0;
        } else {
            return bigtime_as_sec(ps);
        }
    }

    duration_t RiscvCpuCycle::generic_find_next_time_in_ps(
        conf_object_t *NOTNULL clock,
        event_class_t *NOTNULL evclass,
        conf_object_t *NOTNULL obj,
        int (*pred)(lang_void *data, lang_void *match_data),
        lang_void *match_data,
        find_next_cycle_callback_t f,
        uint64 freq)
    {
        bigtime_t ps = find_next_time_in_big_ps(clock, evclass, obj, pred, match_data, f, freq);
        if (bigtime_is_illegal(ps)
            || bigtime_lt(ps, BIGTIME_ZERO)
            || bigtime_as_ps_hi(ps) != 0 
            || bigtime_as_ps_lo(ps) > 0x7fffffffffffffff) {
            return ILLEGAL_DURATION;
        } else {
            return duration_from_ps(static_cast<int64_t>(bigtime_as_ps_lo(ps)));
        }
    }
} /* ! kz::riscv::core ! */