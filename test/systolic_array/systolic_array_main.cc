#include "systolic_array_tb.h"

double sc_time_stamp() {
  return 0;
}

int main(int argc, char** argv) {
  Verilated::commandArgs(argc, argv);
  systolic_arrayTest tb;

  std::cout << "============ Running systolic_array Testbench ============\n";
  return tb.run_all_tests() ? 0 : 1;
}