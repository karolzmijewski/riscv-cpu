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

#include <simics/c++/model-iface/execute.h>
#include <simics/c++/model-iface/int-register.h>
#include <simics/c++/model-iface/processor-info.h>
#include <simics/c++/model-iface/direct-memory.h>
#include <simics/c++/model-iface/step.h>

#include "riscv-cpu-types.hpp"
#include "riscv-cpu-conf.hpp"

namespace kz::riscv::core {
    class riscv_cpu:
        public simics::ConfObject,
        public simics::iface::IntRegisterInterface,
        public simics::iface::StepInterface,
        public simics::iface::ExecuteInterface,
        public simics::iface::ProcessorInfoInterface,
        public simics::iface::ProcessorInfoV2Interface,
        public simics::iface::ProcessorCliInterface,
        public simics::iface::DirectMemoryUpdateInterface {
    private:
        // types
        using addr_t = kz::riscv::types::addr_t;
        using instr_t = kz::riscv::types::instr_t;
        using dec_instr_t = kz::riscv::types::dec_instr_t;
        // attributes
        conf_object_t *cobj_;
        uint64_t subsystem_;
        std::array<uint32_t, RV32I_GP_REG_NUM> regs_; // x0..x31
        uint32_t pc_;
        uint32_t mstatus_, mepc_, mcause_, mtvec_;
        direct_memory_lookup_t mem_handler_;
        simics::Connect<simics::iface::DirectMemoryLookupInterface> phys_mem_;
        uint64_t steps_in_quantum_;
        // methods
        direct_memory_lookup_t get_mem_handler_(physical_address_t addr, unsigned size);
        uint8 *read_mem_(addr_t addr, unsigned size);
        inline uint32_t read_reg_(int reg);
        inline void write_reg_(int reg, uint32_t value);
        instr_t fetch_(addr_t addr);
        dec_instr_t decode_(instr_t instr);
        void execute_(dec_instr_t dec_instr);
        // state
        bool running_;
    public:
        explicit riscv_cpu(simics::ConfObjectRef conf_obj);
        virtual ~riscv_cpu();

        /**
         * Method is called after init_local and class constructor, it is intended to do any
         * initialization that require the attribute values, however should avoid calling any
         * interface methods on ther objects.
         */
        void finalize() override;

        /**
         * Method is called at the end on initialization phases when objects are fully
         * constructed and can participate in the simulation and reverse execution.
         * There object can call interface methods on other objects as a part of last pahse
         * of initialization process
         */
        void objects_finalized() override;

        // ! ProcessorCliInterface (proc-cli-iface-impl) !
        /**
         * Method is used for disassemble command as well as to disassemble the next
         * instruction to be executed, when control is retirned to the CLI prompt.
         * @param add_prefix selects the address type of the address paramater, whether
         *     it is a physical address ("p"), a linear address ("l"), or virtual address ("v"),
         *     just as returned from "get_address_prefix"
         * @param address the program counter for instruction to disassemble
         * @param print_cpu if non-zero, then the name of the processor should be included
         *     first in disassembly line.
         * @param mnemonic is used to print exception or interrupt information as returned by the
         *    "get_pending_exception_string" function. If it is not NULL, then it is the mnemonic
         *    to be printed instead of the disassembled instruction.
         * @return a tuple with length of instruction in bytes ad the disassembly string.
         */
        tuple_int_string_t get_disassembly(
            const char *addr_prefix,
            generic_address_t address,
            bool print_cpu,
            const char *mnemonic) override;
        /**
         * Method returnes the string to output in the CLI for the "print-processor-registers"
         * @param all flag correcponding to the -all switch to the print-processor-registers command.
         * @return string with registers info.
         */
        char *get_pregs(bool all) override;
        /**
         * Method used by the "stepi" command when the "-r" flag is used.
         * @return list of register names, where each register in that list will be read through the
         * "int_register" interface before and after an instruction.
         */
        attr_value_t get_diff_regs() override;
        /**
         * Method called before the disassembly to find out if the next step will not be an
         * instruction, but rather a take exception or interrupt. The function should inspect the
         * given cpu and return NULL if the next step will be the execution of the instruction at
         * the current program counter.
         * @return If the next step will instead be handling of the exception or interrupt, then
         * saying that should be returned.
         */
        char *get_pending_exception_string() override;
        /**
         * Method returns a string with the default address prefix for memory related commands.
         * Simics defines the generic prefixes "v" for virtual, "l" for linear, "p" for physical.
         * The default is set to "v".
         * @return address type identifier (prefix: v/l/p)
         */
        char *get_address_prefix() override;
        /**
         * Method translates an address to a physical address. If translate_to_physical is NULL
         * then the only allowed address prefixes are "v" (virtual) and "p" (physical), and then
         * "logical_to_physical" from "processor_info" interface will be used to translate
         * virtual addresses.
         */
        physical_block_t translate_to_physical(
            const char *prefix,
            generic_address_t address) override;
        // ! DirectMemoryUpdateInterface (dmem-iface-impl) !
        void release(
            conf_object_t *target,
            direct_memory_handle_t handle,
            direct_memory_ack_id_t id) override;
        void update_permission(
            conf_object_t *target,
            direct_memory_handle_t handle,
            access_t lost_access,
            access_t lost_permission,
            access_t lost_inhibit,
            direct_memory_ack_id_t id) override;
        void conflicting_access(
            conf_object_t *target,
            direct_memory_handle_t handle,
            access_t conflicting_permission,
            direct_memory_ack_id_t id) override;
        // ! StepInterface (step-iface-impl) !
        /** Method returns the number of steps that corresponds to one unit of execution.
         * For a CPU this is typically one instruction, but it could be more for a VLIW
         * (Very Long Instruction Word) architecture, or less for a very simple CPU that
         * can only execute part of an instruction in one step.
         */
        pc_step_t get_step_count() override;
        /**
         * Method is used to post a step event after 'steps' steps. The event will be posted
         * after the given number of steps have been executed. The event will be posted
         * to the specified event class and object. The user_data parameter is a pointer
         * that will be passed to the event callback when the event is posted.
         */
        void post_step(
            event_class_t *evclass,
            conf_object_t *obj,
            pc_step_t steps,
            lang_void *user_data) override;
        /**
         * Method is used to cancel step events that have been posted with post_step.
         * The pred parameter is a pointer to a function that will be called for each
         * posted event. If the function returns true, the event will be canceled.
         */
        void cancel_step(
            event_class_t *evclass,
            conf_object_t *obj,
            int (*pred)(lang_void *data, lang_void *match_data),
            lang_void *match_data) override;
        /**
         * Method is used to find the next step event that has been posted with post_step.
         */
        pc_step_t find_next_step(
            event_class_t *evclass,
            conf_object_t *obj,
            int (*pred)(lang_void *data, lang_void *match_data),
            lang_void *match_data) override;
        attr_value_t events() override;
        /**
         * Method is used to advance the CPU by the given number of steps.
         * The method should return the number of steps that were actually executed.
         * This may be less than the number of steps requested if the CPU
         * encounters an exception or interrupt that causes it to stop executing.
         * The method should also handle any pending events that are due to be
         * executed during the advance.
         * Simics will call this method as part of its scheduling loop when the CPU is running.
         * @param steps The number of steps to advance the CPU.
         * @return The number of steps that were actually executed.
         */
        pc_step_t advance(pc_step_t steps) override;
        // ! ExecuteInterface (exec-iface-impl) !
        /**
         * Starts execution of the CPU. This function is called to start/restart the CPU executing.
         */
        void run() override;
        /**
         * Stops execution of the CPU. This function is called to stop the CPU from executing.
         * Having received stop, the processor model must stop the simulation as soon as possible
         * and return to the Simics core. This is typically done as soon as the currently
         * executing instruction is committed.
         */
        void stop() override;
        /**
         * Called when this CPU is switched in (becomes the current CPU).
         * It means that if gain control of the simulation from some other execute object
         * in the cell. This can be used to perform any necessary setup before the CPU
         * starts executing. For example, it can be used to set up the CPU state or to
         * allocate resources. This function is called from the simulation thread.
         * Note that this function may be called multiple times if the CPU is switched in
         * and out multiple times.
         * It is guaranteed that switch_out() will be called before switch_in() is called again.
         */
        void switch_in() override;
        /**
         * Called when this CPU is switched out (loses control of the simulation).
         * It means that some other execute object in the cell is taking control of the simulation.
         */
        void switch_out() override;

        // ! IntRegisterInterface (reg-iface-impl) !
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
        uint64 read(int reg) override;
        /**
         * Writes a new register value.
         */
        void write(int reg, uint64 val) override;
        /**
         * Returns a list of all register numbers that can be used for this object.
         */
        attr_value_t all_registers() override;
        /**
         * returns information about a single register. The information return depends on the info parameter
         */
        int register_info(int reg, ireg_info_t info) override;

        // ! ProcessorInfoV2Interface (proc-iface-impl) !
        /**
         * Function sets the program counter in the processor. The get_program_counter function returns
         * the current program counter.
         * @param pc The new value of the program counter.
         */
        void set_program_counter(logical_address_t pc) override;
        /**
         * Function returns the current value of the program counter.
         * @return The current value of the program counter.
         */
        logical_address_t get_program_counter() override;
        /**
         * The disassemble function returns the disassemble string for an instruction at address
         * with opcode according to instruction_data. The instruction_data is an attr_value_t value
         * of data type with the bytes of the opcode. The bytes are in the same order as they are
         * stored in memory.
         * @param address The address of the instruction to disassemble.
         * @param instruction_data The data of the instruction to disassemble.
         * @param sub_operation Used to select which sub-operation to disassemble. The sub-operations
         *     start at zero, and a request for the entire unit including all sub-operations is encoded
         *     with sub-operation -1.
         */
        tuple_int_string_t disassemble(
            generic_address_t address,
            attr_value_t instruction_data,
            int sub_operation) override;
        /**
         * Function translates a logical address to a physical address of the type defined by access_type.
         * The function returns a physical_block_t struct with valid bit and the address. The address is
         * valid when the valid bit is not 0. The logical_to_physical function also returns block_start
         * and block_end. The start and end of a block has the same logical to physical transformation as
         * the translated address. The range is inclusive, so block_end should be the address of the last
         * byte of the block.
         */
        physical_block_t logical_to_physical(
            logical_address_t address,
            access_t access_type) override;
        /**
         * Function returns the current processor mode. The return value is one of the processor_mode_t
         * enum values.
         */
        processor_mode_t get_processor_mode() override;
        /**
         * Function to enable the processor. The functions return 0 if the processor changed from
         * enabled to disabled or from disabled to enabled, and 1 if the processor did not change state.
         * The current state is returned by the get_enabled function
         */
        int enable_processor() override;
        /**
         * Function to disable the processor. The functions return 0 if the processor changed from
         * enabled to disabled or from disabled to enabled, and 1 if the processor did not change state.
         * The current state is returned by the get_enabled function
         */
        int disable_processor() override;
        /**
         * Function returns 1 if the processor is enabled and 0 if it is disabled.
         */
        int get_enabled() override;
        /**
         * Function returns the endian of the processor. The return value is one of the cpu_endian_t
         * enum values.
         */
        cpu_endian_t get_endian() override;
        /**
         * Function returns the physical memory object of the processor. The physical memory object
         * is used by Simics to access the memory of the processor, for instance to set breakpoints
         * by the global break command, and to read and write physical memory through set, get,
         * load-binary, load-file, and the default implementation of disassemble.
         * @return The object returned implements the memory_space and breakpoint interfaces.
         *     The memory_space interface for the returned object is only be used in inquiry mode
         *     corresponding to actions by the simulator itself rather than by the simulated software.
         */
        conf_object_t *get_physical_memory() override;
        /**
         * Function returns the number of logical/virtual address bits of the processor.
         */
        int get_logical_address_width() override;
        /**
         * Function returns the number of physical address bits of the processor.
         */
        int get_physical_address_width() override;
        /**
         * Returns the architecture string of the processor.
         * The architecture string is a short string that identifies the architecture of the processor.
         * It is used by Simics to determine which binaries can run on the processor.
         * Examples of architecture strings are "x86", "arm", "mips", "riscv".
         */
        const char *architecture() override;

        // Static function to initialize the class
        static void init_class(simics::ConfClass *cls) {
            // IntRegister interface is used to expose CPU registers to debugger and other tools
            cls->add(simics::iface::IntRegisterInterface::Info());
            // Execute interface is used to control execution of the CPU
            // By default the Simics scheduler will assume that the object being called in with the
            // execute interface also implements the corresponding processor_info and step interfaces
            // Instruction execution itself is typically handled in the Step interface implementation
            // The Execute interface is used to start and stop execution, as well as to switch between
            // different execution modes (e.g., user mode, supervisor mode).
            cls->add(simics::iface::ExecuteInterface::Info());
            // ProcessorInfo interface is used to provide information about the CPU architecture
            cls->add(simics::iface::ProcessorInfoInterface::Info());
            cls->add(simics::iface::ProcessorInfoV2Interface::Info());
            // ProcessorCli interface is used to provide CLI API for basic interaction with CPU model
            cls->add(simics::iface::ProcessorCliInterface::Info());
            // MemoryUpdate interface is used to control direct access to memory.
            // Every device that uses the direct_memory interface to access memory must implement this interface.
            cls->add(simics::iface::DirectMemoryUpdateInterface::Info());
            // Step interface is used to support stepping through instructions
            cls->add(simics::iface::StepInterface::Info());
            // Attributes
            cls->add(
                simics::Attribute(
                    "phys_mem", "o", "Physical memory space.",
                    ATTR_CLS_VAR(riscv_cpu, phys_mem_)
                )
            );
        }
    };
} /* ! kz::riscv::core ! */
