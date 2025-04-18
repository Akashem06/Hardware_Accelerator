`include "global_defines.sv"

module execution_unit 
import instruction_pkg::*;
import matrix_ops_pkg::*;
(
    input  logic        clk,        // Clock signal
    input  logic        rst_n,      // Active low reset

    // Inputs from decode stage
    input  decoded_instr_t              decoded_instr,
    input logic                         instr_valid,

    // Register file interface
    input  logic [`DATA_WIDTH-1:0]          rs1_data,
    input  logic [`DATA_WIDTH-1:0]          rs2_data,
    output logic                            reg_write_en,
    output logic [`NUM_REGS_BIT_COUNT-1:0]  reg_write_addr,
    output logic [`DATA_WIDTH-1:0]          reg_write_data,

    // Matrix unit interface
    output matrix_op_t                    matrix_op,
    output logic                          matrix_start,
    output logic [`ADDR_WIDTH-1:0]        matrix_src1_addr,
    output logic [`ADDR_WIDTH-1:0]        matrix_src2_addr,
    output logic [`ADDR_WIDTH-1:0]        matrix_dest_addr,
    output logic [`INSTRUCTION_WIDTH-1:0] matrix_dimension, // First 16 bits is for the rows, latter 16 bits are for the columns
    input  logic                          matrix_busy,
    input  logic                          matrix_done,
    input  logic                          matrix_error,

    // Configuration interface
    output logic                          conf_write_en,
    output logic [`INSTRUCTION_WIDTH-1:0] conf_data,

    // Status signals
    output logic                     exec_done,
    output logic                     exec_error,
    output logic                     exec_busy
);

typedef enum logic [2:0] {
    IDLE,
    EXECUTE_ALU,
    EXECUTE_MATRIX,
    EXECUTE_DMA,
    EXECUTE_MEM,
    WAIT_COMPLETION,
    WRITEBACK
} exec_state_t;

endmodule
