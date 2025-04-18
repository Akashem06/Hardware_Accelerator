#include "register_file_tb.h"

double sc_time_stamp() {
  return 0;
}

int main(int argc, char **argv) {
  Verilated::commandArgs(argc, argv);
  register_fileTest tb;

  std::cout << "============ Running register_file Testbench ============\n";
  return tb.run_all_tests() ? 0 : 1;
}