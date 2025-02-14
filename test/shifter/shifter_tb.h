#pragma once

#include "../hardware_test.h"
#include "Vshifter.h"

class shifterTest : public HardwareTestBase {
 protected:
  std::unique_ptr<Vshifter> dut_;

 public:
  shifterTest() : HardwareTestBase() {
    dut_ = std::make_unique<Vshifter>();
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

    return all_passed;
  }

 private:
  bool test_case_1() {
    std::cout << "\nRunning Test Case 1 (0xFF << 4)..." << std::endl;

    dut_->in_0 = 0x000000FF;
    dut_->shift_amount = 4;
    dut_->op_selector = 0b00;  // LSL

    dut_->eval();

    if (dut_->out != 0x000000FF << 4) {
      std::cout << "Test Case 1 Failed: Expected output to be 0xFF but got " << std::hex
                << dut_->out << std::dec << std::endl;
      return false;
    }

    return true;
  }

  bool test_case_2() {
    std::cout << "\nRunning Test Case 2 (0x80000000 >> 1)..." << std::endl;

    dut_->in_0 = 0x80000000;
    dut_->shift_amount = 1;
    dut_->op_selector = 0b01;  // LSR

    dut_->eval();

    if (dut_->out != 0x80000000 >> 1) {
      std::cout << "Test Case 2 Failed: Expected output to be 0x40000000 but got " << std::hex
                << dut_->out << std::dec << std::endl;
      return false;
    }

    return true;
  }

  bool test_case_3() {
    std::cout << "\nRunning Test Case 3 (0x80000000 >>> 1)..." << std::endl;

    dut_->in_0 = 0x80000000;
    dut_->shift_amount = 1;
    dut_->op_selector = 0b10;  // ASR

    dut_->eval();

    if (dut_->out != 0xC0000000) {
      std::cout << "Test Case 3 Failed: Expected output to be 0xC0000000 but got " << std::hex
                << dut_->out << std::dec << std::endl;
      return false;
    }

    return true;
  }

  bool test_case_4() {
    std::cout << "\nRunning Test Case 4 (0x1 << 1)..." << std::endl;

    dut_->in_0 = 0x00000001;
    dut_->shift_amount = 1;
    dut_->op_selector = 0b00;  // LSL

    dut_->eval();

    if (dut_->out != 0x00000001 << 1) {
      std::cout << "Test Case 4 Failed: Expected output to be 0x00000002 but got " << std::hex
                << dut_->out << std::dec << std::endl;
      return false;
    }

    return true;
  }

  bool test_case_5() {
    std::cout << "\nRunning Test Case 5 (0xFFFFFFFE >> 31)..." << std::endl;

    dut_->in_0 = 0xFFFFFFFE;
    dut_->shift_amount = 31;
    dut_->op_selector = 0b01;  // LSR

    dut_->eval();

    if (dut_->out != 0xFFFFFFFE >> 31) {
      std::cout << "Test Case 5 Failed: Expected output to be 0x00000000 but got " << std::hex
                << dut_->out << std::dec << std::endl;
      return false;
    }

    return true;
  }

  bool test_case_6() {
    std::cout << "\nRunning Test Case 6 (0x80000000 >> 31)..." << std::endl;

    dut_->in_0 = 0x80000000;
    dut_->shift_amount = 31;
    dut_->op_selector = 0b01;  // LSR

    dut_->eval();

    if (dut_->out != 0x80000000 >> 31) {
      std::cout << "Test Case 6 Failed: Expected output to be 0x00000000 but got " << std::hex
                << dut_->out << std::dec << std::endl;
      return false;
    }

    return true;
  }

  bool test_case_7() {
    std::cout << "\nRunning Test Case 7 (0x1 << 14)..." << std::endl;

    dut_->in_0 = 0x00000001;
    dut_->shift_amount = 14;
    dut_->op_selector = 0b00;  // LSL

    dut_->eval();

    if (dut_->out != 0x00000001 << 14) {
      std::cout << "Test Case 7 Failed: Expected output to be 0x00000000 but got " << std::hex
                << dut_->out << std::dec << std::endl;
      return false;
    }

    return true;
  }

  bool test_case_8() {
    std::cout << "\nRunning Test Case 8 (0x12345678 >> 16)..." << std::endl;

    dut_->in_0 = 0x12345678;
    dut_->shift_amount = 16;
    dut_->op_selector = 0b01;  // LSR

    dut_->eval();

    if (dut_->out != 0x12345678 >> 16) {
      std::cout << "Test Case 8 Failed: Expected output to be 0x00001234 but got " << std::hex
                << dut_->out << std::dec << std::endl;
      return false;
    }

    return true;
  }
};
