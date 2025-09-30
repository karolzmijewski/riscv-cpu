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

#include <simics/base/types.h>
#include <simics/base/event.h>
#include <simics/base/time.h>
#include <simics/processor/types.h>
#include <string>
#include <deque>
#include <algorithm>

#include "riscv-cpu-queue.hpp"

namespace kz::riscv::core {
    // ! Event !
    event::event()
        : next(EVENT_QUEUE_END),
          delta(0),
          slot(0),
          evclass(nullptr),
          obj(nullptr),
          param(nullptr) {}

    event::event(simtime_t d, int s, event_class_t *ec, conf_object_t *o, lang_void *p)
        : next(EVENT_QUEUE_END),
          delta(d),
          slot(s),
          evclass(ec),
          obj(o),
          param(p) {}

    event::~event() {}

    // ! Event Queue !
    event_queue::event_queue(const char *name) : name_(name) {}
    event_queue::~event_queue() = default;

    void event_queue::init(const char *name) { name_ = name; }

    simtime_t event_queue::next(
        const event_class_t* evclass,
        const conf_object_t* obj,
        int (*pred)(lang_void* data, void* match_data),
        void* match_data) {
        auto it = std::find_if(events_.begin(), events_.end(),
            [&](const event& e) {
                return (!evclass || e.evclass == evclass) &&
                       (!obj || e.obj == obj) &&
                       (!pred || pred(e.param, match_data));
            });
        return (it != events_.end()) ? it->delta : 0;
    }

    void event_queue::post(simtime_t when, event_class_t* evclass, conf_object_t* obj, lang_void* param) {
        // slot can be used for ordering if needed, here set to 0
        events_.emplace_back(when, 0, evclass, obj, param);
    }

    void event_queue::rescale_time(uint64 old_freq, uint64 new_freq) {
        for (auto& e : events_) {
            e.delta = (e.delta * old_freq) / new_freq;
        }
    }

    bool event_queue::add(attr_value_t *ev) {
        // This is a placeholder implementation
        // In a real implementation, you would parse the attr_value_t
        // and add the corresponding event to the queue
        return false;
    }

    int event_queue::is_empty() const {
        return events_.empty();
    }

    void event_queue::decrement(simtime_t delta) {
        for (auto& e : events_) {
            if (e.delta > delta) {
                e.delta -= delta;
            } else {
                e.delta = 0;
            }
        }
    }

    simtime_t event_queue::get_delta() const {
        if (events_.empty()) {
            return -1;
        }
        return events_.front().delta;
    }

    void event_queue::handle_next() {
        if (!events_.empty()) {
            auto e = events_.front();
            events_.pop_front();
            if (e.evclass && e.evclass->callback) {
                e.evclass->callback(e.obj, e.param);
            }
        }
    }

    attr_value_t event_queue::to_attr_list(simtime_t start) const {
        // This is a placeholder implementation
        // In a real implementation, you would convert the events to an attr_value_t list
        return SIM_alloc_attr_list(0);
    }

    set_error_t event_queue::set(attr_value_t *val) {
        // This is a placeholder implementation
        // In a real implementation, you would parse the attr_value_t
        // and set the queue's events accordingly
        return Sim_Set_Ok;
    }

    void event_queue::clear() {
        events_.clear();
    }
} /* ! kz::riscv::core ! */