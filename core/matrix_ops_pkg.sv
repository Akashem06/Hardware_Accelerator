package matrix_ops_pkg;
typedef enum logic [2:0] {
    MATRIX_NONE         = 3'd0,
    MATRIX_LOAD         = 3'd1,     // ISA Instruction MLOAD
    MATRIX_STORE        = 3'd2,     // ISA Instruction MSTORE
    MATRIX_MULTIPLY     = 3'd3,     // ISA Instruction MMULT
    MATRIX_ADD          = 3'd4,     // ISA Instruction MADD
    MATRIX_SUBTRACT     = 3'd5,     // ISA Instruction MSUB
    MATRIX_TRANSPOSE    = 3'd6      // ISA Instruction MTRANSPOSE
} matrix_op_t;
endpackage