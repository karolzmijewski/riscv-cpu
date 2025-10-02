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

#include <iostream>

#include <simics/cc-api.h>
#include <simics/processor/processor-platform.h>
#include <simics/c++/model-iface/direct-memory.h>

#include "riscv-cpu.hpp"
#include "riscv-cpu-decode.hpp"
#include "riscv-cpu-conf.hpp"


namespace kz::riscv::core {
    riscv_cpu::riscv_cpu(simics::ConfObjectRef conf_obj)
    : simics::ConfObject(conf_obj), step_queue_("step-queue"), cycle_queue_("cycle-queue") {
        cobj_ = obj().object();
        regs_.fill(0);
        mstatus_ = 0;
        mepc_ = 0;
        mcause_ = 0;
        mtvec_ = 0;
        pc_ = 0;
        subsystem_ = 0;
        steps_in_quantum_ = 0;
        state_ = execute_state_t::Stopped;
        is_enabled_ = true;
        stall_cycles_ = 0;
        total_stall_cycles_ = 0;
        current_cycle_ = 0;
        current_step_ = 0;
        freq_hz_ = 100000000;  // Default frequency: 100 MHz
        time_offset_.val = {};
    }

    riscv_cpu::riscv_cpu::~riscv_cpu() {}

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

    uint8 *riscv_cpu::read_mem_(addr_t offset, unsigned size) {
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

    uint32_t riscv_cpu::read_reg_(int reg) {
        if (reg < 0 || reg >= RV32I_GP_REG_NUM) {
            throw std::out_of_range("Invalid register number");
        }
        return regs_[reg];
    }

    void riscv_cpu::write_reg_(int reg, uint32_t value) {
        if (reg < 0 || reg >= RV32I_GP_REG_NUM) {
            throw std::out_of_range("Invalid register number");
        }
        regs_[reg] = value;
    }

    kz::riscv::types::instr_t riscv_cpu::fetch_(addr_t address) {
        SIM_LOG_INFO(4, cobj_, 0, "Fetching instruction from address 0x%08x", static_cast<unsigned int>(address));
        uint8 *data = read_mem_(address, INSTR_SIZE);
        instr_t instr = 0;
        // Little-endian
        for (int i = 0; i < INSTR_SIZE; ++i) {
            instr |= static_cast<instr_t>(data[i]) << (i * 8);
        }
        SIM_LOG_INFO(
            4, cobj_, 0,
            "Fetched instruction 0x%08x from address 0x%08x",
            instr, static_cast<unsigned int>(address)
        );
        return instr;
    }

    kz::riscv::types::dec_instr_t riscv_cpu::decode_(instr_t instr) {
        SIM_LOG_INFO(4, cobj_, 0, "Decoding instruction 0x%08x", instr);
        dec_instr_t dec_instr;
        riscv_cpu_decoder::decode(instr, &dec_instr);
        return dec_instr;
    }

    void riscv_cpu::execute_(dec_instr_t dec_instr) {
        using operation_code_t = kz::riscv::types::operation_code_t;
        //cycles_t stall_cycles = 0; // for IDLE operation
        // cycles_t cycles_left = 0; // for IDLE operation
        uint32_t rs1_val = read_reg_(dec_instr.rs1);
        uint32_t rs2_val = read_reg_(dec_instr.rs2);
        int imm12 = ((int)dec_instr.imm) << 12; // for U_TYPE
        switch(dec_instr.opcode) {
            case operation_code_t::LOAD:
                // Load instructions (e.g., LB, LH, LW, LBU, LHU)
                // Implement load logic here
                SIM_LOG_INFO(2, cobj_, 0, "Executing LOAD instruction");
                pc_ += INSTR_SIZE;
                inc_cycles_(1);
                break;
            case operation_code_t::STORE:
                // Store instructions (e.g., SB, SH, SW)
                // Implement store logic here
                SIM_LOG_INFO(2, cobj_, 0, "Executing STORE instruction");
                pc_ += INSTR_SIZE;
                inc_cycles_(1);
                break;
            case operation_code_t::OP_IMM:
                SIM_LOG_INFO(2, cobj_, 0, "Executing OP_IMM instruction");
                // Immediate arithmetic instructions (e.g., ADDI, SLTI, ANDI)
                switch(dec_instr.func3) {
                    case 0b000: // ADDI
                        write_reg_(dec_instr.rd, static_cast<int32_t>(rs1_val) + static_cast<int32_t>(dec_instr.imm));
                        break;
                    case 0b010: // SLTI
                        write_reg_(dec_instr.rd, (static_cast<int32_t>(rs1_val) < static_cast<int32_t>(dec_instr.imm)) ? 1 : 0);
                        break;
                    case 0b011: // SLTIU
                        write_reg_(dec_instr.rd, (rs1_val < static_cast<uint32_t>(dec_instr.imm)) ? 1 : 0);
                        break;
                    case 0b100: // XORI
                        write_reg_(dec_instr.rd, static_cast<int32_t>(rs1_val) ^ static_cast<int32_t>(dec_instr.imm));
                        break;
                    case 0b110: // ORI
                        write_reg_(dec_instr.rd, static_cast<int32_t>(rs1_val) | static_cast<int32_t>(dec_instr.imm));
                        break;
                    case 0b111: // ANDI
                        write_reg_(dec_instr.rd, static_cast<int32_t>(rs1_val) & static_cast<int32_t>(dec_instr.imm));
                        break;
                    case 0b001: // SLLI
                        if (dec_instr.func7 != 0b0000000) {
                            SIM_LOG_ERROR(
                                cobj_, 0,
                                "Invalid SLLI func7: 0x%08x",
                                static_cast<uint32_t>(dec_instr.func7)
                            );
                            throw std::runtime_error("Invalid SLLI func7");
                        }
                        write_reg_(dec_instr.rd, static_cast<int32_t>(rs1_val) << (static_cast<int32_t>(dec_instr.imm) & 0b11111));
                        break;
                    case 0b101: // SRLI and SRAI
                        if (dec_instr.func7 == 0b0000000) {
                            // SRLI
                            write_reg_(dec_instr.rd, static_cast<uint32_t>(rs1_val) >> (static_cast<int32_t>(dec_instr.imm) & 0b11111));
                        } else if (dec_instr.func7 == 0b0100000) {
                            // SRAI
                            write_reg_(dec_instr.rd, static_cast<int32_t>(rs1_val) >> (static_cast<int32_t>(dec_instr.imm) & 0b11111));
                        } else {
                            SIM_LOG_ERROR(
                                cobj_, 0,
                                "Invalid SRLI/SRAI func7: 0x%08x",
                                static_cast<uint32_t>(dec_instr.func7)
                            );
                            throw std::runtime_error("Invalid SRLI/SRAI func7");
                        }
                        break;
                    // Handle other immediate operations here
                    default:
                        SIM_LOG_ERROR(
                            cobj_, 0,
                            "Unsupported OP_IMM func3: 0x%08x",
                            static_cast<uint32_t>(dec_instr.func3)
                        );
                        throw std::runtime_error("Unsupported OP_IMM func3");
                }
                pc_ += INSTR_SIZE;
                inc_cycles_(1);
                break;
            case operation_code_t::OP:
                SIM_LOG_INFO(2, cobj_, 0, "Executing OP instruction");
                // Register-register arithmetic instructions (e.g., ADD, SUB, AND, OR)
                switch(dec_instr.func3) {
                    case 0b000: // ADD and SUB
                        if (dec_instr.func7 == 0b0000000) {
                            // ADD
                            write_reg_(dec_instr.rd, rs1_val + rs2_val);
                        } else if (dec_instr.func7 == 0b0100000) {
                            // SUB
                            write_reg_(dec_instr.rd, rs1_val - rs2_val);
                        } else {
                            SIM_LOG_ERROR(
                                cobj_, 0,
                                "Invalid ADD/SUB func7: 0x%08x",
                                static_cast<uint32_t>(dec_instr.func7)
                            );
                            throw std::runtime_error("Invalid ADD/SUB func7");
                        }
                        break;
                    case 0b001: // SLL
                        if (dec_instr.func7 != 0b0000000) {
                            SIM_LOG_ERROR(
                                cobj_, 0,
                                "Invalid SLL func7: 0x%08x",
                                static_cast<uint32_t>(dec_instr.func7)
                            );
                            throw std::runtime_error("Invalid SLL func7");
                        }
                        write_reg_(dec_instr.rd, rs1_val << (rs2_val & 0b11111));
                        break;
                    case 0b010: // SLT
                        if (dec_instr.func7 != 0b0000000) {
                            SIM_LOG_ERROR(
                                cobj_, 0,
                                "Invalid SLT func7: 0x%08x",
                                static_cast<uint32_t>(dec_instr.func7)
                            );
                            throw std::runtime_error("Invalid SLT func7");
                        }
                        write_reg_(dec_instr.rd, (rs1_val < rs2_val) ? 1 : 0);
                        break;
                    case 0b011: // SLTU
                        if (dec_instr.func7 != 0b0000000) {
                            SIM_LOG_ERROR(
                                cobj_, 0,
                                "Invalid SLTU func7: 0x%08x",
                                static_cast<uint32_t>(dec_instr.func7)
                            );
                            throw std::runtime_error("Invalid SLTU func7");
                        }
                        write_reg_(
                            dec_instr.rd,
                            (static_cast<uint32_t>(rs1_val) < static_cast<uint32_t>(rs2_val)) ? 1 : 0
                        );
                        break;
                    case 0b100: // XOR
                        if (dec_instr.func7 != 0b0000000) {
                            SIM_LOG_ERROR(
                                cobj_, 0,
                                "Invalid XOR func7: 0x%08x",
                                static_cast<uint32_t>(dec_instr.func7)
                            );
                            throw std::runtime_error("Invalid XOR func7");
                        }
                        write_reg_(dec_instr.rd, (rs1_val ^ rs2_val));
                        break;
                    case 0b101: // SRL and SRA
                        if (dec_instr.func7 == 0b0000000) {
                            // SRL
                            write_reg_(dec_instr.rd, static_cast<uint32_t>(rs1_val) >> (rs2_val & 0b11111));
                        } else if (dec_instr.func7 == 0b0100000) {
                            // SRA
                            write_reg_(dec_instr.rd, static_cast<int32_t>(rs1_val) >> (rs2_val & 0b11111));
                        } else {
                            SIM_LOG_ERROR(
                                cobj_, 0,
                                "Invalid SRL/SRA func7: 0x%08x",
                                static_cast<uint32_t>(dec_instr.func7)
                            );
                            throw std::runtime_error("Invalid SRL/SRA func7");
                        }
                        break;
                    case 0b110: // OR
                        if (dec_instr.func7 != 0b0000000) {
                            SIM_LOG_ERROR(
                                cobj_, 0,
                                "Invalid OR func7: 0x%08x",
                                static_cast<uint32_t>(dec_instr.func7)
                            );
                            throw std::runtime_error("Invalid OR func7");
                        }
                        break;
                        write_reg_(dec_instr.rd, (rs1_val | rs2_val));
                    case 0b111: // AND
                        if (dec_instr.func7 != 0b0000000) {
                            SIM_LOG_ERROR(
                                cobj_, 0,
                                "Invalid AND func7: 0x%08x",
                                static_cast<uint32_t>(dec_instr.func7)
                            );
                            throw std::runtime_error("Invalid AND func7");
                        }
                        write_reg_(dec_instr.rd, (rs1_val & rs2_val));
                        break;
                    // Handle other register-register operations here
                    default:
                        SIM_LOG_ERROR(
                            cobj_, 0,
                            "Unsupported OP func3: 0x%08x",
                            static_cast<uint32_t>(dec_instr.func3)
                        );
                        throw std::runtime_error("Unsupported OP func3");
                    }
                pc_ += INSTR_SIZE;
                inc_cycles_(1);
                break;
            case operation_code_t::LUI:
                // Load Upper Immediate
                SIM_LOG_INFO(2, cobj_, 0, "Executing LUI instruction");
                write_reg_(dec_instr.rd, imm12);
                pc_ += INSTR_SIZE;
                inc_cycles_(1);
                break;
            case operation_code_t::AUIPC:
                // Add Upper Immediate to PC
                SIM_LOG_INFO(2, cobj_, 0, "Executing AUIPC instruction");
                write_reg_(dec_instr.rd, pc_ + imm12);
                pc_ += INSTR_SIZE;
                inc_cycles_(1);
                break;
            case operation_code_t::JAL:
                // Jump and Link
                SIM_LOG_INFO(2, cobj_, 0, "Executing JAL instruction");
                write_reg_(dec_instr.rd, pc_ + INSTR_SIZE);
                pc_ += (int32_t)dec_instr.imm;
                inc_cycles_(1);
                break;
            case operation_code_t::JALR:
                // Jump and Link Register
                SIM_LOG_INFO(2, cobj_, 0, "Executing JALR instruction");
                write_reg_(dec_instr.rd, pc_ + INSTR_SIZE);
                pc_ = (rs1_val + (int32_t)dec_instr.imm) & 0xFFFFFFFE;
                inc_cycles_(1);
                break;
            case operation_code_t::BRANCH:
                // Branch instructions (e.g., BEQ, BNE, BLT, BGE, BLTU, BGEU)
                SIM_LOG_INFO(2, cobj_, 0, "Executing BRANCH instruction");
                switch(dec_instr.func3) {
                    case 0b000: // BEQ
                        if (rs1_val == rs2_val) {
                            pc_ += (int32_t)dec_instr.imm;
                        } else {
                            pc_ += INSTR_SIZE;
                        }
                        inc_cycles_(1);
                        break;
                    case 0b001: // BNE
                        if (rs1_val != rs2_val) {
                            pc_ += (int32_t)dec_instr.imm;
                        } else {
                            pc_ += INSTR_SIZE;
                        }
                        inc_cycles_(1);
                        break;
                    case 0b100: // BLT
                        if (static_cast<int32_t>(rs1_val) < static_cast<int32_t>(rs2_val)) {
                            pc_ += (int32_t)dec_instr.imm;
                        } else {
                            pc_ += INSTR_SIZE;
                        }
                        inc_cycles_(1);
                        break;
                    case 0b101: // BGE
                        if (static_cast<int32_t>(rs1_val) >= static_cast<int32_t>(rs2_val)) {
                            pc_ += (int32_t)dec_instr.imm;
                        } else {
                            pc_ += INSTR_SIZE;
                        }
                        inc_cycles_(1);
                        break;
                    case 0b110: // BLTU
                        if (static_cast<uint32_t>(rs1_val) < static_cast<uint32_t>(rs2_val)) {
                            pc_ += (int32_t)dec_instr.imm;
                        } else {
                            pc_ += INSTR_SIZE;
                        }
                        inc_cycles_(1);
                        break;
                    case 0b111: // BGEU
                        if (static_cast<uint32_t>(rs1_val) >= static_cast<uint32_t>(rs2_val)) {
                            pc_ += (int32_t)dec_instr.imm;
                        } else {
                            pc_ += INSTR_SIZE;
                        }
                        inc_cycles_(1);
                        break;
                    default:
                        SIM_LOG_ERROR(
                            cobj_, 0,
                            "Unsupported BRANCH func3: 0x%08x",
                            static_cast<uint32_t>(dec_instr.func3)
                        );
                        throw std::runtime_error("Unsupported BRANCH func3");
                }
                break;
            default:
                SIM_LOG_ERROR(
                    cobj_, 0,
                    "Unsupported opcode: 0x%08x",
                    static_cast<uint32_t>(dec_instr.opcode)
                );
                throw std::runtime_error("Unsupported opcode");
        }
    }

    void riscv_cpu::handle_events_(event_queue_t *queue) {
        while (!queue->is_empty()
            && queue->get_delta() == 0
            && state_ != execute_state_t::Stopped) {
                /* Check for pending asynchronous events before
                handling CPU event */
                VT_check_async_events();
                queue->handle_next();
        }
    }

    void riscv_cpu::inc_cycles_(int cycles) {
        if (cycles < 0) {
            throw std::invalid_argument("Cycles to increment must be non-negative");
        }
        if (cycles > 0) {
            cycle_queue_.decrement(cycles);
            current_cycle_ += cycles;
        }
    }

    void riscv_cpu::inc_steps_(int steps) {
        if (steps < 0) {
            throw std::invalid_argument("Steps to increment must be non-negative");
        }
        if (steps > 0) {
            step_queue_.decrement(steps);
            current_step_ += steps;
        }
    }

    void riscv_cpu::finalize() {
    }

    void riscv_cpu::objects_finalized() {
        mem_handler_ = get_mem_handler_(RESET_ADDR, RAM_SIZE);
    }
} /* ! kz::riscv::core ! */

// init_local() is called once when the device module is loaded into Simics
// It is responsible to initialize device, and register the device class in module
// The function is declared 'extern "C"' to prevent C++ name mangling
// The function is wrapped in a try/catch block to catch and report exceptions
// that may be thrown during device registration
extern "C"
void init_local() try {
    simics::make_class<kz::riscv::core::riscv_cpu>(
        "riscv_cpu",
        "RV32I CPU model, single core, 32-bit, in-order, non-pipelined, no hyperthreading.",
        "This is a basic implementation of a RISC-V RV32I CPU model. It supports a subset of "
        "the RISC-V instruction set architecture (ISA) and is intended for educational and "
        "simulation purposes. The model is single-core, 32-bit, in-order, non-pipelined, and "
        "does not support hyperthreading. It provides basic functionalities such as instruction "
        "fetch, decode, execute, memory access, and write-back stages. The model also includes a "
        "simple memory management unit (MMU - currently in progress) for address translation and "
        "supports basic exception handling. Note that this is a simplified model and may not "
        "include all features of a full-fledged RISC-V CPU implementation. For more advanced "
        "features and optimizations, please refer to more comprehensive RISC-V CPU models or "
        "implementations.");
} catch(const std::exception& e) {
    std::cerr << e.what() << std::endl;
}
