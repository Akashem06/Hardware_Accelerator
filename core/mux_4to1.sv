`include "global_defines.sv"

module mux_4to1 (
    input  logic [`INSTRUCTION_WIDTH-1:0] in_0, in_1, in_2, in_3,
    input logic [1:0] selector,
    output logic [`INSTRUCTION_WIDTH-1:0] out
);

always_comb begin
    case (selector)
    2'b00: out = in_0;
    2'b01: out = in_1;
    2'b10: out = in_2;
    2'b11: out = in_3;
    default: out = in_0;
    endcase
end

endmodule
