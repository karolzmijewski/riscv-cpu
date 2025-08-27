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
    fde_ip(0, ram);
    printf("done\n");
    return 0;
}