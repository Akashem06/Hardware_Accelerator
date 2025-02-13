#pragma once

#include "../hardware_test.h"
#include "Vmux_4to1.h"

class mux_4to1Test : public HardwareTestBase {
 protected:
  std::unique_ptr<Vmux_4to1> dut_;

 public:
  mux_4to1Test() : HardwareTestBase() {
    dut_ = std::make_unique<Vmux_4to1>();
  }

  void eval_clock_low() override {}

  void eval_clock_high() override {}

  void reset() override {}

  bool run_all_tests() override {
    bool all_passed = true;
    reset();

    all_passed &= test_case_1();
    all_passed &= test_case_2();
    all_passed &= test_case_3();
    all_passed &= test_case_4();

    std::cout << "\nFinal Result: " << (all_passed ? "ALL TESTS PASSED" : "SOME TESTS FAILED")
              << std::endl;

    return all_passed;
  }

 private:
  bool test_case_1() {
    std::cout << "\nRunning Test Case 1..." << std::endl;

    dut_->in_0 = 0xABCD;
    dut_->in_1 = 0x1234;
    dut_->in_2 = 0xDEAD;
    dut_->in_3 = 0xBEEF;

    dut_->selector = 0b00;

    dut_->eval();

    // When selector is 0, we should output input 0
    if (dut_->out != dut_->in_0) {
      std::cout << "Test Case 1 Failed: Expected output to be " << dut_->in_0 << " but got "
                << dut_->out << std::endl;
      return false;
    }

    std::cout << "Test Case 1 Passed" << std::endl;
    return true;
  }

  bool test_case_2() {
    std::cout << "\nRunning Test Case 2..." << std::endl;

    dut_->in_0 = 0xABCD;
    dut_->in_1 = 0x1234;
    dut_->in_2 = 0xDEAD;
    dut_->in_3 = 0xBEEF;

    dut_->selector = 0b01;

    dut_->eval();

    // When selector is 1, we should output input 1
    if (dut_->out != dut_->in_1) {
      std::cout << "Test Case 2 Failed: Expected output to be " << dut_->in_1 << " but got "
                << dut_->out << std::endl;
      return false;
    }

    std::cout << "Test Case 2 Passed" << std::endl;
    return true;
  }

  bool test_case_3() {
    std::cout << "\nRunning Test Case 3..." << std::endl;

    dut_->in_0 = 0xABCD;
    dut_->in_1 = 0x1234;
    dut_->in_2 = 0xDEAD;
    dut_->in_3 = 0xBEEF;

    dut_->selector = 0b10;

    dut_->eval();

    // When selector is 2, we should output input 2
    if (dut_->out != dut_->in_2) {
      std::cout << "Test Case 3 Failed: Expected output to be " << dut_->in_2 << " but got "
                << dut_->out << std::endl;
      return false;
    }

    return true;
  }

  bool test_case_4() {
    std::cout << "\nRunning Test Case 4..." << std::endl;

    dut_->in_0 = 0xABCD;
    dut_->in_1 = 0x1234;
    dut_->in_2 = 0xDEAD;
    dut_->in_3 = 0xBEEF;

    dut_->selector = 0b11;

    dut_->eval();

    // When selector is 3, we should output input 3
    if (dut_->out != dut_->in_3) {
      std::cout << "Test Case 4 Failed: Expected output to be " << dut_->in_3 << " but got "
                << dut_->out << std::endl;
      return false;
    }

    return true;
  }
};
