# Copyright © 2025 Karol Zmijewski
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of this
# software and associated documentation files (the “Software”), to deal in the Software
# without restriction, including without limitation the rights to use, copy, modify, merge,
# publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
# to whom the Software is furnished to do so, subject to the following conditions:
# The above copyright notice and this permission notice shall be included in all copies or
# substantial portions of the Software.
# THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
#
# INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
# PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
# FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
# OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.

# class(es) implemented in this module
MODULE_CLASSES = riscv_cpu

# set file-names
CURRENT_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
SRC_FILES = riscv-cpu.cpp \
            riscv-cpu-decode.cpp \
            riscv-cpu-encode.cpp \
            riscv-cpu-disasm.cpp \
            riscv-cpu-types.cpp \
            reg-iface-impl.cpp \
            exec-iface-impl.cpp \
            proc-iface-impl.cpp \
            proc-cli-iface-impl.cpp \
            dmem-iface-impl.cpp
PYTHON_FILES = module_load.py
MODULE_CFLAGS += -I$(CURRENT_DIR)/include
SIMICS_API := 7
THREAD_SAFE := yes

# set this as we use the Simics C++ Device API
USE_CC_API = 2

# Set rpath to libstdc++
ifeq ($(HOST_TYPE), linux64)
MODULE_LDFLAGS = -Wl,-rpath,'$$ORIGIN/../sys/lib'
endif

ifeq ($(MODULE_MAKEFILE),)
    $(error Make sure you compile your module from the project directory)
endif
include $(MODULE_MAKEFILE)
