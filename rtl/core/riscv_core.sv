// rtl/core/riscv_core.sv

`include "riscv_pkg.sv"
import riscv_pkg::XLEN, riscv_pkg::ILEN;

module riscv_core (
    input  logic clk,
    input  logic rst_n,
    
    // Instruction memory interface
    output logic [XLEN-1:0] imem_addr,
    input  logic [ILEN-1:0] imem_data,
    
    // Data memory interface
    output logic [XLEN-1:0] dmem_addr,
    output logic [XLEN-1:0] dmem_wdata,
    input  logic [XLEN-1:0] dmem_rdata,
    output logic           dmem_we
);

    // Program counter
    logic [XLEN-1:0] pc;

    // Update PC on clock
    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n)         // If the negative reset is low (ACTIVE LOW)
            pc <= '0;       // Reset PC to 0
        else
            pc <= pc + 4;   // Increment PC by 4 bytes (word-aligned)
    end

    // Connect PC to instruction memory address
    assign imem_addr = pc;

    // For now, no data memory operations
    assign dmem_addr = '0;
    assign dmem_wdata = '0;
    assign dmem_we = '0;

endmodule
