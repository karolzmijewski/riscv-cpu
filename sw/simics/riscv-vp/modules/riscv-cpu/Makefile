# Makefile for the riscv-cpu module

# TODO: specify the class(es) implemented in this module
MODULE_CLASSES = riscv_cpu

# TODO: set file-names
SRC_FILES = riscv-cpu.cc
PYTHON_FILES = module_load.py
MODULE_CFLAGS =
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
