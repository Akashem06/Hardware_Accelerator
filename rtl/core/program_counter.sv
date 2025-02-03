`include "riscv_pkg.sv"
import riscv_pkg::XLEN, riscv_pkg::ILEN;

module program_coutner (
    input  logic clk,
    input  logic rst_n,
    input  logic [XLEN-1:0] next_pc,
    output logic [XLEN-1:0] pc
)

always_ff @(posedge clk or negedge rst_n ) begin
    if (!rst_n)
        pc <= 0 // Reset
    else
        pc <= next_pc
end

endmodule;
