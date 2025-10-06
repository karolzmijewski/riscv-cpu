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
#include <simics/c++/model-iface/cycle.h>
#include <simics/c++/model-iface/cycle-event.h>
#include <simics/c++/devs/frequency.h>

#include "riscv-cpu-types.hpp"
#include "riscv-cpu-conf.hpp"
#include "riscv-cpu-state.hpp"
#include "riscv-cpu-queue.hpp"

namespace kz::riscv::core {
    class RiscvCpu:
        public simics::ConfObject,
        public simics::iface::IntRegisterInterface,
        public simics::iface::StepInterface,
        public simics::iface::ExecuteInterface,
        public simics::iface::CycleInterface,
        public simics::iface::ProcessorInfoInterface,
        public simics::iface::ProcessorInfoV2Interface,
        public simics::iface::ProcessorCliInterface,
        public simics::iface::DirectMemoryUpdateInterface,
        public simics::iface::FrequencyListenerInterface {
    private:
        // types
        using addr_t = kz::riscv::types::addr_t;
        using instr_t = kz::riscv::types::instr_t;
        using dec_instr_t = kz::riscv::types::dec_instr_t;
        // attributes
        conf_object_t *cobj_;
        std::array<uint32_t, RV32I_GP_REG_NUM> regs_; // x0..x31
        uint32_t pc_;
        uint32_t mstatus_, mepc_, mcause_, mtvec_;
        direct_memory_lookup_t mem_handler_;
        simics::Connect<simics::iface::DirectMemoryLookupInterface> phys_mem_;
        // state
        uint64_t subsystem_;
        execute_state_t state_;
        bool is_enabled_;
        uint64_t freq_hz_;
        cycles_t current_cycle_;
        cycles_t stall_cycles_;
        cycles_t total_stall_cycles_;
        pc_step_t current_step_;
        bigtime_t time_offset_;
        event_queue_t step_queue_;
        event_queue_t cycle_queue_;
        // methods
        // -- methods: memory access
        direct_memory_lookup_t get_mem_handler_(physical_address_t addr, unsigned size);
        uint8 *read_mem_(addr_t addr, unsigned size);
        // -- methods: register access
        inline uint32_t read_reg_(int reg);
        inline void write_reg_(int reg, uint32_t value);
        // -- methods: instruction processing
        instr_t fetch_(addr_t addr);
        dec_instr_t decode_(instr_t instr);
        void execute_(dec_instr_t dec_instr);
        // -- methods: cycle / step processing
        void handle_events_(event_queue_t *queue);
        void inc_cycles_(int cycles);
        void inc_steps_(int steps);
    public:
        explicit RiscvCpu(simics::ConfObjectRef conf_obj);
        virtual ~RiscvCpu();
        void set_freq_hz(uint64_t new_freq);

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

        class frequency_port:
            public simics::Port<RiscvCpu>,
            public simics::iface::SimpleDispatcherInterface {
        public:
            explicit frequency_port(simics::ConfObjectRef obj) : simics::Port<RiscvCpu>(obj) {}
            virtual ~frequency_port() = default;

            // ! SimpleDispatcherInterface (sdispatcher-iface-impl) !
            void subscribe(conf_object_t *listener, const char *listener_port) override;
            void unsubscribe(conf_object_t *listener, const char *listener_port) override;

            static void init_class(simics::ConfClass *cls) {
                cls->add(simics::iface::SimpleDispatcherInterface::Info());
            }
        };

        // ! FrequencyListenerInterface (frequency-listener-iface-impl) !
        /**
         * Method is called to set the frequency of the listener.
         * @param numerator the frequency numerator in Hz
         * @param denominator the frequency denominator in Hz
         */
        void set(uint64 numerator, uint64 denominator) override;

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
         * @param prefix the address type prefix, as returned by "get_address_prefix"
         * @param address the address to translate
         */
        physical_block_t translate_to_physical(
            const char *prefix,
            generic_address_t address) override;

        // ! DirectMemoryUpdateInterface (dmem-iface-impl) !
        /**
         * Method is called to release a direct memory mapping previously obtained
         * through "get_direct_memory" method in the "direct_memory_lookup" interface.
         * @param target the object that obtained the mapping
         * @param handle the handle that was returned by "get_direct_memory"
         * @param id the ack id that was returned by "get_direct_memory"
         */
        void release(
            conf_object_t *target,
            direct_memory_handle_t handle,
            direct_memory_ack_id_t id) override;
        /**
         * Method is called to inform the CPU that some access or permission has been lost
         * on a direct memory mapping previously obtained through "get_direct_memory"
         * @param target the object that obtained the mapping
         * @param handle the handle that was returned by "get_direct_memory"
         * @param lost_access the access that has been lost, see access_t enum
         * @param lost_permission the permission that has been lost, see access_t enum
         * @param lost_inhibit the inhibit that has been lost, see access_t enum
         * @param id the ack id that was returned by "get_direct_memory"
         */
        void update_permission(
            conf_object_t *target,
            direct_memory_handle_t handle,
            access_t lost_access,
            access_t lost_permission,
            access_t lost_inhibit,
            direct_memory_ack_id_t id) override;
        /**
         * Method is called to inform the CPU that a conflicting access has been attempted
         * on a direct memory mapping previously obtained through "get_direct_memory"
         * @param target the object that obtained the mapping
         * @param handle the handle that was returned by "get_direct_memory"
         * @param conflicting_permission the permission that was violated, see access_t enum
         * @param id the ack id that was returned by "get_direct_memory"
         */
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
         * @return The number of steps that corresponds to one unit of execution.
         */
        pc_step_t get_step_count() override;
        /**
         * Method is used to post a step event after 'steps' steps. The event will be posted
         * after the given number of steps have been executed. The event will be posted
         * to the specified event class and object. The user_data parameter is a pointer
         * that will be passed to the event callback when the event is posted.
         * @param evclass the event class to post the event to
         * @param obj the object to post the event to
         * @param steps the number of steps to wait before posting the event
         * @param user_data a pointer that will be passed to the event callback when the event
         *   is posted.
         * @return The number of steps until the event is posted.
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
         * @param evclass the event class to match, or NULL to match all event classes
         * @param obj the object to match, or NULL to match all objects
         * @param pred a pointer to a function that will be called for each posted event. If the
         *    function returns true, the event will be canceled.
         * @param match_data a pointer that will be passed to the pred function when it is called.
         * @return The number of step events that were canceled.
         */
        void cancel_step(
            event_class_t *evclass,
            conf_object_t *obj,
            int (*pred)(lang_void *data, lang_void *match_data),
            lang_void *match_data) override;
        /**
         * Method is used to find the next step event that has been posted with post_step.
         * The pred parameter is a pointer to a function that will be called for each
         * posted event. If the function returns true, the event will be returned.
         * @param evclass the event class to match, or NULL to match all event classes
         * @param obj the object to match, or NULL to match all objects
         * @param pred a pointer to a function that will be called for each posted event. If the
         *     function returns true, the event will be returned.
         * @param match_data a pointer that will be passed to the pred function when it is called.
         * @return The number of steps until the next matching event, or -1 if no matching event is
         *     found.
         */
        pc_step_t find_next_step(
            event_class_t *evclass,
            conf_object_t *obj,
            int (*pred)(lang_void *data, lang_void *match_data),
            lang_void *match_data) override;
        /**
         * Method returns a list of all event classes that have events posted on them.
         * TODO: DUPLICATE WITH CYCLE INTERFACE???
         */
        //attr_value_t events() override;
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

        // ! CycleInterface (cycle-iface-impl) !
        /**
         * Method returns the current cycle count of the CPU. The cycle count is a monotonically
         * increasing value that represents the number of cycles that have been executed by the CPU.
         * The cycle count is typically incremented by one for each cycle, but it can be incremented
         * by more than one for CPUs that can execute multiple cycles in parallel.
         * @return current cycle count
         */
        cycles_t get_cycle_count() override;
        /**
         * Method returns the current time in seconds. The time is typically derived from the
         * cycle count and the CPU frequency.
         * @return current time in seconds
         */
        double get_time() override;
        /**
         * Method returns the number of cycles that have elapsed since the given time in seconds.
         * @return number of cycles since the given time in seconds.
         */
        cycles_t cycles_delta(double when) override;
        /**
         * Method returns the frequency of the CPU in Hz.
         * @return frequency in Hz
         */
        uint64_t get_frequency() override;
        /**
         * Method is used to post a cycle event after 'cycles' cycles. The event will be posted
         * after the given number of cycles have been executed. The event will be posted
         * to the specified event class and object. The user_data parameter is a pointer
         * that will be passed to the event callback when the event is posted.
         * @param evclass the event class to match, or NULL to match all event classes
         * @param obj the object to match, or NULL to match all objects
         * @param cycles the number of cycles after which the event should be posted
         * @param user_data a pointer that will be passed to the event callback when the event
         *   is posted. This parameter can be used to pass additional data to the event callback.
         */
        void post_cycle(
            event_class_t *evclass,
            conf_object_t *obj,
            cycles_t cycles,
            lang_void *user_data) override;
        /**
         * Method is used to post a time event after 'seconds' seconds. The event will be posted
         * after the given time has elapsed. The event will be posted
         * to the specified event class and object. The user_data parameter is a pointer
         * that will be passed to the event callback when the event is posted.
         * @note This method is similar to post_cycle, but uses time in seconds instead of cycles.
         * @param evclass the event class to match, or NULL to match all event classes
         * @param obj the object to match, or NULL to match all objects
         * @param seconds the time in seconds after which the event should be posted
         * @param user_data a pointer that will be passed to the event callback when the event is posted.
         * @note This parameter can be used to pass additional data to the event callback.
         */
        void post_time(
            event_class_t *evclass,
            conf_object_t *obj,
            double seconds,
            lang_void *user_data) override;
        /**
         * Method is used to cancel cycle events that have been posted with post_cycle.
         * The pred parameter is a pointer to a function that will be called for each
         * posted event. If the function returns true, the event will be canceled.
         * @param evclass the event class to match, or NULL to match all event classes
         * @param obj the object to match, or NULL to match all objects
         *  @param pred a pointer to a function that will be called for each posted event.
         *    If the function returns true, the event will be canceled.
         * @param match_data a pointer that will be passed to the pred function when it is called.
         */
        void cancel(
            event_class_t *evclass,
            conf_object_t *obj,
            int (*pred)(lang_void *data,
                lang_void *match_data),
                lang_void *match_data) override;
        /**
         * Method is used to find the next cycle event that has been posted with post_cycle.
         * The method returns the number of cycles until the next event that matches
         * the given criteria. If no matching event is found, the method returns
         * a special value indicating that there are no more events.
         * The pred parameter is a pointer to a function that will be called for each
         * posted event. If the function returns true, the event is considered a match.
         * @param evclass the event class to match, or NULL to match all event classes
         * @param obj the object to match, or NULL to match all objects
         * @param pred a pointer to a function that will be called for each posted event.
         *     If the function returns true, the event is considered a match.
         * @param match_data a pointer that will be passed to the pred function when it is called.
         * @return The number of cycles until the next matching event, or a special value if no
         *     matching event is found.
         */
        cycles_t find_next_cycle(
            event_class_t *evclass,
            conf_object_t *obj,
            int (*pred)(lang_void *data, lang_void *match_data),
            lang_void *match_data) override;
        /**
         * Method is used to find the next time event that has been posted with post_time.
         * The method returns the time in seconds until the next event that matches
         * the given criteria. If no matching event is found, the method returns
         * a special value indicating that there are no more events.
         * The pred parameter is a pointer to a function that will be called for each
         * posted event. If the function returns true, the event is considered a match.
         * @note This method is similar to find_next_cycle, but uses time in seconds instead of cycles.
         * @param evclass The event class to match, or NULL to match all event classes.
         * @param obj The object to match, or NULL to match all objects.
         * @param pred A pointer to a function that will be called for each posted event.
         *     If the function returns true, the event is considered a match.
         * @param match_data A pointer that will be passed to the pred function when it is called.
         * @return The time in seconds until the next matching event, or a special value if no
         *     matching event is found.
         */
        double find_next_time(
            event_class_t *evclass,
            conf_object_t *obj,
            int (*pred)(lang_void *data, lang_void *match_data),
            lang_void *match_data) override;
        /**
         * Method returns a list of all event classes that have events posted on them.
         */
        attr_value_t events() override;
        /**
         * Method returns the current time in picoseconds. The time is typically derived from the
         * cycle count and the CPU frequency.
         * @note This method is similar to get_time, but returns the time in picoseconds.
         * @return The current time in picoseconds.
         */
        local_time_t get_time_in_ps() override;
        /**
         * Method returns the number of picoseconds that have elapsed since the given time.
         * @param when The time in picoseconds to calculate the delta from.
         * @return The number of picoseconds that have elapsed since the given time.
         */
        cycles_t cycles_delta_from_ps(local_time_t when) override;
        /**
         * Method returns the frequency of the CPU in Hz.
         */
        //uint64_t get_frequency() override;
        /**
         * Method is used to post a time event after 'picoseconds' picoseconds. The event will be
         * posted after the given time has elapsed. The event will be posted
         * to the specified event class and object. The user_data parameter is a pointer
         * that will be passed to the event callback when the event is posted.
         * @note This method is similar to post_time, but uses picoseconds instead of seconds.
         * @param evclass the event class to post the event to
         * @param obj the object to post the event to
         * @param picoseconds the time in picoseconds after which the event should be posted
         * @param user_data a pointer that will be passed to the event callback when the event is
         *     posted
         */
        void post_time_in_ps(
            event_class_t *evclass,
            conf_object_t *obj,
            duration_t picoseconds,
            lang_void *user_data) override;
        /**
         * Method is used to cancel time events that have been posted with post_time_in_ps.
         * The pred parameter is a pointer to a function that will be called for each
         * posted event. If the function returns true, the event will be canceled.
         * @param evclass the event class to match, or NULL to match all event classes
         * @param obj the object to match, or NULL to match all objects
         * @param pred a pointer to a function that will be called for each posted event.
         *     If the function returns true, the event will be canceled.
         * @param match_data a pointer that will be passed to the pred function when it is called.
         * @return the number of events that were canceled.
         */
        duration_t find_next_time_in_ps(
            event_class_t *evclass,
            conf_object_t *obj,
            int (*pred)(lang_void *data, lang_void *match_data),
            lang_void *match_data) override;

        // ! IntRegisterInterface (reg-iface-impl) !
        // The int_register interface is used for access to registers in a processor.
        // It can be used to access any kind of integer register, not only the "normal"
        // registers. This includes all kinds of control registers, hidden registers
        // and anything else that might be useful to access as a register. The only
        // limitation is that the register value should be representable as a 64-bit
        // unsigned integer.
        /**
         * Translates a register name to its number.
         * @param name The name of the register to translate.
         * @return -1 if the register does not exist.
         */
        int get_number(const char *name) override;
        /**
         * Translates a register number to its canonical name.
         * @param reg The number of the register to translate.
         * @return NULL if the register does not exist.
         */
        const char *get_name(int reg) override;
        /**
         * Reads a register value.
         * @param reg The number of the register to read.
         * @return The value of the register.
         */
        uint64 read(int reg) override;
        /**
         * Writes a new register value.
         * @param reg The number of the register to write.
         * @param val The new value of the register.
         */
        void write(int reg, uint64 val) override;
        /**
         * Returns a list of all register numbers that can be used for this object.
         */
        attr_value_t all_registers() override;
        /**
         * Get information about a single register. The information return depends on the info
         * parameter.
         * @param reg The number of the register to get information about.
         * @param info The type of information to return.
         * @return The requested information about the register.
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
         * @return A tuple with the length of the instruction in bytes and the disassembly string.
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
         * @param address The logical address to translate.
         * @param access_type The type of access to translate. The access_type is one of the access_t
         *     enum values.
         * @return A physical_block_t struct with the translation result.
         */
        physical_block_t logical_to_physical(
            logical_address_t address,
            access_t access_type) override;
        /**
         * Function returns the current processor mode. The return value is one of the processor_mode_t
         * enum values.
         * @return The current processor mode.
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
         * @return The endian of the processor.
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
         * @return The architecture string of the processor.
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
            // Cycle interface is used to support cycle-accurate simulation
            cls->add(simics::iface::CycleInterface::Info());
            SIM_register_clock(
                *cls, static_cast<const cycle_interface_t*>(
                    simics::iface::CycleInterface::Info().cstruct()
                )
            );
            // Attributes
            cls->add(
                simics::Attribute(
                    "phys_mem", "o", "Physical memory space.",
                    ATTR_CLS_VAR(RiscvCpu, phys_mem_)
                )
            );
            cls->add(
                simics::Attribute(
                    "freq_hz", "i", "CPU frequency in Hz.",
                    ATTR_CLS_VAR(RiscvCpu, freq_hz_)
                )
            );
            cls->add(
                simics::Attribute(
                    "steps", "i", "Number of steps executed.",
                    ATTR_CLS_VAR(RiscvCpu, current_step_)
                )
            );
            cls->add(
                simics::Attribute(
                    "step_queue", "[]|[[osaii]*]",
                    "Step event queue: ((<i>object</i>, <i>evclass</i>, <i>value</i>, <i>slot</i>,"
                    " <i>step</i>)*).",
                    [](conf_object_t *obj) -> attr_value_t {
                        auto *cpu = simics::from_obj<RiscvCpu>(obj);
                        return cpu->step_queue_.to_attr_list(0);
                    },
                    [](conf_object_t *obj, attr_value_t *val) {
                        auto *cpu = simics::from_obj<RiscvCpu>(obj);
                        return cpu->step_queue_.set(val);
                    }
                )
            );
            cls->add(
                simics::Attribute(
                    "cycles", "i", "Number of cycles executed.",
                    ATTR_CLS_VAR(RiscvCpu, current_cycle_)
                )
            );
            cls->add(
                simics::Attribute(
                    "cycle_queue", "[]|[[osaii]*]",
                    "Cycle event queue: ((<i>object</i>, <i>evclass</i>, <i>value</i>, <i>slot</i>,"
                    " <i>cycle</i>)*).",
                    [](conf_object_t *obj) -> attr_value_t {
                        auto *cpu = simics::from_obj<RiscvCpu>(obj);
                        return cpu->cycle_queue_.to_attr_list(0);
                    },
                    [](conf_object_t *obj, attr_value_t *val) {
                        auto *cpu = simics::from_obj<RiscvCpu>(obj);
                        return cpu->cycle_queue_.set(val);
                    }
                )
            );
            cls->add(
                simics::Attribute(
                    "pc", "i", "Program counter.",
                    ATTR_CLS_VAR(RiscvCpu, pc_)
                )
            );
        }
    };
} /* ! kz::riscv::core ! */
