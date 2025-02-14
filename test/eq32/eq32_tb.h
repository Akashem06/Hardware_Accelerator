#pragma once

#include "../hardware_test.h"
#include "Veq32.h"

class eq32Test : public HardwareTestBase {
 protected:
  std::unique_ptr<Veq32> dut_;

 public:
  eq32Test() : HardwareTestBase() {
    dut_ = std::make_unique<Veq32>();
  }

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
    std::cout << "\nRunning Test Case 1 (DEAD == DEAD)..." << std::endl;

    dut_->in_0 = 0xDEAD;
    dut_->in_1 = 0xDEAD;

    dut_->eval();

    // Outputs 1 if the two values are equal
    if (dut_->out != 1) {
      std::cout << "Test Case 1 Failed: Expected output to be 1 but got " << dut_->out << std::endl;
      return false;
    }

    return true;
  }

  bool test_case_2() {
    std::cout << "\nRunning Test Case 2 (DEAD != BEEF)..." << std::endl;

    dut_->in_0 = 0xDEAD;
    dut_->in_1 = 0xBEEF;

    dut_->eval();

    // Outputs 0 if the two values are not equal
    if (dut_->out != 0) {
      std::cout << "Test Case 2 Failed: Expected output to be 0 but got " << dut_->out << std::endl;
      return false;
    }

    return true;
  }
};
