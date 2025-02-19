package instruction_pkg;
    // Custom ISA encoding
    typedef enum logic[3:0] {
        MLOAD   = 4'b0000,
        MSTORE  = 4'b0001,
        MMULT   = 4'b0010,
        MADD    = 4'b0011,
        DMA_ST  = 4'b0100,
        DMA_CHK = 4'b0101,
        CONF    = 4'b0110,
        SYNC    = 4'b0111,
        LD      = 4'b1000,
        ST      = 4'b1001
    } opcode_t;

    // Decoded instruction struct
    typedef struct packed {
        opcode_t     opcode;
        logic [3:0]  rd;
        logic [3:0]  rs1;
        logic [3:0]  rs2;
        logic [15:0] imm;
        logic        is_matrix_op;
        logic        uses_dma;
        logic        needs_sync;
    } decoded_instr_t;
endpackage
