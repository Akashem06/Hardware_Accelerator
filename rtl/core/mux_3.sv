`include "riscv_pkg.sv"
import riscv_pkg::XLEN, riscv_pkg::ILEN;

module mux_3 (
    input  logic [XLEN-1:0] in_0, in_1, in_2,
    input logic [1:0] selector,
    output logic [XLEN-1:0] out
);

always_comb begin
    case (selector)
    2'b00: out = in_0;
    2'b01: out = in_1;
    2'b10: out = in_2;
    default: out = in_0;
    endcase
end

endmodule
