#include "global_defines_tb.h"

double sc_time_stamp() {
  return 0;
}

int main(int argc, char **argv) {
  Verilated::commandArgs(argc, argv);
  global_definesTest tb;

  std::cout << "============ Running global_defines Testbench ============\n";
  return tb.run_all_tests() ? 0 : 1;
}