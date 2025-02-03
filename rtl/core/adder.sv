`include "riscv_pkg.sv"
import riscv_pkg::XLEN, riscv_pkg::ILEN;

module adder (
    input logic [XLEN-1:0] in_0, in_1;
    output logic [XLEN-1:0] out;
);

assign out = $signed(in_0) + $signed(in_1);

endmodule
