#include "multiplier32_tb.h"

double sc_time_stamp() {
  return 0;
}

int main(int argc, char **argv) {
  Verilated::commandArgs(argc, argv);
  multiplier32Test tb;

  std::cout << "============ Running multiplier32 Testbench ============\n";
  return tb.run_all_tests() ? 0 : 1;
}