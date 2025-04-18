#pragma once

#include "../hardware_test.h"
#include "Vregister_file.h"

class register_fileTest : public HardwareTestBase {
 protected:
  std::unique_ptr<Vregister_file> dut_;

 public:
  register_fileTest() : HardwareTestBase() {
    dut_ = std::make_unique<Vregister_file>();
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

    std::cout << "\nFinal Result: " << (all_passed ? "ALL TESTS PASSED" : "SOME TESTS FAILED")
              << std::endl;

    return all_passed;
  }

 private:
  bool test_case_1() {
    std::cout << "\nRunning Test Case 1: Single Register Write/Read..." << std::endl;

    // Reset to initialize all registers to 0
    reset();

    // Write value 0xA5A5 to register 5
    dut_->rd_addr = 5;
    dut_->rd_data = 0xA5A5;
    dut_->rd_write_en = 1;

    tick();

    // Disable write enable after the write
    dut_->rd_write_en = 0;

    // Read the written register using both read ports
    dut_->rs1_addr = 5;
    dut_->rs2_addr = 5;
    dut_->eval();

    if (dut_->rs1_data != 0xA5A5 || dut_->rs2_data != 0xA5A5) {
      std::cout << "Test Case 1 Failed: Expected register value 0xA5A5, but got "
                << "rs1_data=0x" << std::hex << dut_->rs1_data << " and rs2_data=0x" << std::hex
                << dut_->rs2_data << std::endl;
      return false;
    }

    std::cout << "Test Case 1 Passed." << std::endl;
    return true;
  }

  // Test Case 2: Write to multiple registers sequentially and verify the contents
  bool test_case_2() {
    std::cout << "\nRunning Test Case 2: Multiple Register Writes..." << std::endl;
    reset();
    bool passed = true;

    // Write value 0x1111 to register 2
    dut_->rd_addr = 2;
    dut_->rd_data = 0x1111;
    dut_->rd_write_en = 1;

    tick();

    // Write value 0x2222 to register 7
    dut_->rd_addr = 7;
    dut_->rd_data = 0x2222;
    dut_->rd_write_en = 1;

    tick();

    // Disable write enable after writes
    dut_->rd_write_en = 0;
    dut_->eval();

    // Set read addresses to verify the written registers
    dut_->rs1_addr = 2;
    dut_->rs2_addr = 7;
    dut_->eval();

    if (dut_->rs1_data != 0x1111) {
      std::cout << "Test Case 2 Failed: Expected rs1_data=0x1111, got 0x" << std::hex
                << dut_->rs1_data << std::endl;
      passed = false;
    }

    if (dut_->rs2_data != 0x2222) {
      std::cout << "Test Case 2 Failed: Expected rs2_data=0x2222, got 0x" << std::hex
                << dut_->rs2_data << std::endl;
      passed = false;
    }

    if (passed) {
      std::cout << "Test Case 2 Passed." << std::endl;
    }
    return passed;
  }
};
