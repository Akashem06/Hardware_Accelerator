`include "global_defines.sv"

module shifter (
    input logic [`INSTRUCTION_WIDTH-1:0] in_0,
    input logic [4:0] shift_amount,
    input logic [1:0] op_selector, // 00 = Logical shift left, 01 = Logical shift right, 10 = Arithmetic shift right
    output logic [`INSTRUCTION_WIDTH-1:0] out
);

always_comb begin
    case (op_selector)
    2'b00: out = (in_0 << shift_amount);
    2'b01: out = (in_0 >> shift_amount);
    2'b10: out = ($signed(in_0) >>> shift_amount);
    default: out = 32'b0;
    endcase
end

endmodule
