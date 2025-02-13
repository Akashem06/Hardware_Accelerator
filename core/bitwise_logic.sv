`include "global_defines.sv"

module bitwise_logic (
    input logic [`INSTRUCTION_WIDTH-1:0] in_0, in_1,
    input logic [1:0] op_selector, // 0b00 = AND, 0b01 = OR, 0b10 = XOR, 0b11 = NOT(in_0)
    output logic [`INSTRUCTION_WIDTH-1:0] out
);

always_comb begin
    case (op_selector)
    2'b00: out = (in_0 & in_1);
    2'b01: out = (in_0 | in_1);
    2'b10: out = (in_0 ^ in_1);
    2'b11: out = (~in_0);
    default: out = 32'b0;
    endcase
end

endmodule
