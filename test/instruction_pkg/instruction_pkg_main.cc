#include "instruction_pkg_tb.h"

double sc_time_stamp() {
  return 0;
}

int main(int argc, char **argv) {
  Verilated::commandArgs(argc, argv);
  instruction_pkgTest tb;

  std::cout << "============ Running instruction_pkg Testbench ============\n";
  return tb.run_all_tests() ? 0 : 1;
}