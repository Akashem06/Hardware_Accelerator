#pragma once
#include <map>
#include <vector>

#include "../hardware_test.h"
#include "Vinstruction_cache.h"

class instruction_cacheTest : public HardwareTestBase {
 protected:
  std::unique_ptr<Vinstruction_cache> dut_;

 public:
  instruction_cacheTest() : HardwareTestBase() {
    dut_ = std::make_unique<Vinstruction_cache>();
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
    tick(5);  // Hold reset for a few cycles
    dut_->rst_n = 1;
    tick(1);
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
    std::cout << "\nRunning Test Case 1 (Cache Miss and Fill)..." << std::endl;

    // Choose a block-aligned address (assume INSTRUCTION_WIDTH is 32 bits)
    dut_->pc = 0x1000;
    dut_->fetch_req = 1;

    // Clear mem_data array
    for (int i = 0; i < 16; i++) {
      dut_->mem_data[i] = 0;
    }
    dut_->mem_valid = 0;

    // First cycle: expect a miss since the cache is empty
    tick();
    if (dut_->cache_hit != 0) {
      std::cout << "Test Case 1 Failed: Expected cache_hit = 0, got " << dut_->cache_hit
                << std::endl;
      return false;
    }
    if (dut_->mem_read != 1) {
      std::cout << "Test Case 1 Failed: Expected mem_read = 1, got " << dut_->mem_read << std::endl;
      return false;
    }

    // Next cycle: simulate memory response
    dut_->mem_valid = 1;
    // Set mem_data[0] to 0xDEADBEEF and clear the rest
    for (int i = 0; i < 16; i++) {
      dut_->mem_data[i] = (i == 0) ? 0xDEADBEEF : 0;
    }
    tick();

    if (dut_->instruction != 0xDEADBEEF) {
      std::cout << "Test Case 1 Failed: Expected instruction = 0xDEADBEEF, got " << std::hex
                << dut_->instruction << std::dec << std::endl;
      return false;
    }
    if (dut_->mem_read != 0) {
      std::cout << "Test Case 1 Failed: Expected mem_read = 0 after mem_valid, got "
                << dut_->mem_read << std::endl;
      return false;
    }

    // Clean up signals for next test cycle
    dut_->fetch_req = 0;
    dut_->mem_valid = 0;
    tick();
    return true;
  }

  bool test_case_2() {
    std::cout << "\nRunning Test Case 2 (Cache Hit)..." << std::endl;

    // The previous test loaded address 0x1000 into the cache
    dut_->pc = 0x1000;
    dut_->fetch_req = 1;
    dut_->mem_valid = 0;

    tick();

    // First cycle: expect a cache hit since 0x1000 instruction is already loaded in
    if (dut_->cache_hit != 1) {
      std::cout << "Test Case 2 Failed: Expected cache_hit = 1, got " << dut_->cache_hit
                << std::endl;
      return false;
    }
    if (dut_->instruction != 0xDEADBEEF) {
      std::cout << "Test Case 2 Failed: Expected instruction = 0xDEADBEEF, got " << std::hex
                << dut_->instruction << std::dec << std::endl;
      return false;
    }
    if (dut_->mem_read != 0) {
      std::cout << "Test Case 2 Failed: Expected mem_read = 0, got " << dut_->mem_read << std::endl;
      return false;
    }
    dut_->fetch_req = 0;
    tick();
    return true;
  }

  bool test_case_3() {
    std::cout << "\nRunning Test Case 3 (Cache Replacement with LRU)..." << std::endl;
    // Use two addresses that map to the same cache set
    // pc1 = 0x1000 is already loaded
    // pc2 = 0x5000 (assuming it maps to the same set as 0x1000 but different tag
    dut_->pc = 0x5000;
    dut_->fetch_req = 1;
    dut_->mem_valid = 0;
    tick();

    // Expect a miss for pc2, since not yet loaded
    if (dut_->cache_hit != 0) {
      std::cout << "Test Case 3 Failed: Expected cache miss for pc 0x5000, got cache_hit = "
                << dut_->cache_hit << std::endl;
      return false;
    }
    if (dut_->mem_read != 1) {
      std::cout << "Test Case 3 Failed: Expected mem_read = 1 for pc 0x5000, got " << dut_->mem_read
                << std::endl;
      return false;
    }

    // Simulate memory response for pc2 with a distinct value
    dut_->mem_valid = 1;
    for (int i = 0; i < 16; i++) {
      dut_->mem_data[i] = (i == 0) ? 0xCAFEBABE : 0;
    }
    tick();

    // The new memory data should be laoded into the other way of the 2-way set
    if (dut_->instruction != 0xCAFEBABE) {
      std::cout << "Test Case 3 Failed: Expected instruction = 0xCAFEBABE for pc 0x5000, got "
                << std::hex << dut_->instruction << std::dec << std::endl;
      return false;
    }
    if (dut_->mem_read != 0) {
      std::cout << "Test Case 3 Failed: Expected mem_read = 0 after loading pc 0x5000, got "
                << dut_->mem_read << std::endl;
      return false;
    }
    dut_->fetch_req = 0;
    dut_->mem_valid = 0;
    tick();

    // Now re-access pc1, it should still be in the cache
    dut_->pc = 0x1000;
    dut_->fetch_req = 1;
    tick();

    // Should be a cache hit
    if (dut_->cache_hit != 1) {
      std::cout << "Test Case 3 Failed: Expected cache_hit = 1 for pc 0x1000, got "
                << dut_->cache_hit << std::endl;
      return false;
    }
    if (dut_->instruction != 0xDEADBEEF) {
      std::cout << "Test Case 3 Failed: Expected instruction = 0xDEADBEEF for pc 0x1000, got "
                << std::hex << dut_->instruction << std::dec << std::endl;
      return false;
    }
    dut_->fetch_req = 0;
    tick();

    // Finally, re-access pc2 to ensure it is also still cached
    dut_->pc = 0x5000;
    dut_->fetch_req = 1;
    tick();
    if (dut_->cache_hit != 1) {
      std::cout << "Test Case 3 Failed: Expected cache_hit = 1 for pc 0x5000, got "
                << dut_->cache_hit << std::endl;
      return false;
    }
    if (dut_->instruction != 0xCAFEBABE) {
      std::cout << "Test Case 3 Failed: Expected instruction = 0xCAFEBABE for pc 0x5000, got "
                << std::hex << dut_->instruction << std::dec << std::endl;
      return false;
    }
    dut_->fetch_req = 0;
    tick();
    return true;
  }

