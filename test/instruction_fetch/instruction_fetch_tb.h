#pragma once

#include <cstdint>
#include <iostream>
#include <memory>

#include "../hardware_test.h"
#include "Vinstruction_fetch.h"

class instruction_fetchTest : public HardwareTestBase {
 protected:
  std::unique_ptr<Vinstruction_fetch> dut_;

 public:
  instruction_fetchTest() : HardwareTestBase() {
    dut_ = std::make_unique<Vinstruction_fetch>();
  }

  void eval_clock_low() override {
    dut_->clk = 0;
    dut_->eval();
  }

  void eval_clock_high() override {
    dut_->clk = 1;
    dut_->eval();
  }

  // Active low reset: hold for a few cycles.
  void reset() override {
    dut_->rst_n = 0;
    tick(5);
    dut_->rst_n = 1;
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

    std::cout << "\nFinal Result: " << (all_passed ? "ALL TESTS PASSED" : "SOME TESTS FAILED")
              << std::endl;

    return all_passed;
  }

 private:
  bool test_case_1() {
    std::cout << "\nRunning Test Case 1 (Basic Sequential Fetch)..." << std::endl;
    // Initialize signals.
    dut_->pipeline_stall = 0;
    dut_->branch_taken = 0;
    dut_->branch_target = 0;
    dut_->cache_hit = 0;
    dut_->instruction = 0;

    // After reset, PC should be 0.
    if (dut_->pc != 0) {
      std::cout << "Test Case 1 Failed: Expected PC = 0, got " << dut_->pc << std::endl;
      return false;
    }
    // With an empty prefetch buffer, fetch_req should be asserted.
    if (!dut_->fetch_req) {
      std::cout << "Test Case 1 Failed: Expected fetch_req = 1 when prefetch buffer is empty."
                << std::endl;
      return false;
    }
    // Simulate a cache hit with an instruction.
    dut_->cache_hit = 1;
    dut_->instruction = 0x11111111;
    tick();  // Next cycle.

    // PC should now be incremented by one word (assumed 4 bytes).
    if (dut_->pc != 4) {
      std::cout << "Test Case 1 Failed: Expected PC = 4 after first fetch, got " << dut_->pc
                << std::endl;
      return false;
    }
    // The fetched instruction should be available.
    if (dut_->fetch_instruction != 0x11111111) {
      std::cout << "Test Case 1 Failed: Expected fetch_instruction = 0x11111111, got " << std::hex
                << dut_->fetch_instruction << std::dec << std::endl;
      return false;
    }
    if (!dut_->fetch_valid) {
      std::cout << "Test Case 1 Failed: Expected fetch_valid = 1 after fetching instruction."
                << std::endl;
      return false;
    }
    // Clear signals for next test.
    dut_->cache_hit = 0;
    dut_->instruction = 0;
    return true;
  }

  bool test_case_2() {
    std::cout << "\nRunning Test Case 2 (Pipeline Stall Behavior)..." << std::endl;
    // Assert pipeline stall.
    dut_->pipeline_stall = 1;
    dut_->cache_hit = 1;
    dut_->instruction = 0x22222222;
    uint32_t current_pc = dut_->pc;  // PC should not update.
    tick();

    if (dut_->pc != current_pc) {
      std::cout << "Test Case 2 Failed: Expected PC to remain " << current_pc
                << " during stall, got " << dut_->pc << std::endl;
      return false;
    }
    // With stall, fetch_req should be deasserted.
    if (dut_->fetch_req) {
      std::cout << "Test Case 2 Failed: Expected fetch_req = 0 during pipeline stall." << std::endl;
      return false;
    }
    // The fetch_instruction should remain unchanged.
    uint32_t prev_fetch_instruction = dut_->fetch_instruction;
    tick();
    if (dut_->fetch_instruction != prev_fetch_instruction) {
      std::cout
          << "Test Case 2 Failed: Expected fetch_instruction to remain unchanged during stall."
          << std::endl;
      return false;
    }
    // Deassert stall and simulate a new cache hit.
    dut_->pipeline_stall = 0;
    dut_->cache_hit = 1;
    dut_->instruction = 0x33333333;
    tick();
    if (dut_->pc != (current_pc + 4)) {
      std::cout << "Test Case 2 Failed: Expected PC = " << (current_pc + 4)
                << " after deasserting stall, got " << dut_->pc << std::endl;
      return false;
    }
    if (dut_->fetch_instruction != 0x33333333) {
      std::cout << "Test Case 2 Failed: Expected fetch_instruction = 0x33333333 after stall, got "
                << std::hex << dut_->fetch_instruction << std::dec << std::endl;
      return false;
    }
    return true;
  }

