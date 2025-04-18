#include "execution_unit_tb.h"

double sc_time_stamp() {
  return 0;
}

int main(int argc, char** argv) {
  Verilated::commandArgs(argc, argv);
  execution_unitTest tb;

  std::cout << "============ Running execution_unit Testbench ============\n";
  return tb.run_all_tests() ? 0 : 1;
}