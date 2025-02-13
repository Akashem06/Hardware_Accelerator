`include "global_defines.sv"

module eq32 (
    input logic [`INSTRUCTION_WIDTH-1:0] in_0, in_1,
    output logic out
);

assign out = (in_0 == in_1);

endmodule