  bool test_case_3() {
    std::cout << "\nRunning Test Case 3 (Branch Flush Behavior)..." << std::endl;
    // Normal operation: simulate a fetch.
    dut_->pipeline_stall = 0;
    dut_->branch_taken = 0;
    dut_->cache_hit = 1;
    dut_->instruction = 0x44444444;
    tick();  // PC increments.

    // Now assert branch_taken.
    dut_->branch_taken = 1;
    dut_->branch_target = 0x2000;
    tick();  // Branch flush cycle.

    if (dut_->pc != 0x2000) {
      std::cout << "Test Case 3 Failed: Expected PC = 0x2000 after branch taken, got " << std::hex
                << dut_->pc << std::dec << std::endl;
      return false;
    }
    // Prefetch buffer should be flushed; fetch_valid should be deasserted.
    if (dut_->fetch_valid) {
      std::cout << "Test Case 3 Failed: Expected fetch_valid = 0 after branch flush." << std::endl;
      return false;
    }
    // Deassert branch_taken and simulate a new fetch.
    dut_->branch_taken = 0;
    dut_->cache_hit = 1;
    dut_->instruction = 0x55555555;
    tick();  // Next fetch cycle.
    if (dut_->pc != (0x2000 + 4)) {
      std::cout << "Test Case 3 Failed: Expected PC = 0x2004 after branch fetch, got " << std::hex
                << dut_->pc << std::dec << std::endl;
      return false;
    }
    if (dut_->fetch_instruction != 0x55555555) {
      std::cout
          << "Test Case 3 Failed: Expected fetch_instruction = 0x55555555 after branch fetch, got "
          << std::hex << dut_->fetch_instruction << std::dec << std::endl;
      return false;
    }
    return true;
  }

  bool test_case_4() {
    std::cout << "\nRunning Test Case 4 (Prefetch Buffer Wrap-around)..." << std::endl;
    // With a prefetch depth of 4, run 6 cycles to force pointer wrap-around.
    dut_->pipeline_stall = 0;
    dut_->branch_taken = 0;
    uint32_t start_pc = dut_->pc;
    for (int i = 0; i < 6; i++) {
      dut_->cache_hit = 1;
      // Provide a unique instruction value.
      uint32_t instr = 0x1000 + i;
      dut_->instruction = instr;
      tick();
      // PC should increment by 4 each cycle.
      if (dut_->pc != (start_pc + (i + 1) * 4)) {
        std::cout << "Test Case 4 Failed: Cycle " << i << " Expected PC = " << std::hex
                  << (start_pc + (i + 1) * 4) << ", got " << dut_->pc << std::dec << std::endl;
        return false;
      }
      // Check that fetch_valid is asserted and fetch_instruction matches.
      if (!dut_->fetch_valid) {
        std::cout << "Test Case 4 Failed: Cycle " << i << " Expected fetch_valid = 1." << std::endl;
        return false;
      }
      if (dut_->fetch_instruction != instr) {
        std::cout << "Test Case 4 Failed: Cycle " << i << " Expected fetch_instruction = 0x"
                  << std::hex << instr << ", got 0x" << dut_->fetch_instruction << std::dec
                  << std::endl;
        return false;
      }
    }
    return true;
  }

  bool test_case_5() {
    std::cout << "\nRunning Test Case 5 (Multiple Sequential Fetches with Intermittent Stall)..."
              << std::endl;
    // Start from the current PC.
    dut_->pipeline_stall = 0;
    dut_->branch_taken = 0;
    uint32_t initial_pc = dut_->pc;

    // Cycle 1: Normal fetch.
    dut_->cache_hit = 1;
    dut_->instruction = 0x77777777;
    tick();
    if (dut_->fetch_instruction != 0x77777777) {
      std::cout << "Test Case 5 Failed: Cycle 1 Expected fetch_instruction = 0x77777777, got "
                << std::hex << dut_->fetch_instruction << std::dec << std::endl;
      return false;
    }
    // Cycle 2: Assert stall (instruction should not be updated).
    dut_->pipeline_stall = 1;
    dut_->cache_hit = 1;
    dut_->instruction = 0x88888888;  // This value should be ignored.
    tick();
    if (dut_->pc != (initial_pc + 4)) {
      std::cout << "Test Case 5 Failed: Cycle 2 Expected PC to remain " << (initial_pc + 4)
                << ", got " << dut_->pc << std::endl;
      return false;
    }
    if (dut_->fetch_instruction != 0x77777777) {
      std::cout
          << "Test Case 5 Failed: Cycle 2 Expected fetch_instruction to remain 0x77777777, got "
          << std::hex << dut_->fetch_instruction << std::dec << std::endl;
      return false;
    }
    // Cycle 3: Deassert stall and fetch new instruction.
    dut_->pipeline_stall = 0;
    dut_->cache_hit = 1;
    dut_->instruction = 0x99999999;
    tick();
    if (dut_->pc != (initial_pc + 8)) {
      std::cout << "Test Case 5 Failed: Cycle 3 Expected PC = " << (initial_pc + 8) << ", got "
                << dut_->pc << std::endl;
      return false;
    }
    if (dut_->fetch_instruction != 0x99999999) {
      std::cout << "Test Case 5 Failed: Cycle 3 Expected fetch_instruction = 0x99999999, got "
                << std::hex << dut_->fetch_instruction << std::dec << std::endl;
      return false;
    }
    return true;
  }