  bool test_case_4() {
    std::cout << "\nRunning Test Case 4 (Reset Behavior)..." << std::endl;
    // Perform a fetch to load data
    dut_->pc = 0x1000;
    dut_->fetch_req = 1;
    tick();

    // Now reset the DUT
    reset();
    tick();

    if (dut_->instruction != 0) {
      std::cout << "Test Case 4 Failed: Expected instruction = 0 after reset, got " << std::hex
                << dut_->instruction << std::dec << std::endl;
      return false;
    }
    return true;
  }

  bool test_case_5() {
    std::cout << "\nRunning Test Case 5 (Sequential Access within Block)..." << std::endl;

    // Start with a reset to ensure a clean cache state
    reset();

    // Fetch the block starting at 0x2000
    // Assume block size is 16 words (64 bytes) and each word is 4 bytes
    // We'll simulate different instruction words within this block

    dut_->pc = 0x2000;  // Block-aligned address
    dut_->fetch_req = 1;
    dut_->mem_valid = 0;
    tick();

    // Expect a miss and a memory read
    if (dut_->cache_hit != 0) {
      std::cout << "Test Case 5 Failed: Expected cache_hit = 0 for initial access at 0x2000, got "
                << dut_->cache_hit << std::endl;
      return false;
    }
    if (dut_->mem_read != 1) {
      std::cout << "Test Case 5 Failed: Expected mem_read = 1 for 0x2000, got " << dut_->mem_read
                << std::endl;
      return false;
    }

    // Simulate memory response: Fill the entire block with unique values
    dut_->mem_valid = 1;
    for (int i = 0; i < 16; i++) {
      // For example, set each word to (0x2000 + i) so that:
      // address 0x2000 -> word 0 = 0x2000, address 0x2004 -> word 1 = 0x2001, etc.
      dut_->mem_data[i] = 0x2000 + i;
    }
    tick();

    // The first instruction fetched (for 0x2000) should be the first word
    if (dut_->instruction != 0x2000) {
      std::cout << "Test Case 5 Failed: Expected instruction = 0x2000 for address 0x2000, got "
                << std::hex << dut_->instruction << std::dec << std::endl;
      return false;
    }
    if (dut_->mem_read != 0) {
      std::cout << "Test Case 5 Failed: Expected mem_read = 0 after fill, got " << dut_->mem_read
                << std::endl;
      return false;
    }

    // Now fetch the next sequential instruction (address 0x2004, which is in the same block)
    dut_->pc = 0x2004;  // 4-byte offset from 0x2000
    dut_->fetch_req = 1;
    dut_->mem_valid = 0;
    tick();

    // Expect a cache hit with the instruction from the block offset (should be 0x2001)
    if (dut_->cache_hit != 1) {
      std::cout << "Test Case 5 Failed: Expected cache_hit = 1 for address 0x2004, got "
                << dut_->cache_hit << std::endl;
      return false;
    }
    if (dut_->instruction != 0x2001) {
      std::cout << "Test Case 5 Failed: Expected instruction = 0x2001 for address 0x2004, got "
                << std::hex << dut_->instruction << std::dec << std::endl;
      return false;
    }

    dut_->fetch_req = 0;
    tick();
    return true;
  }

