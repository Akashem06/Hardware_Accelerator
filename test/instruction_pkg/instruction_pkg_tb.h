#pragma once

#include "../hardware_test.h"
#include "Vinstruction_pkg.h"

class instruction_pkgTest : public HardwareTestBase {
 public:
  instruction_pkgTest() : HardwareTestBase() {}

  void eval_clock_low() override {}

  void eval_clock_high() override {}

  void reset() override {}

  bool run_all_tests() override {
    bool all_passed = true;
    reset();

    all_passed &= test_case_1();
    all_passed &= test_case_2();

    std::cout << "\nFinal Result: " << (all_passed ? "ALL TESTS PASSED" : "SOME TESTS FAILED")
              << std::endl;

    return all_passed;
  }

 private:
  bool test_case_1() {
    std::cout << "\nRunning Test Case 1..." << std::endl;
    /* Implement test case 1 */
    return true;
  }

  bool test_case_2() {
    std::cout << "\nRunning Test Case 2..." << std::endl;
    /* Implement test case 2 */
    return true;
  }
};
