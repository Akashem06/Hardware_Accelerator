`include "global_defines.sv"

module adder32 (
    input logic [`INSTRUCTION_WIDTH-1:0] in_0, in_1,
    output logic [`INSTRUCTION_WIDTH-1:0] out
);

assign out = $signed(in_0) + $signed(in_1);

endmodule