  bool test_case_6() {
    std::cout << "\nRunning Test Case 6 (Multiple Block Fills and Random Accesses)..." << std::endl;
    // Reset DUT to start with an empty cache.
    reset();

    // Define a set of block-aligned addresses.
    std::vector<uint32_t> addresses = { 0x8000, 0x9000, 0xA000, 0xB000 };
    std::map<uint32_t, uint32_t> expected_instr;

    // Load each block.
    for (auto addr : addresses) {
      dut_->pc = addr;
      dut_->fetch_req = 1;
      dut_->mem_valid = 0;
      tick();
      if (dut_->cache_hit != 0) {
        std::cout << "Test Case 6 Failed: Expected miss for addr 0x" << std::hex << addr << std::dec
                  << ", got cache_hit = " << dut_->cache_hit << std::endl;
        return false;
      }
      if (dut_->mem_read != 1) {
        std::cout << "Test Case 6 Failed: Expected mem_read = 1 for addr 0x" << std::hex << addr
                  << std::dec << ", got " << dut_->mem_read << std::endl;
        return false;
      }
      // Simulate memory response: assign a unique value to the first word of the block.
      dut_->mem_valid = 1;
      for (int i = 0; i < 16; i++) {
        dut_->mem_data[i] = (i == 0) ? (addr | 0x1) : 0;
      }
      tick();

      if (dut_->instruction != (addr | 0x1)) {
        std::cout << "Test Case 6 Failed: Expected instruction = 0x" << std::hex << (addr | 0x1)
                  << " for addr 0x" << addr << ", got " << dut_->instruction << std::dec
                  << std::endl;
        return false;
      }

      // Record expected value.
      expected_instr[addr] = (addr | 0x1);

      dut_->fetch_req = 0;
      dut_->mem_valid = 0;
      tick();
    }

    // Now perform accesses in a different (random) order.
    std::vector<uint32_t> access_order = { 0xA000, 0x8000, 0xB000, 0x9000 };
    for (auto addr : access_order) {
      dut_->pc = addr;
      dut_->fetch_req = 1;
      dut_->mem_valid = 0;
      tick();

      if (dut_->cache_hit != 1) {
        std::cout << "Test Case 6 Failed: Expected hit for addr 0x" << std::hex << addr << std::dec
                  << ", got cache_hit = " << dut_->cache_hit << std::endl;
        return false;
      }
      if (dut_->instruction != expected_instr[addr]) {
        std::cout << "Test Case 6 Failed: Expected instruction = 0x" << std::hex
                  << expected_instr[addr] << " for addr 0x" << addr << ", got " << dut_->instruction
                  << std::dec << std::endl;
        return false;
      }
      dut_->fetch_req = 0;
      tick();
    }
    return true;
  }

  bool test_case_7() {
    std::cout << "\nRunning Test Case 7 (LRU Replacement Algorithm)..." << std::endl;

    reset();

    // Load first address into way 0
    uint32_t addr1 = 0x1000;
    dut_->pc = addr1;
    dut_->fetch_req = 1;
    tick();

    // Fill memory data
    dut_->mem_valid = 1;
    for (int i = 0; i < 16; i++) {
      dut_->mem_data[i] = (i == 0) ? 0xAAAAAAAA : 0;
    }
    tick();
    dut_->fetch_req = 0;
    dut_->mem_valid = 0;
    tick();

    // Load second address into way 1 (same set)
    uint32_t addr2 = 0x21000;  // Different tag, same set as addr1
    dut_->pc = addr2;
    dut_->fetch_req = 1;
    tick();

    // Fill with different data
    dut_->mem_valid = 1;
    for (int i = 0; i < 16; i++) {
      dut_->mem_data[i] = (i == 0) ? 0xBBBBBBBB : 0;
    }
    tick();
    dut_->fetch_req = 0;
    dut_->mem_valid = 0;
    tick();

    // Access addr1 to make it MRU
    dut_->pc = addr1;
    dut_->fetch_req = 1;
    tick();

    if (dut_->instruction != 0xAAAAAAAA) {
      std::cout << "Test Case 7 Failed: Expected 0xAAAAAAAA from addr1, got " << std::hex
                << dut_->instruction << std::dec << std::endl;
      return false;
    }
    dut_->fetch_req = 0;
    tick();

    // Load third address - should replace addr2 since it's LRU
    uint32_t addr3 = 0x41000;  // Different tag, same set
    dut_->pc = addr3;
    dut_->fetch_req = 1;
    tick();

    // Fill with new data
    dut_->mem_valid = 1;
    for (int i = 0; i < 16; i++) {
      dut_->mem_data[i] = (i == 0) ? 0xCCCCCCCC : 0;
    }
    tick();
    dut_->fetch_req = 0;
    dut_->mem_valid = 0;
    tick();

    // Verify addr1 is still in cache (since it was MRU)
    dut_->pc = addr1;
    dut_->fetch_req = 1;
    tick();

    if (!dut_->cache_hit || dut_->instruction != 0xAAAAAAAA) {
      std::cout << "Test Case 7 Failed: addr1 was incorrectly replaced (expected 0xAAAAAAAA, got "
                << std::hex << dut_->instruction << ")" << std::dec << std::endl;
      return false;
    }

    // Verify addr2 was replaced (should be a miss)
    dut_->pc = addr2;
    dut_->fetch_req = 1;
    tick();

    if (dut_->cache_hit) {
      std::cout << "Test Case 7 Failed: addr2 still in cache when it should have been replaced"
                << std::endl;
      return false;
    }

    return true;
  }
};
