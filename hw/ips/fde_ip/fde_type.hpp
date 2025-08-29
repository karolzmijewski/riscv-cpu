#ifndef FDE_OP_FORMAT_H
#define FDE_OP_FORMAT_H

#include "fde_core.hpp"

#define UNDEF_TYPE 0
#define R_TYPE 1
#define I_TYPE 2
#define S_TYPE 3
#define B_TYPE 4
#define U_TYPE 5
#define J_TYPE 6
#define OTHER_TYPE 7

op_type_t get_op_type(opcode_t opcode);

#endif /* !FDE_OP_FORMAT_H! */