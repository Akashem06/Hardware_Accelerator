#pragma once

#include "../hardware_test.h"
#include "Vexecution_unit.h"

class execution_unitTest : public HardwareTestBase {
 protected:
  std::unique_ptr<Vexecution_unit> dut_;

 public:
  execution_unitTest() : HardwareTestBase() {
    dut_ = std::make_unique<Vexecution_unit>();
  }

  void eval_clock_low() override {
    dut_->clk = 0;
    dut_->eval();
  }

  void eval_clock_high() override {
    dut_->clk = 1;
    dut_->eval();
  }

  void reset() override {
    dut_->rst_n = 0;
    tick(5);
    dut_->rst_n = 1;
    tick(1);
  }

  bool run_all_tests() override {
    bool all_passed = true;
    reset();

    all_passed &= test_case_1();
    all_passed &= test_case_2();
        
    std::cout << "\nFinal Result: " << (all_passed ? "ALL TESTS PASSED" : "SOME TESTS FAILED") << std::endl;
        
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