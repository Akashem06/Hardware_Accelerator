`include "global_defines.sv"

/* verilator lint_off UNUSED */
module top (
    input logic clk,
    input logic rst_n
);

logic [`INSTRUCTION_WIDTH-1:0] pc;

// ==============================================================================
// Instruction Fetch (IF) Pipeline Stage
// ==============================================================================
logic [`INSTRUCTION_WIDTH-1:0] fetch_instruction;
logic fetch_valid;
logic fetch_req;

// ==============================================================================
// Cache Memory Interface
// ==============================================================================
logic cache_hit;                                // Cache hit flag
logic [`INSTRUCTION_WIDTH-1:0] mem_instruction; // Output instruction from cache/main memory

logic                            mem_read;    // Read request to memory
logic [`INSTRUCTION_WIDTH-1:0]   mem_addr;    // Address to fetch from memory
logic [(64*8)-1:0]               mem_data;    // Cache line width
logic                            mem_valid;   // Memory valid signal

// ==============================================================================
// Branch Instruction
// ==============================================================================
logic branch_taken;
logic [`INSTRUCTION_WIDTH-1:0] branch_target;

instruction_cache u_instruction_cache (
    .clk(clk),
    .rst_n(rst_n),
    .pc(pc),
    .fetch_req(fetch_req),
    .instruction(mem_instruction),
    .cache_hit(cache_hit),
    .mem_read(mem_read),
    .mem_addr(mem_addr),
    .mem_data(mem_data),
    .mem_valid(mem_valid)
);

instruction_fetch u_instruction_fetch (
    .clk(clk),
    .rst_n(rst_n),
    .branch_taken(branch_taken),
    .branch_target(branch_target),
    .pc(pc),
    .fetch_req(fetch_req),
    .instruction(),
    .cache_hit(cache_hit),
    .pipeline_stall(1'b0),
    .fetch_instruction(fetch_instruction),
    .fetch_valid(fetch_valid)
);

endmodule
