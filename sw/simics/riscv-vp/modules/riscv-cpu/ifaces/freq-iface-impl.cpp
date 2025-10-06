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

namespace kz::riscv::core {
    void RiscvCpu::set(uint64 numerator, uint64 denominator) {
        uint64_t old_freq = freq_hz_;
        uint64_t new_freq = 1;
        if (numerator == 0 || denominator == 0) {
            SIM_LOG_ERROR(
                cobj_, 0,
                "Got invalid frequency from bus, setting to 1 Hz"
            );
        } else {
            new_freq = (numerator + (denominator / 2)) / denominator;
            if (new_freq == 0) {
                /* very low frequency, round up to 1 Hz */
                new_freq = 1;
            }
        }
        VT_clock_frequency_about_to_change(cobj_);
        if (old_freq != 0) {
            time_offset_ = RiscvCpuCycle::adjusted_time_offset(
                time_offset_,
                current_cycle_,
                old_freq,
                new_freq
            );
            ASSERT(!bigtime_is_illegal(time_offset_));
            cycle_queue_.rescale_time(old_freq, new_freq);
        }
        freq_hz_ = new_freq;
        VT_clock_frequency_change(cobj_, freq_hz_);
    }
} // namespace kz::riscv::core