#include "shifter_tb.h"

double sc_time_stamp() {
  return 0;
}

int main(int argc, char **argv) {
  Verilated::commandArgs(argc, argv);
  shifterTest tb;

  std::cout << "============ Running shifter Testbench ============\n";
  return tb.run_all_tests() ? 0 : 1;
}