`include "global_defines.sv"

module register_file (
    input  logic        clk,        // Clock signal
    input  logic        rst_n,      // Active low reset

    // Read ports
    input logic [`NUM_REGS_BIT_COUNT-1:0]   rs1_addr, // Register source 1 address
    input logic [`NUM_REGS_BIT_COUNT-1:0]   rs2_addr, // Register source 2 address
    output logic[`INSTRUCTION_WIDTH-1:0]    rs1_data, // Register source 1 data
    output logic[`INSTRUCTION_WIDTH-1:0]    rs2_data, // Register source 2 data

    // Write ports
    input logic                             rd_write_en, // Register write enable
    input logic [`NUM_REGS_BIT_COUNT-1:0]   rd_addr,     // Register destination address
    input logic [`INSTRUCTION_WIDTH-1:0]    rd_data      // Register destination data
);

logic [`INSTRUCTION_WIDTH-1:0] registers [`NUM_REGS-1:0];

assign rs1_data = registers[rs1_addr];
assign rs2_data = registers[rs2_addr];

always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        for (int i = 0; i < `NUM_REGS; i++) begin
            registers[i] <= '0;
        end
    end else if (rd_write_en) begin
        registers[rd_addr] <= rd_data;
    end
end

endmodule
