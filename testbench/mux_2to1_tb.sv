`include "global_defines.sv"

module mux_2to1_tb (
    input  logic clk,
    output logic [`INSTRUCTION_WIDTH-1:0] out
);
    logic [`INSTRUCTION_WIDTH-1:0] in_0, in_1;
    logic selector;

    // A state variable to sequence our tests
    typedef enum logic [1:0] { INIT, TEST1, TEST2 } state_t;
    state_t state;

    // Instantiate the DUT (mux_2)
    mux_2to1 dut (
        .in_0(in_0),
        .in_1(in_1),
        .selector(selector),
        .out(out)
    );

    initial begin
        in_0 = 32'h0000_1100;
        in_1 = 32'h0000_0011;
        selector = 0;
        state = INIT;
    end

    // Clock-driven state machine to drive test cases
    always @(posedge clk) begin
        case (state)
            INIT: begin
                state <= TEST1;
            end

            TEST1: begin
                // Test case 1: selector should be 0, so out should equal in_0
                if (out !== in_0) begin
                    $error("Test 1 failed! Expected %h but got %h", in_0, out);
                end else begin
                    $display("Test 1: Selector = %b, Out = %h", selector, out);
                end

                // Prepare for test case 2
                selector <= 1;
                state <= TEST2;
            end

            TEST2: begin
                // Test case 2: now selector is 1, so out should equal in_1
                if (out !== in_1) begin
                    $error("Test 2 failed! Expected %h but got %h", in_1, out);
                end else begin
                    $display("Test 2: Selector = %b, Out = %h", selector, out);
                end

                $display("All tests passed!");
                $finish;
            end

            default: state <= INIT;
        endcase
    end

endmodule
