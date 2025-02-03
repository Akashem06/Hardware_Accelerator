`include "riscv_pkg.sv"
import riscv_pkg::XLEN, riscv_pkg::ILEN;

module mux_2 (
    input  logic [XLEN-1:0] in_0, in_1,
    input logic selector,
    output logic [XLEN-1:0] out
);

assign out = selector ? in_1 : in_0;

endmodule
