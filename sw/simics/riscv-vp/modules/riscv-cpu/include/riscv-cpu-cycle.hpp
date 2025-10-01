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

#include <simics/base/types.h>
#include <simics/base/time.h>
#include <functional>
#include <simics/base/local-time.h>
#include <simics/base/bigtime.h>
#include <simics/base/event.h>

namespace kz::riscv::core {
    class riscv_cpu_cycle {
    private:
        static constexpr unsigned long long ONE_SECOND_AS_PS = 1000000000000ULL;
    public:
        using post_cycle_callback_t = std::function<void(event_class_t*, conf_object_t*, cycles_t, void*)>;
        using find_next_cycle_callback_t = std::function<cycles_t(event_class_t*, conf_object_t*, int (*)(lang_void*, lang_void*), lang_void*)>;
        /**
         * How many picoseconds have been spend to run 'cycles' at 'frequency'
         */
        static bigtime_t cc_as_ps(cycles_t cycles, uint64_t freq);
        /**
         * How many 'cycles' can we run before passing 'time' at 'frequency'. The
         * result is always rounded down so the cycle count stops just before
         * 'time'.
         * @return true if the time could be represented as cycles, false otherwise
         */
        static bool ps_as_cc_floor(bigtime_t time, uint64_t freq, cycles_t *p_cycles);
        /**
         * How many 'cycles' should we run to approximate 'time'. The result is rounded
         * to the nearest cycle.
         * @return true if the time could be represented as cycles, false otherwise
         */
        static bool ps_as_cc_nearest(bigtime_t time, uint64_t freq, cycles_t *p_cycles);
        /**
         * Convert time offset attribute value to bigtime_t
         */
        static bigtime_t time_offset_from_attr(attr_value_t attr);
        /**
         * Convert bigtime_t time offset to attribute value
         */
        static attr_value_t time_offset_as_attr(bigtime_t time_offset);
        /**
         * Get the time in picoseconds corresponding to the given cycle count
         * and time offset at the given frequency.
         */
        static bigtime_t get_time_in_big_ps(bigtime_t time_offset, cycles_t cc, uint64_t freq);
        /**
         * Method returns the current time in picoseconds. The time is typically derived from the
         * cycle count and the CPU frequency.
         */
        static local_time_t generic_get_time_in_ps(
            conf_object_t *clock,
            bigtime_t time_offset,
            cycles_t cycle_count,
            uint64_t freq);
        /**
         * Convert local_time_t to cycles
         */
        static bool generic_cycles_from_ps(
            local_time_t when,
            bigtime_t time_offset,
            uint64 freq,
            cycles_t *cycles);
        /**
         * Convert local_time_t to delta cycles from the given cycle count
         */
        static bool generic_delta_from_ps(
            local_time_t when,
            bigtime_t time_offset,
            cycles_t cycle_count,
            uint64 freq,
            cycles_t *delta);
        /**
         * Adjust the time offset based on the given cycle count and frequency changes.
         */
        static bigtime_t adjusted_time_offset(
            bigtime_t current_offset,
            cycles_t cycle_count,
            uint64 old_freq,
            uint64 new_freq);
        /**
         * Method is used to post a cycle event after 'cycles' cycles. The event will be posted
         * after the given number of cycles has elapsed. The event will be posted
         * to the specified event class and object. The user_data parameter is a pointer
         * that will be passed to the event callback when the event is posted.
         */
        static bool posting_cycle_delta(bigtime_t ps, uint64 freq, cycles_t *delta);
        /**
         * Method checks the parameters for posting a cycle event.
         * @return NULL if the parameters are valid, or an error message otherwise
         */
        static const char *check_post_cycle_params(cycles_t cycles, event_class_t *evclass);
        /**
         * Method is used to post a time event after 'picoseconds' picoseconds. The event will be posted
         * after the given time has elapsed. The event will be posted
         * to the specified event class and object. The user_data parameter is a pointer
         * that will be passed to the event callback when the event is posted.
         */
        static const char *post_time_in_big_ps(
            conf_object_t *NOTNULL queue_obj,
            event_class_t *NOTNULL evclass,
            conf_object_t *NOTNULL poster_obj,
            bigtime_t picoseconds,
            lang_void *user_data,
            post_cycle_callback_t f,
            uint64 freq);
        /**
         * Method is used to post a time event after 'seconds' seconds. The event will be posted
         * after the given time has elapsed. The event will be posted
         * to the specified event class and object. The user_data parameter is a pointer
         * that will be passed to the event callback when the event is posted.
         */
        static const char *generic_post_time(
            conf_object_t *NOTNULL queue_obj,
            event_class_t *NOTNULL evclass,
            conf_object_t *NOTNULL poster_obj,
            double seconds,
            lang_void *user_data,
            post_cycle_callback_t f,
            uint64 freq);
        /**
         * Method is used to post a time event after 'picoseconds' picoseconds. The event will be posted
         * after the given time has elapsed. The event will be posted
         * to the specified event class and object. The user_data parameter is a pointer
         * that will be passed to the event callback when the event is posted.
         */
        static const char *generic_post_time_in_ps(
            conf_object_t *NOTNULL queue_obj,
            event_class_t *NOTNULL evclass,
            conf_object_t *NOTNULL poster_obj,
            duration_t picoseconds,
            lang_void *user_data,
            post_cycle_callback_t f,
            uint64 freq);
        /**
         * Method is used to find the next time event that has been posted with post_time_in
         * or post_time_in_ps. The method returns the time in picoseconds until the next event that matches
         * the given criteria. If no matching event is found, the method returns
         * a special value indicating that there are no more events.
         */
        static bigtime_t find_next_time_in_big_ps(
            conf_object_t *NOTNULL queue,
            event_class_t *NOTNULL evclass,
            conf_object_t *NOTNULL obj,
            int (*pred)(lang_void *data, lang_void *match_data),
            lang_void *match_data,
            find_next_cycle_callback_t f,
            uint64 freq);
        /**
         * Method is used to find the next time event that has been posted with post_time
         * or post_time_in_ps. The method returns the time in seconds until the next event that matches
         * the given criteria. If no matching event is found, the method returns
         *  a special value indicating that there are no more events.
         */
        static double generic_find_next_time(
            conf_object_t *NOTNULL queue,
            event_class_t *NOTNULL evclass,
            conf_object_t *NOTNULL obj,
            int (*pred)(lang_void *data, lang_void *match_data),
            lang_void *match_data,
            find_next_cycle_callback_t f,
            uint64 freq);
        /**
         * Method is used to find the next time event that has been posted with post_time_in_ps.
         * The method returns the time in picoseconds until the next event that matches
         * the given criteria. If no matching event is found, the method returns
         * a special value indicating that there are no more events.
         */
        static duration_t generic_find_next_time_in_ps(
            conf_object_t *NOTNULL clock,
            event_class_t *NOTNULL evclass,
            conf_object_t *NOTNULL obj,
            int (*pred)(lang_void *data, lang_void *match_data),
            lang_void *match_data,
            find_next_cycle_callback_t f,
            uint64 freq);
    };
} /* ! kz::riscv::core ! */