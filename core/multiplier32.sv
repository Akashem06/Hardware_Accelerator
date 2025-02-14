`include "global_defines.sv"

module multiplier32 (
    input logic [`INSTRUCTION_WIDTH-1:0] in_0, in_1,
    output logic [`INSTRUCTION_WIDTH-1:0] out
);

assign out = in_0 * in_1;

endmodule
