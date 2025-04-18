`include "global_defines.sv"

module systolic_array
import matrix_ops_pkg::*;
(
    input  logic        clk,        // Clock signal
    input  logic        rst_n,      // Active low reset

    // Control interface
    input  matrix_op_t  operation,
    input  logic        start_operation,
    output logic        operation_done,
    output logic        operation_error,
    output logic        busy,

    // Configuration interface
    input  logic                          config_valid,
    input  logic [`INSTRUCTION_WIDTH-1:0] config_data  // Rows[31:16] Cols[15:0]
);

typedef enum logic [3:0] {
    S_IDLE,
    S_LOAD_CONFIG,
    S_LOAD_MATRIX_A,
    S_LOAD_MATRIX_B,
    S_EXECUTE,
    S_STORE_RESULT,
    S_DONE,
    S_ERROR
} state_t;

state_t current_state, next_state;

logic [`NUM_MATRIX_COLS_ROWS-1:0] matrix_rows;
logic [`NUM_MATRIX_COLS_ROWS-1:0] matrix_cols;

logic configured;

always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        current_state <= S_IDLE;
        configured <= 1'b0;
        matrix_rows <= `NUM_MATRIX_COLS_ROWS'd0;
        matrix_cols <= `NUM_MATRIX_COLS_ROWS'd0;
        busy <= 1'b0;
    end else begin
        current_state <= next_state;

        if (current_state == S_IDLE && config_valid) begin
            matrix_rows <= config_data[`INSTRUCTION_WIDTH-1:`NUM_MATRIX_COLS_ROWS];
            matrix_cols <= config_data[`NUM_MATRIX_COLS_ROWS-1:0];
            configured <= 1'b1;
        end

        busy <= (current_state != S_IDLE && current_state != S_DONE && current_state != S_ERROR);
    end
end

always_comb begin
    next_state = current_state;
    operation_done = 1'b0;
    operation_error = 1'b0;

    case (current_state)
        S_IDLE: begin
            if (config_valid) next_state = S_LOAD_CONFIG;
            else if (configured && start_operation) next_state = S_LOAD_MATRIX_A;
        end

        S_LOAD_CONFIG: begin
            next_state = S_IDLE;
        end

        S_LOAD_MATRIX_A: begin
            next_state = S_LOAD_MATRIX_B;
        end

        S_LOAD_MATRIX_B: begin
            next_state = S_EXECUTE;
        end

        S_EXECUTE: begin
            next_state = S_STORE_RESULT;
        end

        S_STORE_RESULT: begin
            next_state = S_DONE;
        end

        S_DONE: begin
            operation_done = 1'b1;
            next_state = S_IDLE;
        end

        S_ERROR: begin
            operation_error = 1'b1;
            next_state = S_IDLE;
        end

        default: begin
            next_state = S_ERROR;
        end
    endcase
end

endmodule