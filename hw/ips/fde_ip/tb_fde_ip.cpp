#include <stdio.h>
#include "fde_core.hpp"
#include "fde_ip.hpp"

unsigned int ram[RAM_SIZE] = {
    #include "./tests/test_op_imm.hex"
};

/**
 * main function need to return int value, "void"
 * is not supported by Vitis_HLS
*/
int main() {
    unsigned int instr_counter;
    fde_ip(0, ram, &instr_counter);
    printf("%d fetched and decoded instructions\n", instr_counter);
    return 0;
}