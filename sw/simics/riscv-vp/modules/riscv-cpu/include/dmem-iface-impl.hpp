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

#include <simics/c++/model-iface/direct-memory.h>

namespace kz::riscv::cpu::iface {
    class dmem_iface_impl: public simics::iface::DirectMemoryLookupV2Interface {
    public:
        /**
         * It is called on the memory space object. The lookup method traces the range specified
         * by addr and size through memory spaces and translators until a direct-memory object
         * is reached. The direct-memory object is returned in the target field and the offset
         * into this object corresponding to addr is returned in the offs field.
         * The call to lookup fails if the specified range does not map continuously to a
         * direct-memory object. A lookup failure is indicated by returning NULL in the target field.
         * target filed is a part of direct_memory_lookup_t structure.
         * Once a direct-memory object has been found, the direct_memory interface can be used to
         * obtain a direct pointer to the contents of the direct-memory object.
         * @param transaction The transaction that is being performed. This can be used to
         *     get additional information about the transaction.
         * @param addr The physical address to look up.
         * @param access The type of access being performed (read, write, execute). All specified
         *     access types must reach the same direct-memory object and range for the lookup to succeed.
         *     Note that the actual access permissions needed to access the real data must be requested from
         *     the direct-memory object using the request method of the direct_memory interface.
         * @return A structure containing the target direct-memory object, offset, and access information.
         */
        direct_memory_lookup_t lookup(transaction_t *transaction, physical_address_t addr, access_t access) override;
        /**
         * Reads memory directly from the specified physical address into the provided buffer.
         * This function is called to perform a direct memory read operation.
         * It returns the number of bytes successfully read.
         */
        size_t read_memory(uint64 addr, void *buf, size_t size);
        /**
         * Writes memory directly to the specified physical address from the provided buffer.
         * This function is called to perform a direct memory write operation.
         * It returns the number of bytes successfully written.
         */
        size_t write_memory(uint64 addr, const void *buf, size_t size);
    };
} // namespace kz::riscv::cpu::iface