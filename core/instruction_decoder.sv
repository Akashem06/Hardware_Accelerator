`include "global_defines.sv"

module instruction_decoder
import instruction_pkg::*;          // Import isntruction package
(
    input  logic        clk,        // Clock signal
    input  logic        rst_n,      // Active low reset

    // Instruction fetch pipeline interface
    input logic [`INSTRUCTION_WIDTH-1:0]    fetch_instruction,
    input logic                             fetch_valid,

    // Instruction decoder output
    output decoded_instr_t decoded_instruction,     // Decoded instruction struct
    output logic           decode_valid             // Instruction decode valid signal
);

// ==============================================================================
// Combinational field extraction and control derivation
// ==============================================================================

decoded_instr_t combinational_decoded_instruction;
logic           combinational_decode_valid;

always_comb begin
    // Based on custom ISA spec
    logic [3:0]  local_opcode = fetch_instruction [31:28]; // Operation code
    logic [3:0]  local_rd     = fetch_instruction [27:24]; // Desination register
    logic [3:0]  local_rs1    = fetch_instruction [23:20]; // Source register 1
    logic [3:0]  local_rs2    = fetch_instruction [19:16]; // Source register 2
    logic [15:0] local_imm    = fetch_instruction [15:0]; // Immediate value or offset value

    // Default values
    combinational_decoded_instruction = '0;
    combinational_decode_valid = 1'b0;

    // Valid fetch instruction
    if (fetch_valid) begin
        combinational_decoded_instruction.opcode = opcode_t'(local_opcode); // Type cast opcode
        combinational_decoded_instruction.rd     = local_rd;
        combinational_decoded_instruction.rs1    = local_rs1;
        combinational_decoded_instruction.rs2    = local_rs2;
        combinational_decoded_instruction.imm    = local_imm;

        // Check if it is a matrix operation
        combinational_decoded_instruction.is_matrix_op = (local_opcode inside {
            MLOAD,
            MSTORE,
            MMULT,
            MADD
        });

        // Check if it requires DMA
        combinational_decoded_instruction.uses_dma = (local_opcode inside {
            DMA_ST,
            DMA_CHK
        });

        combinational_decoded_instruction.needs_sync = (local_opcode == SYNC);

        combinational_decode_valid = 1'b1;
    end
end

// ==============================================================================
// Sequential logic
// ==============================================================================
always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        decoded_instruction <= '0;
        decode_valid        <= 1'b0;
    end else begin
        decoded_instruction <= combinational_decoded_instruction;
        decode_valid          <= combinational_decode_valid;
    end
end

endmodule
