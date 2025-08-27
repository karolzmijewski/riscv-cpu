#ifndef FDE_CORE_HPP
#define FDE_CORE_HPP

#include "ap_int.h"

// memory defines
// it is set to 2^16 words, single word is 2^2 (4 bytes) so it will consume
// (2^2)^16) = 2^18 bytes of memory, which is equal to 256 KB, change this
// parameter to extend the memory
#define RAM_ADDR_WIDTH 16
#define RAM_SIZE (1 << RAM_ADDR_WIDTH)

// cpu opcodes defines 
#define RET_OPCODE 0x8067

// basic types
typedef unsigned int instr_t;
typedef ap_uint<RAM_ADDR_WIDTH> addr_t;
typedef ap_uint<1> bit_t;

#endif /* !FDE_CORE_HPP! */