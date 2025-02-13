#pragma once

#include <bitset>
#include <iostream>

#include "../hardware_test.h"
#include "Vcomparator.h"

class comparatorTest : public HardwareTestBase {
 protected:
  std::unique_ptr<Vcomparator> dut_;

 public:
  comparatorTest() : HardwareTestBase() {
    dut_ = std::make_unique<Vcomparator>();
  }

  void eval_clock_low() override {}

  void eval_clock_high() override {}

  void reset() override {
    // Reset logic if needed
  }

  bool run_all_tests() override {
    bool all_passed = true;
    reset();

    all_passed &= test_case_1();
    all_passed &= test_case_2();
    all_passed &= test_case_3();
    all_passed &= test_case_4();
    all_passed &= test_case_5();
    all_passed &= test_case_6();
    all_passed &= test_case_7();
    all_passed &= test_case_8();
    all_passed &= test_case_9();
    all_passed &= test_case_10();

    std::cout << "\nFinal Result: " << (all_passed ? "ALL TESTS PASSED" : "SOME TESTS FAILED")
              << std::endl;

    return all_passed;
  }

 private:
  bool test_case_1() {
    std::cout << "\nRunning Test Case 1 (in_0 < in_1)..." << std::endl;

    dut_->in_0 = 0b0100;
    dut_->in_1 = 0b1000;
    dut_->eval();

    if (dut_->out_lt != true || dut_->out_gt != false || dut_->out_eq != false) {
      std::cout << "Test case 1 failed." << std::endl;
      return false;
    }

    return true;
  }

  bool test_case_2() {
    std::cout << "\nRunning Test Case 2 (in_0 > in_1)..." << std::endl;

    dut_->in_0 = 0b1000;
    dut_->in_1 = 0b0100;
    dut_->eval();

    if (dut_->out_lt != false || dut_->out_gt != true || dut_->out_eq != false) {
      std::cout << "Test case 2 failed." << std::endl;
      return false;
    }

    return true;
  }

  bool test_case_3() {
    std::cout << "\nRunning Test Case 3 (in_0 == in_1)..." << std::endl;

    dut_->in_0 = 0b1010;
    dut_->in_1 = 0b1010;
    dut_->eval();

    if (dut_->out_lt != false || dut_->out_gt != false || dut_->out_eq != true) {
      std::cout << "Test case 3 failed." << std::endl;
      return false;
    }

    return true;
  }

  bool test_case_4() {
    std::cout << "\nRunning Test Case 4 (in_0 < in_1 with zeros)..." << std::endl;

    dut_->in_0 = 0b0000;
    dut_->in_1 = 0b1111;
    dut_->eval();

    if (dut_->out_lt != true || dut_->out_gt != false || dut_->out_eq != false) {
      std::cout << "Test case 4 failed." << std::endl;
      return false;
    }

    return true;
  }

  bool test_case_5() {
    std::cout << "\nRunning Test Case 5 (in_0 > in_1 with zeros)..." << std::endl;

    dut_->in_0 = 0b1111;
    dut_->in_1 = 0b0000;
    dut_->eval();

    if (dut_->out_lt != false || dut_->out_gt != true || dut_->out_eq != false) {
      std::cout << "Test case 5 failed." << std::endl;
      return false;
    }

    return true;
  }

  bool test_case_6() {
    std::cout << "\nRunning Test Case 6 (in_0 == in_1 with zeros)..." << std::endl;

    dut_->in_0 = 0b0000;
    dut_->in_1 = 0b0000;
    dut_->eval();

    if (dut_->out_lt != false || dut_->out_gt != false || dut_->out_eq != true) {
      std::cout << "Test case 6 failed." << std::endl;
      return false;
    }

    return true;
  }

  bool test_case_7() {
    std::cout << "\nRunning Test Case 7 (maximum value for in_0 and in_1)" << std::endl;

    dut_->in_0 = ~0;  // 32-bit all ones
    dut_->in_1 = ~0;
    dut_->eval();

    if (dut_->out_lt != false || dut_->out_gt != false || dut_->out_eq != true) {
      std::cout << "Test case 7 failed." << std::endl;
      return false;
    }

    return true;
  }

  bool test_case_8() {
    std::cout << "\nRunning Test Case 8 (in_0 < in_1 with maximum value for in_0)" << std::endl;

    dut_->in_0 = ~0;
    dut_->in_1 = 0b0001;
    dut_->eval();

    if (dut_->out_lt != false || dut_->out_gt != true || dut_->out_eq != false) {
      std::cout << "Test case 8 failed." << std::endl;
      return false;
    }

    return true;
  }

  bool test_case_9() {
    std::cout << "\nRunning Test Case 9 (in_0 > in_1 with maximum value for in_1)" << std::endl;

    dut_->in_0 = 0b0001;
    dut_->in_1 = ~0;
    dut_->eval();

    if (dut_->out_lt != true || dut_->out_gt != false || dut_->out_eq != false) {
      std::cout << "Test case 9 failed." << std::endl;
      return false;
    }

    return true;
  }

  bool test_case_10() {
    std::cout << "\nRunning Test Case 10 (both inputs are zero)" << std::endl;

    dut_->in_0 = 0b0000;
    dut_->in_1 = 0b0000;
    dut_->eval();

    if (dut_->out_lt != false || dut_->out_gt != false || dut_->out_eq != true) {
      std::cout << "Test case 10 failed." << std::endl;
      return false;
    }

    return true;
  }
};
