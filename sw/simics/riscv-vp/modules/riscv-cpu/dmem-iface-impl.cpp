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
    direct_memory_lookup_t riscv_cpu::get_mem_handler_(physical_address_t addr, unsigned size) {
        direct_memory_lookup_t dml = phys_mem_.iface().lookup(cobj_, addr, size, Sim_Access_Read);
        assert(dml.target == nullptr);
        SIM_LOG_INFO(
            1, cobj_, 0,
            "direct memory lookup: target='%s', offs='%llu', access='%d'",
            SIM_object_name(dml.target), dml.offs, dml.access
        );
        return dml;
    }

    uint8 *riscv_cpu::read_mem_(uint32_t offset, unsigned size) {
        simics::Connect<simics::iface::DirectMemoryInterface> dm_iface;
        // set proper memory target for direct memory access interface
        dm_iface.set(mem_handler_.target);
        // The get_handle method is used by a memory user (cpu) to create or retrieve a handle to the
        // memory region starting at offset - "offs" with size - "size", it's unique for requestor
        // representec by "cobj" reference and subsystem id.
        direct_memory_handle_t mem_handler = dm_iface.iface().get_handle(cobj_, subsystem_, offset, size);
        direct_memory_t dm = dm_iface.iface().request(mem_handler, Sim_Access_Read, Sim_Access_Write);
        assert(dm.data == nullptr);
        return dm.data;
    }

    void riscv_cpu::release(
        conf_object_t *target,
        direct_memory_handle_t handle,
        direct_memory_ack_id_t id) {
        // TBD
    }

    void riscv_cpu::update_permission(
        conf_object_t *target,
        direct_memory_handle_t handle,
        access_t lost_access,
        access_t lost_permission,
        access_t lost_inhibit,
        direct_memory_ack_id_t id) {
        // TBD
    }

    void riscv_cpu::conflicting_access(
        conf_object_t *target,
        direct_memory_handle_t handle,
        access_t conflicting_permission,
        direct_memory_ack_id_t id) {
        // TBD
    }
}
