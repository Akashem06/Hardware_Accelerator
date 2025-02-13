`include "global_defines.sv"

module comparator (
    input logic [`INSTRUCTION_WIDTH-1:0] in_0, in_1,
    output logic out_lt, out_gt, out_eq
);

always_comb begin
    out_lt = (in_0 < in_1);
    out_gt = (in_0 > in_1);
    out_eq = (in_0 == in_1);
end

endmodule
