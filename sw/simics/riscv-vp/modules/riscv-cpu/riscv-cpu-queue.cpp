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
#include <vector>
#include <algorithm>

#include "riscv-cpu-queue.hpp"

namespace kz::riscv::core {
    Event::Event(simtime_t d, int s, event_class_t *ec, conf_object_t *o, lang_void *p)
        : next(EVENT_QUEUE_END),
          delta(d),
          slot(s),
          evclass(ec),
          obj(o),
          param(p) {}

    Event::~Event() {}

    attr_value_t Event::to_attr_val(simtime_t time) const {
        if (evclass->flags & Sim_EC_Notsaved) {
            return SIM_make_attr_invalid(); /* don't save this attribute */
        }
        if (!evclass->get_value) {
            return SIM_make_attr_invalid(); /* can't get value */
        }
        attr_value_t val = evclass->get_value(obj, param);
        return SIM_make_attr_list(
            5,
            SIM_make_attr_object(obj),
            SIM_make_attr_string(evclass->name),
            val,
            SIM_make_attr_uint64(slot),
            SIM_make_attr_uint64(time)
        );
    }

    // ! Event Queue !
    EventQueue::EventQueue(const char *name) : name_(name) {}
    EventQueue::~EventQueue() = default;

    void EventQueue::init(const char *name) { name_ = name; }

    simtime_t EventQueue::next(
        const event_class_t* evclass,
        const conf_object_t* obj,
        int (*pred)(lang_void* data, void* match_data),
        void* match_data) {
        auto it = std::find_if(events_.begin(), events_.end(),
            [&](const Event& e) {
                return (!evclass || e.evclass == evclass) &&
                       (!obj || e.obj == obj) &&
                       (!pred || pred(e.param, match_data));
            });
        return (it != events_.end()) ? it->delta : 0;
    }

    void EventQueue::remove(
        event_class_t *evclass,
        const conf_object_t *obj,
        int (*pred)(lang_void *data, lang_void *match_data),
        void *match_data) {
        auto it = std::remove_if(events_.begin(), events_.end(),
            [&](const Event& e) {
                return (!evclass || e.evclass == evclass) &&
                       (!obj || e.obj == obj) &&
                       (!pred || pred(e.param, match_data));
            });
        events_.erase(it, events_.end());
    }

    void EventQueue::post(simtime_t when, event_class_t* evclass, conf_object_t* obj, lang_void* param) {
        // slot can be used for ordering if needed, here set to 0
        events_.emplace_back(when, 0, evclass, obj, param);
    }

    void EventQueue::rescale_time(uint64 old_freq, uint64 new_freq) {
        // Step 1: Convert deltas to absolute times
        std::vector<simtime_t> abs_times;
        simtime_t total_delta = 0;
        for (const auto& e : events_) {
            total_delta += e.delta;
            abs_times.push_back(total_delta);
        }
        // Step 2: Rescale absolute times
        for (auto& t : abs_times) {
            t = static_cast<simtime_t>((double)t * new_freq / old_freq + 0.5);
        }
        // Step 3: Convert back to deltas
        simtime_t prev_time = 0;
        for (size_t i = 0; i < events_.size(); ++i) {
            simtime_t new_delta = abs_times[i] - prev_time;
            events_[i].delta = new_delta;
            prev_time = abs_times[i];
        }
    }

    bool EventQueue::add(attr_value_t *ev) {
        conf_object_t *obj;
        const char *ecname;
        attr_value_t *val;
        int64 slot;     /* ignored since it can be computed from the
                           event class */
        int64 when;
        bool ret = SIM_ascanf(ev, "osaii", &obj, &ecname, &val, &slot, &when);

        if (!ret || obj == nullptr) {
            return false;
        }
        event_class_t *evclass = SIM_get_event_class(SIM_object_class(obj), ecname);
        if (evclass == nullptr) {
            return false;
        }
        lang_void *user_data = nullptr;
        if (evclass->set_value != nullptr && !SIM_attr_is_invalid(*val)) {
            user_data = evclass->set_value(obj, *val);
            if (user_data == nullptr) {
                return false;
            }
        }
        post(when, evclass, obj, user_data);
        return true;
    }

    int EventQueue::is_empty() const {
        return events_.empty();
    }

    void EventQueue::decrement(simtime_t delta) {
        for (auto& e : events_) {
            if (e.delta > delta) {
                e.delta -= delta;
            } else {
                e.delta = 0;
            }
        }
    }

    simtime_t EventQueue::get_delta() const {
        if (events_.empty()) {
            return -1;
        }
        return events_.front().delta;
    }

    void EventQueue::handle_next() {
        if (!events_.empty()) {
            auto e = events_.front();
            events_.pop_front();
            if (e.evclass && e.evclass->callback) {
                e.evclass->callback(e.obj, e.param);
            }
        }
    }

    attr_value_t EventQueue::to_attr_list(simtime_t start) const {
        // This is a placeholder implementation
        attr_value_t ret = SIM_alloc_attr_list(events_.size());
        simtime_t t = start;
        int index = 0;
        for (const auto& e : events_) {
            t += e.delta;
            attr_value_t attr = e.to_attr_val(t);
            if (!SIM_attr_is_invalid(attr)) {
                SIM_attr_list_set_item(&ret, index++, attr);
            }
        }
        SIM_attr_list_resize(&ret, index);
        return ret;
    }

    set_error_t EventQueue::set(attr_value_t *val) {
        clear();
        for (unsigned i = 0; i < SIM_attr_list_size(*val); i++) {
            attr_value_t elem = SIM_attr_list_item(*val, i);
            if (!add(&elem)) {
                return Sim_Set_Illegal_Value;
            }
        }
        return Sim_Set_Ok;
    }

    void EventQueue::clear() {
        events_.clear();
    }
} /* ! kz::riscv::core ! */