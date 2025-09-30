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
#include <simics/base/event.h>
#include <simics/base/time.h>
#include <simics/processor/types.h>
#include <string>

namespace kz::riscv::core {
    class event {
    public:
        static constexpr int EVENT_QUEUE_END = -1;
        int next;               // index of next event, or EVENT_QUEUE_END
        simtime_t delta;        // clocks until next event
        int slot;               // determines the order when the expire time
                                // is the same for multiple events
        event_class_t *evclass; // class event belongs to
        conf_object_t *obj;     // object event operates on
        lang_void *param;       // user data
        event();
        event(simtime_t d, int s, event_class_t *ec, conf_object_t *o, lang_void *p);
        ~event();
    };
    using event_t = event;

    class event_queue {
    public:
        event_queue(const char *name);
        ~event_queue();

        void init(const char *name);
        simtime_t next(
            const event_class_t* evclass,
            const conf_object_t* obj,
            int (*pred)(lang_void* data, void* match_data),
            void* match_data);
        void remove(
            event_class_t* evclass,
            const conf_object_t* obj,
            int (*pred)(lang_void* data, void* match_data),
            void* match_data);
        void post(
            simtime_t when,
            event_class_t* evclass,
            conf_object_t* obj,
            lang_void* param);
        void rescale_time(uint64 old_freq, uint64 new_freq);
        bool add(attr_value_t *ev);
        int is_empty() const;
        void decrement(simtime_t delta);
        simtime_t get_delta() const;
        void handle_next();
        attr_value_t to_attr_list(simtime_t start) const;
        set_error_t set(attr_value_t *val);
        void clear();
    private:
        std::string name_;
        std::deque<event> events_;
    };
    using event_queue_t = event_queue;
} /* ! kz::riscv::core ! */