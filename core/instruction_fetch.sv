`include "global_defines.sv"

// ==============================================================================
// Instruction fetch pipeline block with prefetch depth of 4
// ==============================================================================

module instruction_fetch #(
    parameter PREFETCH_DEPTH = 4    // Number of instructions/addresses that can be speculatively fetched
)(
    input  logic        clk,        // Clock signal
    input  logic        rst_n,      // Active low reset
    
    // Branch Interface
    input  logic branch_taken,                            // Branch struction signal
    input  logic [`INSTRUCTION_WIDTH-1:0] branch_target,  // Branch target address signal (PC-independent)
    
    // Cache Interface
    output logic [`INSTRUCTION_WIDTH-1:0] pc,           // Program counter
    output logic fetch_req,                             // Fetch request to the instruction cache
    input  logic [`INSTRUCTION_WIDTH-1:0] instruction,  // Fetched instruction from cache
    input  logic cache_hit,                             // Hit/miss signal
    
    // Pipeline Interface
    input  logic pipeline_stall,                              // Pipeline stall input
    output logic [`INSTRUCTION_WIDTH-1:0] fetch_instruction,  // Fetched instruction output for pipeline
    output logic fetch_valid                                  // Valid instruction fetch signal for pipeline
);

// ==============================================================================
// PC and next PC logic
// ==============================================================================
logic [`INSTRUCTION_WIDTH-1:0] next_pc;

assign next_pc = pc + `WORD_SIZE;

always_ff @(posedge clk or negedge rst_n) begin
    if(!rst_n) begin
        pc <= 32'b0;
    end else if(!pipeline_stall) begin
        // If there is no pipeline stall
        if(branch_taken) begin
            pc <= branch_target;
        end else begin
            pc <= next_pc;
        end
    end
end

// ==============================================================================
// Prefetch buffer
// ==============================================================================
logic [`INSTRUCTION_WIDTH-1:0] prefetch_buffer [PREFETCH_DEPTH-1:0];    // 32-bit Circular buffer of size 4
logic [PREFETCH_DEPTH-1:0] buffer_valid;                                // Bitmap indicating validity of buffer entries
logic [$clog2(PREFETCH_DEPTH)-1:0] write_ptr, read_ptr;                 // Prefetch buffer write and read pointer

always_ff @(posedge clk or negedge rst_n) begin
    if(!rst_n) begin
        buffer_valid <= '0;
        write_ptr <= '0;
        read_ptr <= '0;
    end else begin
        if(branch_taken) begin
            // If a branch is taken reset everything and clear the prefetch buffer (Flushing all instructions)
            // This has the highest priority
            buffer_valid <= '0;
            write_ptr <= '0;
            read_ptr <= '0;
        end else begin
            if(cache_hit && !pipeline_stall && !buffer_valid[write_ptr]) begin
                // If cache is hit and no pipeline stall is present and the current write slot is not valid
                prefetch_buffer[write_ptr] <= instruction; // Write the instruction into the prefetch buffer
                buffer_valid[write_ptr] <= 1'b1;           // Update the valid buffer bitmap
                write_ptr <= write_ptr + 1;                // Increment the write pointer. Automatically wraps around
            end
            
            if(!pipeline_stall && buffer_valid[read_ptr]) begin
                // If the prefetch buffer is valid and there is no pipeline stall
                buffer_valid[read_ptr] <= 1'b0; // Set the prefetch buffer to invalid
                read_ptr <= read_ptr + 1;       // Increment the read pointer. Automatically wraps around
            end
        end
    end
end

// ==============================================================================
// Output updates
// ==============================================================================
assign fetch_instruction = prefetch_buffer[read_ptr];           // Output instruction from the prefetch buffer
assign fetch_valid = buffer_valid[read_ptr] && !branch_taken;   // Valid fetch is the valid bitmap is set, and branch is not taken
assign fetch_req = !pipeline_stall && !buffer_valid[write_ptr]; // Fetch request to instruction cache if no pipline stall and current write data is not valid

endmodule