#include "bitwise_logic_tb.h"

double sc_time_stamp() {
  return 0;
}

int main(int argc, char **argv) {
  Verilated::commandArgs(argc, argv);
  bitwise_logicTest tb;

  std::cout << "============ Running bitwise_logic Testbench ============\n";
  return tb.run_all_tests() ? 0 : 1;
}