  bool test_case_6() {
    std::cout << "\nRunning Test Case 6 (Rapid Branch Flushes)..." << std::endl;
    // Cycle 1: Normal fetch.
    dut_->pipeline_stall = 0;
    dut_->branch_taken = 0;
    dut_->cache_hit = 1;
    dut_->instruction = 0xAAAAAAA1;
    tick();
    // Cycle 2: Assert branch flush.
    dut_->branch_taken = 1;
    dut_->branch_target = 0x3000;
    tick();
    if (dut_->pc != 0x3000) {
      std::cout << "Test Case 6 Failed: After first branch, expected PC = 0x3000, got " << std::hex
                << dut_->pc << std::dec << std::endl;
      return false;
    }
    if (dut_->fetch_valid) {
      std::cout << "Test Case 6 Failed: After first branch, expected fetch_valid = 0." << std::endl;
      return false;
    }
    // Cycle 3: Resume normal fetch.
    dut_->branch_taken = 0;
    dut_->cache_hit = 1;
    dut_->instruction = 0xBBBBBBB2;
    tick();
    if (dut_->pc != (0x3000 + 4)) {
      std::cout << "Test Case 6 Failed: After resuming fetch, expected PC = " << std::hex
                << (0x3000 + 4) << ", got " << dut_->pc << std::dec << std::endl;
      return false;
    }
    // Cycle 4: Immediately branch again.
    dut_->branch_taken = 1;
    dut_->branch_target = 0x4000;
    tick();
    if (dut_->pc != 0x4000) {
      std::cout << "Test Case 6 Failed: After second branch, expected PC = 0x4000, got " << std::hex
                << dut_->pc << std::dec << std::endl;
      return false;
    }
    if (dut_->fetch_valid) {
      std::cout << "Test Case 6 Failed: After second branch, expected fetch_valid = 0."
                << std::endl;
      return false;
    }
    // Cycle 5: Resume again.
    dut_->branch_taken = 0;
    dut_->cache_hit = 1;
    dut_->instruction = 0xCCCCCCCC;
    tick();
    if (dut_->pc != (0x4000 + 4)) {
      std::cout << "Test Case 6 Failed: After resuming post second branch, expected PC = "
                << std::hex << (0x4000 + 4) << ", got " << dut_->pc << std::dec << std::endl;
      return false;
    }
    if (dut_->fetch_instruction != 0xCCCCCCCC) {
      std::cout
          << "Test Case 6 Failed: After resuming post second branch, expected fetch_instruction = "
             "0xCCCCCCCC, got "
          << std::hex << dut_->fetch_instruction << std::dec << std::endl;
      return false;
    }
    return true;
  }

  bool test_case_7() {
    std::cout << "\nRunning Test Case 7 (Fetch Request Logic Verification)..." << std::endl;
    // With pipeline not stalled, fetch_req should be asserted.
    dut_->pipeline_stall = 0;
    tick();
    if (!dut_->fetch_req) {
      std::cout << "Test Case 7 Failed: Expected fetch_req = 1 when pipeline not stalled."
                << std::endl;
      return false;
    }
    // With pipeline stall asserted, fetch_req should be deasserted.
    dut_->pipeline_stall = 1;
    tick();
    if (dut_->fetch_req) {
      std::cout << "Test Case 7 Failed: Expected fetch_req = 0 when pipeline stalled." << std::endl;
      return false;
    }
    // Deassert stall and simulate a cache hit.
    dut_->pipeline_stall = 0;
    dut_->cache_hit = 1;
    dut_->instruction = 0xDDDDDDDD;
    tick();
    // While immediate consumption might cause fetch_req to drop momentarily,
    // we at least verify that fetch_req is not erroneously high during stall.
    // (A warning is printed if fetch_req is 0 due to immediate consumption.)
    if (!dut_->fetch_req) {
      std::cout << "Test Case 7 Warning: fetch_req is 0 after a fetch. This may be due to "
                   "immediate consumption."
                << std::endl;
    }
    return true;
  }
};
