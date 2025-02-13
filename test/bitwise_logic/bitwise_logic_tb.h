#pragma once

#include <bitset>
#include <iostream>

#include "../hardware_test.h"
#include "Vbitwise_logic.h"

class bitwise_logicTest : public HardwareTestBase {
 protected:
  std::unique_ptr<Vbitwise_logic> dut_;

 public:
  bitwise_logicTest() : HardwareTestBase() {
    dut_ = std::make_unique<Vbitwise_logic>();
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
    std::cout << "\nRunning Test Case 1 (AND operation)..." << std::endl;

    dut_->in_0 = 0b1100;
    dut_->in_1 = 0b1010;
    dut_->op_selector = 0b00;
    dut_->eval();

    uint32_t expected_out = 0b1000;
    if (dut_->out != expected_out) {
      std::cout << "Test case 1 failed. Expected: " << std::bitset<32>(expected_out)
                << " but got: " << std::bitset<32>(dut_->out) << std::endl;
      return false;
    }

    return true;
  }

  bool test_case_2() {
    std::cout << "\nRunning Test Case 2 (OR operation)..." << std::endl;

    dut_->in_0 = 0b1100;
    dut_->in_1 = 0b1010;
    dut_->op_selector = 0b01;
    dut_->eval();

    uint32_t expected_out = 0b1110;
    if (dut_->out != expected_out) {
      std::cout << "Test case 2 failed. Expected: " << std::bitset<32>(expected_out)
                << " but got: " << std::bitset<32>(dut_->out) << std::endl;
      return false;
    }

    return true;
  }

  bool test_case_3() {
    std::cout << "\nRunning Test Case 3 (XOR operation)..." << std::endl;

    dut_->in_0 = 0b1100;
    dut_->in_1 = 0b1010;
    dut_->op_selector = 0b10;
    dut_->eval();

    uint32_t expected_out = 0b0110;
    if (dut_->out != expected_out) {
      std::cout << "Test case 3 failed. Expected: " << std::bitset<32>(expected_out)
                << " but got: " << std::bitset<32>(dut_->out) << std::endl;
      return false;
    }

    return true;
  }

  bool test_case_4() {
    std::cout << "\nRunning Test Case 4 (NOT operation)..." << std::endl;

    dut_->in_0 = 0b1100;
    dut_->op_selector = 0b11;
    dut_->eval();

    uint32_t expected_out = ~0b1100;
    if (dut_->out != expected_out) {
      std::cout << "Test case 4 failed. Expected: " << std::bitset<32>(expected_out)
                << " but got: " << std::bitset<32>(dut_->out) << std::endl;
      return false;
    }

    return true;
  }

  bool test_case_5() {
    std::cout << "\nRunning Test Case 5 (AND with zeroes)..." << std::endl;

    dut_->in_0 = 0b0000;
    dut_->in_1 = 0b1111;
    dut_->op_selector = 0b00;
    dut_->eval();

    uint32_t expected_out = 0b0000;
    if (dut_->out != expected_out) {
      std::cout << "Test case 5 failed. Expected: " << std::bitset<32>(expected_out)
                << " but got: " << std::bitset<32>(dut_->out) << std::endl;
      return false;
    }

    return true;
  }

  bool test_case_6() {
    std::cout << "\nRunning Test Case 6 (OR with zeroes)..." << std::endl;

    dut_->in_0 = 0b0000;
    dut_->in_1 = 0b1111;
    dut_->op_selector = 0b01;
    dut_->eval();

    uint32_t expected_out = 0b1111;
    if (dut_->out != expected_out) {
      std::cout << "Test case 6 failed. Expected: " << std::bitset<32>(expected_out)
                << " but got: " << std::bitset<32>(dut_->out) << std::endl;
      return false;
    }

    return true;
  }

  bool test_case_7() {
    std::cout << "\nRunning Test Case 7 (XOR with zeroes)..." << std::endl;

    dut_->in_0 = 0b0000;
    dut_->in_1 = 0b1111;
    dut_->op_selector = 0b10;
    dut_->eval();

    uint32_t expected_out = 0b1111;
    if (dut_->out != expected_out) {
      std::cout << "Test case 7 failed. Expected: " << std::bitset<32>(expected_out)
                << " but got: " << std::bitset<32>(dut_->out) << std::endl;
      return false;
    }

    return true;
  }

  bool test_case_8() {
    std::cout << "\nRunning Test Case 8 (NOT with all ones)..." << std::endl;

    dut_->in_0 = 0b1111;
    dut_->op_selector = 0b11;
    dut_->eval();

    uint32_t expected_out = ~0b1111;
    if (dut_->out != expected_out) {
      std::cout << "Test case 8 failed. Expected: " << std::bitset<32>(expected_out)
                << " but got: " << std::bitset<32>(dut_->out) << std::endl;
      return false;
    }

    return true;
  }

  bool test_case_9() {
    std::cout << "\nRunning Test Case 9 (AND with all ones)..." << std::endl;

    dut_->in_0 = 0b1111;
    dut_->in_1 = 0b1111;
    dut_->op_selector = 0b00;
    dut_->eval();

    uint32_t expected_out = 0b1111;
    if (dut_->out != expected_out) {
      std::cout << "Test case 9 failed. Expected: " << std::bitset<32>(expected_out)
                << " but got: " << std::bitset<32>(dut_->out) << std::endl;
      return false;
    }

    return true;
  }

  bool test_case_10() {
    std::cout << "\nRunning Test Case 10 (XOR with all ones)..." << std::endl;

    dut_->in_0 = 0b1111;
    dut_->in_1 = 0b1111;
    dut_->op_selector = 0b10;
    dut_->eval();

    uint32_t expected_out = 0b0000;
    if (dut_->out != expected_out) {
      std::cout << "Test case 10 failed. Expected: " << std::bitset<32>(expected_out)
                << " but got: " << std::bitset<32>(dut_->out) << std::endl;
      return false;
    }

    return true;
  }
};
