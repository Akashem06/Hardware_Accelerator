#include "instruction_fetch_tb.h"

double sc_time_stamp() {
  return 0;
}

int main(int argc, char **argv) {
  Verilated::commandArgs(argc, argv);
  instruction_fetchTest tb;

  std::cout << "============ Running instruction_fetch Testbench ============\n";
  return tb.run_all_tests() ? 0 : 1;
}