simics_add_module(
    riscv-cpu
    CLASSES riscv_cpu
    SOURCES riscv-cpu.cc module_load.py
    SIMICS_API 7
    INIT_LOCAL
)
target_include_directories(
    riscv-cpu
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(
    riscv-cpu
    PRIVATE Simics::C++
)
add_subdirectory(test)
