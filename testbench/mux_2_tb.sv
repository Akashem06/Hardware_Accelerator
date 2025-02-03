/* verilator lint_off STMTDLY */
`include "riscv_pkg.sv"
import riscv_pkg::XLEN;

module mux_2_tb (
    input  logic clk,
    output logic [XLEN-1:0] out
);
    logic [XLEN-1:0] in_0, in_1;
    logic selector;

    // Instantiate the DUT (mux_2)
    mux_2 dut (
        .in_0(in_0),
        .in_1(in_1),
        .selector(selector),
        .out(out)
    );

    initial begin
        in_0 = 32'h0000_1100;
        in_1 = 32'h0000_0011;
        selector = 0;

        // Test case 1: Select in_0
        selector = 0;
        #10;
        $display("Test 1: Selector = %b, Out = %h", selector, out);
        if (out !== in_0) $error("Test 1 failed!");

        // Test case 2: Select in_1
        selector = 1;
        #10;
        $display("Test 2: Selector = %b, Out = %h", selector, out);
        if (out !== in_1) $error("Test 2 failed!");

        $display("All tests passed!");
        $finish;
    end

endmodule
/* verilator lint_on STMTDLY */