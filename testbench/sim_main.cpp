#include "Vmux_2_tb.h"
#include "verilated.h"
#include "verilated_vcd_c.h"

double sc_time_stamp() {
    return 0;
}

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);

    Vmux_2_tb* tb = new Vmux_2_tb;

    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    tb->trace(tfp, 99);
    tfp->open("dump.vcd");

    tb->clk = 0;

    for (int i = 0; i < 100; i++) {
        tb->clk = !tb->clk;
        tb->eval();
        tfp->dump(i * 10);
    }

    tfp->close();
    delete tb;
    delete tfp;

    return 0;
}