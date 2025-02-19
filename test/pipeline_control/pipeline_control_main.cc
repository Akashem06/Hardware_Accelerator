#include "pipeline_control_tb.h"

double sc_time_stamp() {
  return 0;
}

int main(int argc, char** argv) {
  Verilated::commandArgs(argc, argv);
  pipeline_controlTest tb;

  std::cout << "============ Running pipeline_control Testbench ============\n";
  return tb.run_all_tests() ? 0 : 1;
}