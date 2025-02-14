#pragma once

#include "../hardware_test.h"
#include "Vmultiplier32.h"

class multiplier32Test : public HardwareTestBase {
 protected:
  std::unique_ptr<Vmultiplier32> dut_;

 public:
  multiplier32Test() : HardwareTestBase() {
    dut_ = std::make_unique<Vmultiplier32>();
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
    std::cout << "\nRunning Test Case 1 (16 * 2)..." << std::endl;

    dut_->in_0 = 0x00000010;  // 16 in decimal
    dut_->in_1 = 0x00000002;  // 2 in decimal

    dut_->eval();

    // Expected 16 * 2 = 32
    if (dut_->out != 0x00000020) {
      std::cout << "Test Case 1 Failed: Expected output to be 0x20 but got " << std::hex
                << dut_->out << std::dec << std::endl;
      return false;
    }

    std::cout << "Test Case 1 Passed" << std::endl;
    return true;
  }

  bool test_case_2() {
    std::cout << "\nRunning Test Case 2 (16 * -16)..." << std::endl;

    dut_->in_0 = 0x00000010;  // 16 in decimal
    dut_->in_1 = 0xFFFFFFF0;  // -16 in decimal (2's complement)

    dut_->eval();

    // Expected 16 * (-16) = -256
    if (dut_->out != 0xFFFFFF00) {
      std::cout << "Test Case 2 Failed: Expected output to be 0xFFFFFFC0 but got " << std::hex
                << dut_->out << std::dec << std::endl;
      return false;
    }

    std::cout << "Test Case 2 Passed" << std::endl;
    return true;
  }

  bool test_case_3() {
    std::cout << "\nRunning Test Case 3 (-16 * -32)..." << std::endl;

    dut_->in_0 = 0xFFFFFFF0;  // -16 in decimal
    dut_->in_1 = 0xFFFFFFE0;  // -32 in decimal

    dut_->eval();

    // Expected (-16) * (-32) = 512
    if (dut_->out != 0x00000200) {
      std::cout << "Test Case 3 Failed: Expected output to be 0x00000480 but got " << std::hex
                << dut_->out << std::dec << std::endl;
      return false;
    }

    std::cout << "Test Case 3 Passed" << std::endl;
    return true;
  }

  bool test_case_4() {
    std::cout << "\nRunning Test Case 4 (16 * 0)..." << std::endl;

    dut_->in_0 = 0x00000010;  // 16 in decimal
    dut_->in_1 = 0x00000000;  // 0 in decimal

    dut_->eval();

    // Expected 16 * 0 = 0
    if (dut_->out != 0x00000000) {
      std::cout << "Test Case 4 Failed: Expected output to be 0x0 but got " << std::hex << dut_->out
                << std::dec << std::endl;
      return false;
    }

    std::cout << "Test Case 4 Passed" << std::endl;
    return true;
  }
};
