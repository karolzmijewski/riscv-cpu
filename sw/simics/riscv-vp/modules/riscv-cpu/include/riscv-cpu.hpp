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

#include <array>
#include <cstdint>
#include <simics/cc-api.h>
#include <simics/c++/model-iface/transaction.h>
#include <simics/c++/model-iface/int-register.h>

class riscv_cpu:
    public simics::ConfObject,
    public simics::iface::TransactionInterface,
    public simics::iface::IntRegisterInterface {
private:
    std::array<uint32_t, 32> regs; // x0..x31
    uint32_t pc;
    uint32_t mstatus, mepc, mcause, mtvec;
public:
    explicit riscv_cpu(simics::ConfObjectRef conf_obj);
    virtual ~riscv_cpu();

    // INTERFACES
    // IntRegisterInterface:
    // The int_register interface is used for access to registers in a processor.
    // It can be used to access any kind of integer register, not only the "normal"
    // registers. This includes all kinds of control registers, hidden registers
    // and anything else that might be useful to access as a register. The only
    // limitation is that the register value should be representable as a 64-bit
    // unsigned integer.
    /**
     * Translates a register name to its number. Returns -1 if the register does not exist.
     */
    int get_number(const char *name) override;
    /**
     * Translates a register number to its canonical name.
     */
    const char *get_name(int reg) override;
    /**
     * Reads a register value.
     */
    virtual uint64 read(int reg) override;
    /**
     * Writes a new register value.
     */
    virtual void write(int reg, uint64 val) override;
    /**
     * Returns a list of all register numbers that can be used for this object.
     */
    virtual attr_value_t all_registers() override;
    /**
     * returns information about a single register. The information return depends on the info parameter
     */
    virtual int register_info(int reg, ireg_info_t info) override;
    // TransactionInterface:
    exception_type_t issue(transaction_t *p_trans, uint64 addr) override;

    // ATTRIBUTES:
    // - specify configuration parameters
    // - saving and restoring state
    // - inspect and control the state of model 
    // - DO NOT USE to communicate simulation info between objects after init
    int value;

    static void init_class(simics::ConfClass *cls) {
        // Transaction interface is required for memory-mapped devices
        // It is a recommended interface for all memory operation replacing old io_memory interface
        cls->add(simics::iface::TransactionInterface::Info());
        // IntRegister interface is used to expose CPU registers to debugger and other tools
        cls->add(simics::iface::IntRegisterInterface::Info());
        cls->add(
            simics::Attribute(
                "value", "i", "A value.",
                ATTR_CLS_VAR(riscv_cpu, value)
            )
        );
    }
};