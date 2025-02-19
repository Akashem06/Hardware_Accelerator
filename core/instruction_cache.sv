`include "global_defines.sv"

// ==============================================================================
// 2-way associative instruction cache
// ==============================================================================

module instruction_cache #(
    parameter CACHE_SIZE_KB = 32,   // Cache size in KB
    parameter LINE_SIZE_BYTES = 64, // Cache line size in bytes
    parameter WAYS = 2              // Associativity of cache
)(
    input  logic                            clk,         // Clock signal
    input  logic                            rst_n,       // Active low reset

    // CPU Interface
    input  logic [`INSTRUCTION_WIDTH-1:0]   pc,          // Program counter
    input  logic                            fetch_req,   // Fetch request from CPU
    output logic [`INSTRUCTION_WIDTH-1:0]   instruction, // Fetched instruction output
    output logic                            cache_hit,   // Hit/miss signal
    
    // Memory Interface
    output logic                            mem_read,    // Read request to memory
    output logic [`INSTRUCTION_WIDTH-1:0]   mem_addr,    // Address to fetch from memory
    input  logic [(LINE_SIZE_BYTES*8)-1:0]  mem_data,    // Cache line width
    input  logic                            mem_valid    // Memory valid signal
);

// ==============================================================================
// Cache Parameters
// ==============================================================================
localparam SETS = (CACHE_SIZE_KB * 1024) / (LINE_SIZE_BYTES * WAYS);    // Number of cache sets = 256 sets
localparam SET_BITS = $clog2(SETS);                                     // Number of bits for set = 8 set bits
localparam OFFSET_BITS = $clog2(LINE_SIZE_BYTES);                       // Number of bits for offset = 6 offset bits
localparam TAG_BITS = 32 - SET_BITS - OFFSET_BITS;                      // Number of bits for tag = 32 - 8 - 6 = 18 tag bits

// ==============================================================================
// Cache Storage Structures
// ==============================================================================
logic [WAYS-1:0][SETS-1:0][LINE_SIZE_BYTES*8-1:0] data;     // Cache data storage (2 ways)(256 sets)(line size) = 32768 bytes
logic [WAYS-1:0][SETS-1:0][TAG_BITS-1:0] tags;              // Cache tag storage (2 ways)(256 sets)(8 set bits) = 512 bytes
logic [WAYS-1:0][SETS-1:0] valid;                           // Cache valid flag storage (2 ways)(256 sets) = 64 bytes
logic [WAYS-1:0][SETS-1:0] lru;                             // Cache lru flag storage (2 ways)(256 sets) = 64 bytes

// ==============================================================================
// Address Breakdown
// ==============================================================================
logic [TAG_BITS-1:0]    req_tag;        // Address request tag bits
logic [SET_BITS-1:0]    req_set;        // Address request set bits
logic [OFFSET_BITS-1:0] req_offset;     // Address request offset bits

logic [$clog2(LINE_SIZE_BYTES/`WORD_SIZE)-1:0] word_offset;  // 4 bits for word offset 

assign {req_tag, req_set, req_offset} = pc;         // Split program counter into tag, set and offset
assign word_offset = req_offset[OFFSET_BITS-1:2];   // Assign the word offset to be the offset bits divied by 4

// ==============================================================================
// Hit Detection Logic
// ==============================================================================
logic [WAYS-1:0] way_hit;       // Signal to store which way has requested instruction
logic [WAYS-1:0] way_valid;     // Signal to store which way has valid entry

always_comb begin
    // Check all ways best on set and tag
    for(int i = 0; i < WAYS; i++) begin
        way_hit[i] = valid[i][req_set] && (tags[i][req_set] == req_tag); // Hit if the data is valid and the tags match
        way_valid[i] = valid[i][req_set];                                // Store the validity of the entry
    end
    cache_hit = |way_hit;   // Update cache hit signal
end

// ==============================================================================
// Miss Handling
// ==============================================================================
logic miss_handling;
logic [31:0] miss_pc;

always_ff @(posedge clk or negedge rst_n) begin
    if(!rst_n) begin
        // Active low reset is enabled
        miss_handling <= 0;
        miss_pc <= '0;
    end else begin
        // If there is a fetch request, and cache has not been hit and miss not already handled
        if(fetch_req && !cache_hit && !miss_handling) begin
            miss_handling <= 1;                                                      // Update miss handling status
            miss_pc <= pc;                                                           // Update missed program counter value
            mem_read <= 1;                                                           // Request read from memory
            mem_addr <= {pc[`INSTRUCTION_WIDTH-1:OFFSET_BITS], {OFFSET_BITS{1'b0}}}; // Update memory address to read. Reads the entire block
        end else if(miss_handling && mem_valid) begin
            // If the miss has already been handled and the memory read is valid, we can clear status bits
            miss_handling <= 0;
            mem_read <= 0;
        end
    end
end

// ==============================================================================
// Cache Hit Logic and LRU Update and Handling
// ==============================================================================
always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        // Clear everything for each way and set
        for (int w = 0; w < WAYS; w++) begin
            for (int s = 0; s < SETS; s++) begin
              valid[w][s] <= 1'b0;
              tags[w][s]  <= '0;
              data[w][s]  <= '0;
              lru[w][s]   <= 1'b0;
            end
        end
          instruction   <= '0;
          miss_handling <= 0;
          miss_pc       <= '0;
          mem_read      <= 0;
          mem_addr      <= '0;
    end else begin
        if (fetch_req && cache_hit) begin
            // If cache has been hit and fetch request is valid
            for(int i = 0; i < WAYS; i++) begin
                if (way_hit[i]) begin
                    // If current way has been hit, mark it as most recently used
                    lru[i][req_set] <= 1'b1;
                end else begin
                    // If it is not the hit way, mark it as 0 to dispose it next time
                    lru[i][req_set] <= 1'b0;
                end

                // Update instruction output
                if(way_hit[i]) begin
                    instruction <= data[i][req_set][word_offset*`INSTRUCTION_WIDTH +: `INSTRUCTION_WIDTH]; // Select the instruction output based on which way is hit
                end
            end
        end else if (miss_handling && mem_valid) begin
            // Handle a cache miss when we get valid data
            if(lru[0][req_set]) begin
                // Replace way 1 since way 0 is recently used
                data[1][req_set] <= mem_data;
                tags[1][req_set] <= miss_pc[`INSTRUCTION_WIDTH-1:`INSTRUCTION_WIDTH-TAG_BITS];
                valid[1][req_set] <= 1'b1;
                lru[1][req_set] <= 1'b1; // Mark as most recently updated
                lru[0][req_set] <= 1'b0; // Mark other as least recently used
            end else begin
                // Replace way0 since it is the LRU
                data[0][req_set] <= mem_data;
                tags[0][req_set] <= miss_pc[`INSTRUCTION_WIDTH-1:`INSTRUCTION_WIDTH-TAG_BITS];
                valid[0][req_set] <= 1'b1;
                lru[0][req_set] <= 1'b1; // Mark as most recently updated
                lru[1][req_set] <= 1'b0; // Mark other as least recently used
            end

            // Update the output with the newly loaded instruction. Divide the program counter by 2
            instruction <= mem_data[miss_pc[OFFSET_BITS-1:2]*`INSTRUCTION_WIDTH +: `INSTRUCTION_WIDTH];
        end
    end
end

endmodule
