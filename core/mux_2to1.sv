`include "global_defines.sv"

module mux_2to1 (
    input  logic [`INSTRUCTION_WIDTH-1:0] in_0, in_1,
    input logic selector,
    output logic [`INSTRUCTION_WIDTH-1:0] out
);

assign out = selector ? in_1 : in_0;

endmodule
