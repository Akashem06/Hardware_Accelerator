#pragma once

#include <cstdint>
#include <iomanip>
#include <iostream>

#include "../hardware_test.h"
#include "Vinstruction_decoder.h"

class instruction_decoderTest : public HardwareTestBase {
 protected:
  std::unique_ptr<Vinstruction_decoder> dut_;

 public:
  instruction_decoderTest() : HardwareTestBase() {
    dut_ = std::make_unique<Vinstruction_decoder>();
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

    all_passed &= test_case_1();   // MLOAD
    all_passed &= test_case_2();   // DMA_ST
    all_passed &= test_case_3();   // MSTORE
    all_passed &= test_case_4();   // MMULT
    all_passed &= test_case_5();   // MADD
    all_passed &= test_case_6();   // DMA_CHK
    all_passed &= test_case_7();   // CONF
    all_passed &= test_case_8();   // SYNC
    all_passed &= test_case_9();   // LD
    all_passed &= test_case_10();  // ST

    std::cout << "\nFinal Result: " << (all_passed ? "ALL TESTS PASSED" : "SOME TESTS FAILED")
              << std::endl;

    return all_passed;
  }

 private:
  // opcode [34:31], rd [30:27], rs1 [26:23], rs2 [22:19], imm [18:3], is_matrix_op [2], uses_dma
  // [1], needs_sync [0]
  uint8_t get_opcode() {
    return (static_cast<uint64_t>(dut_->decoded_instruction) >> 31) & 0xF;
  }
  uint8_t get_rd() {
    return (static_cast<uint64_t>(dut_->decoded_instruction) >> 27) & 0xF;
  }
  uint8_t get_rs1() {
    return (static_cast<uint64_t>(dut_->decoded_instruction) >> 23) & 0xF;
  }
  uint8_t get_rs2() {
    return (static_cast<uint64_t>(dut_->decoded_instruction) >> 19) & 0xF;
  }
  uint16_t get_imm() {
    return (static_cast<uint64_t>(dut_->decoded_instruction) >> 3) & 0xFFFF;
  }
  bool get_is_matrix_op() {
    return (static_cast<uint64_t>(dut_->decoded_instruction) >> 2) & 0x1;
  }
  bool get_uses_dma() {
    return (static_cast<uint64_t>(dut_->decoded_instruction) >> 1) & 0x1;
  }
  bool get_needs_sync() {
    return static_cast<uint64_t>(dut_->decoded_instruction) & 0x1;
  }

  bool test_case_1() {
    std::cout << "\nRunning Test Case 1 (MLOAD Decoding)..." << std::endl;
    // Construct: opcode = 0x0, rd = 2, rs1 = 3, rs2 = 4, imm = 0xABCD.
    // (Note: For MLOAD the spec uses only Rd, Rs1, imm.)
    dut_->fetch_instruction = (0x0 << 28) | (2 << 24) | (3 << 20) | (4 << 16) | 0xABCD;
    dut_->fetch_valid = 1;
    tick();  // Advance one cycle

    std::cout << "decoded_instruction: 0x" << std::hex << dut_->decoded_instruction << std::dec
              << std::endl;

    if (!dut_->decode_valid) {
      std::cout << "Test Case 1 Failed: decode_valid is false" << std::endl;
      return false;
    }
    if (get_opcode() != 0x0) {
      std::cout << "Test Case 1 Failed: Expected opcode MLOAD (0x0), got 0x" << std::hex
                << static_cast<int>(get_opcode()) << std::dec << std::endl;
      return false;
    }
    if (get_rd() != 2) {
      std::cout << "Test Case 1 Failed: Expected rd = 2, got " << static_cast<int>(get_rd())
                << std::endl;
      return false;
    }
    if (get_rs1() != 3) {
      std::cout << "Test Case 1 Failed: Expected rs1 = 3, got " << static_cast<int>(get_rs1())
                << std::endl;
      return false;
    }
    if (get_rs2() != 4) {
      std::cout << "Test Case 1 Failed: Expected rs2 = 4, got " << static_cast<int>(get_rs2())
                << std::endl;
      return false;
    }
    if (get_imm() != 0xABCD) {
      std::cout << "Test Case 1 Failed: Expected imm = 0xABCD, got 0x" << std::hex
                << static_cast<int>(get_imm()) << std::dec << std::endl;
      return false;
    }
    if (!get_is_matrix_op()) {
      std::cout << "Test Case 1 Failed: Expected is_matrix_op to be true" << std::endl;
      return false;
    }
    if (get_uses_dma()) {
      std::cout << "Test Case 1 Failed: Expected uses_dma to be false" << std::endl;
      return false;
    }
    if (get_needs_sync()) {
      std::cout << "Test Case 1 Failed: Expected needs_sync to be false" << std::endl;
      return false;
    }
    std::cout << "Test Case 1 Passed" << std::endl;
    return true;
  }

  bool test_case_2() {
    std::cout << "\nRunning Test Case 2 (DMA_ST Decoding)..." << std::endl;
    // Construct: opcode = 0x4, rd = 1, rs1 = 5, rs2 = 6, imm = 0x1234.
    dut_->fetch_instruction = (0x4 << 28) | (1 << 24) | (5 << 20) | (6 << 16) | 0x1234;
    dut_->fetch_valid = 1;
    tick();  // Advance one cycle

    if (!dut_->decode_valid) {
      std::cout << "Test Case 2 Failed: decode_valid is false" << std::endl;
      return false;
    }
    if (get_opcode() != 0x4) {
      std::cout << "Test Case 2 Failed: Expected opcode DMA_ST (0x4), got 0x" << std::hex
                << static_cast<int>(get_opcode()) << std::dec << std::endl;
      return false;
    }
    if (get_rd() != 1) {
      std::cout << "Test Case 2 Failed: Expected rd = 1, got " << static_cast<int>(get_rd())
                << std::endl;
      return false;
    }
    if (get_rs1() != 5) {
      std::cout << "Test Case 2 Failed: Expected rs1 = 5, got " << static_cast<int>(get_rs1())
                << std::endl;
      return false;
    }
    if (get_rs2() != 6) {
      std::cout << "Test Case 2 Failed: Expected rs2 = 6, got " << static_cast<int>(get_rs2())
                << std::endl;
      return false;
    }
    if (get_imm() != 0x1234) {
      std::cout << "Test Case 2 Failed: Expected imm = 0x1234, got 0x" << std::hex
                << static_cast<int>(get_imm()) << std::dec << std::endl;
      return false;
    }
    if (get_is_matrix_op()) {
      std::cout << "Test Case 2 Failed: Expected is_matrix_op to be false" << std::endl;
      return false;
    }
    if (!get_uses_dma()) {
      std::cout << "Test Case 2 Failed: Expected uses_dma to be true" << std::endl;
      return false;
    }
    if (get_needs_sync()) {
      std::cout << "Test Case 2 Failed: Expected needs_sync to be false" << std::endl;
      return false;
    }
    std::cout << "Test Case 2 Passed" << std::endl;
    return true;
  }

  bool test_case_3() {
    std::cout << "\nRunning Test Case 3 (MSTORE Decoding)..." << std::endl;
    // Construct: opcode = 0x1, rd = 3, rs1 = 4, rs2 = 0 (unused), imm = 0xDEAD.
    dut_->fetch_instruction = (0x1 << 28) | (3 << 24) | (4 << 20) | (0 << 16) | 0xDEAD;
    dut_->fetch_valid = 1;
    tick();

    if (!dut_->decode_valid) {
      std::cout << "Test Case 3 Failed: decode_valid is false" << std::endl;
      return false;
    }
    if (get_opcode() != 0x1) {
      std::cout << "Test Case 3 Failed: Expected opcode MSTORE (0x1), got 0x" << std::hex
                << static_cast<int>(get_opcode()) << std::dec << std::endl;
      return false;
    }
    if (get_rd() != 3) {
      std::cout << "Test Case 3 Failed: Expected rd = 3, got " << static_cast<int>(get_rd())
                << std::endl;
      return false;
    }
    if (get_rs1() != 4) {
      std::cout << "Test Case 3 Failed: Expected rs1 = 4, got " << static_cast<int>(get_rs1())
                << std::endl;
      return false;
    }
    if (get_rs2() != 0) {
      std::cout << "Test Case 3 Failed: Expected rs2 = 0, got " << static_cast<int>(get_rs2())
                << std::endl;
      return false;
    }
    if (get_imm() != 0xDEAD) {
      std::cout << "Test Case 3 Failed: Expected imm = 0xDEAD, got 0x" << std::hex
                << static_cast<int>(get_imm()) << std::dec << std::endl;
      return false;
    }
    if (!get_is_matrix_op()) {
      std::cout << "Test Case 3 Failed: Expected is_matrix_op to be true" << std::endl;
      return false;
    }
    if (get_uses_dma()) {
      std::cout << "Test Case 3 Failed: Expected uses_dma to be false" << std::endl;
      return false;
    }
    if (get_needs_sync()) {
      std::cout << "Test Case 3 Failed: Expected needs_sync to be false" << std::endl;
      return false;
    }
    std::cout << "Test Case 3 Passed" << std::endl;
    return true;
  }

  bool test_case_4() {
    std::cout << "\nRunning Test Case 4 (MMULT Decoding)..." << std::endl;
    // Construct: opcode = 0x2, rd = 5, rs1 = 6, rs2 = 7, imm = 0x0000.
    dut_->fetch_instruction = (0x2 << 28) | (5 << 24) | (6 << 20) | (7 << 16) | 0x0000;
    dut_->fetch_valid = 1;
    tick();

    if (!dut_->decode_valid) {
      std::cout << "Test Case 4 Failed: decode_valid is false" << std::endl;
      return false;
    }
    if (get_opcode() != 0x2) {
      std::cout << "Test Case 4 Failed: Expected opcode MMULT (0x2), got 0x" << std::hex
                << static_cast<int>(get_opcode()) << std::dec << std::endl;
      return false;
    }
    if (get_rd() != 5) {
      std::cout << "Test Case 4 Failed: Expected rd = 5, got " << static_cast<int>(get_rd())
                << std::endl;
      return false;
    }
    if (get_rs1() != 6) {
      std::cout << "Test Case 4 Failed: Expected rs1 = 6, got " << static_cast<int>(get_rs1())
                << std::endl;
      return false;
    }
    if (get_rs2() != 7) {
      std::cout << "Test Case 4 Failed: Expected rs2 = 7, got " << static_cast<int>(get_rs2())
                << std::endl;
      return false;
    }
    if (get_imm() != 0x0000) {
      std::cout << "Test Case 4 Failed: Expected imm = 0x0000, got 0x" << std::hex
                << static_cast<int>(get_imm()) << std::dec << std::endl;
      return false;
    }
    if (!get_is_matrix_op()) {
      std::cout << "Test Case 4 Failed: Expected is_matrix_op to be true" << std::endl;
      return false;
    }
    if (get_uses_dma()) {
      std::cout << "Test Case 4 Failed: Expected uses_dma to be false" << std::endl;
      return false;
    }
    if (get_needs_sync()) {
      std::cout << "Test Case 4 Failed: Expected needs_sync to be false" << std::endl;
      return false;
    }
    std::cout << "Test Case 4 Passed" << std::endl;
    return true;
  }

  bool test_case_5() {
    std::cout << "\nRunning Test Case 5 (MADD Decoding)..." << std::endl;
    // Construct: opcode = 0x3, rd = 8, rs1 = 9, rs2 = 10, imm = 0x0000.
    dut_->fetch_instruction = (0x3 << 28) | (8 << 24) | (9 << 20) | (10 << 16) | 0x0000;
    dut_->fetch_valid = 1;
    tick();

    if (!dut_->decode_valid) {
      std::cout << "Test Case 5 Failed: decode_valid is false" << std::endl;
      return false;
    }
    if (get_opcode() != 0x3) {
      std::cout << "Test Case 5 Failed: Expected opcode MADD (0x3), got 0x" << std::hex
                << static_cast<int>(get_opcode()) << std::dec << std::endl;
      return false;
    }
    if (get_rd() != 8) {
      std::cout << "Test Case 5 Failed: Expected rd = 8, got " << static_cast<int>(get_rd())
                << std::endl;
      return false;
    }
    if (get_rs1() != 9) {
      std::cout << "Test Case 5 Failed: Expected rs1 = 9, got " << static_cast<int>(get_rs1())
                << std::endl;
      return false;
    }
    if (get_rs2() != 10) {
      std::cout << "Test Case 5 Failed: Expected rs2 = 10, got " << static_cast<int>(get_rs2())
                << std::endl;
      return false;
    }
    if (get_imm() != 0x0000) {
      std::cout << "Test Case 5 Failed: Expected imm = 0x0000, got 0x" << std::hex
                << static_cast<int>(get_imm()) << std::dec << std::endl;
      return false;
    }
    if (!get_is_matrix_op()) {
      std::cout << "Test Case 5 Failed: Expected is_matrix_op to be true" << std::endl;
      return false;
    }
    if (get_uses_dma()) {
      std::cout << "Test Case 5 Failed: Expected uses_dma to be false" << std::endl;
      return false;
    }
    if (get_needs_sync()) {
      std::cout << "Test Case 5 Failed: Expected needs_sync to be false" << std::endl;
      return false;
    }
    std::cout << "Test Case 5 Passed" << std::endl;
    return true;
  }

  bool test_case_6() {
    std::cout << "\nRunning Test Case 6 (DMA_CHK Decoding)..." << std::endl;
    // Construct: opcode = 0x5, rd = 2, rs1 = 0, rs2 = 0, imm = 0x0000.
    dut_->fetch_instruction = (0x5 << 28) | (2 << 24) | (0 << 20) | (0 << 16) | 0x0000;
    dut_->fetch_valid = 1;
    tick();

    if (!dut_->decode_valid) {
      std::cout << "Test Case 6 Failed: decode_valid is false" << std::endl;
      return false;
    }
    if (get_opcode() != 0x5) {
      std::cout << "Test Case 6 Failed: Expected opcode DMA_CHK (0x5), got 0x" << std::hex
                << static_cast<int>(get_opcode()) << std::dec << std::endl;
      return false;
    }
    if (get_rd() != 2) {
      std::cout << "Test Case 6 Failed: Expected rd = 2, got " << static_cast<int>(get_rd())
                << std::endl;
      return false;
    }
    if (get_rs1() != 0 || get_rs2() != 0) {
      std::cout << "Test Case 6 Failed: Expected rs1 and rs2 to be 0" << std::endl;
      return false;
    }
    if (get_imm() != 0x0000) {
      std::cout << "Test Case 6 Failed: Expected imm = 0x0000, got 0x" << std::hex
                << static_cast<int>(get_imm()) << std::dec << std::endl;
      return false;
    }
    if (get_is_matrix_op()) {
      std::cout << "Test Case 6 Failed: Expected is_matrix_op to be false" << std::endl;
      return false;
    }
    if (!get_uses_dma()) {
      std::cout << "Test Case 6 Failed: Expected uses_dma to be true" << std::endl;
      return false;
    }
    if (get_needs_sync()) {
      std::cout << "Test Case 6 Failed: Expected needs_sync to be false" << std::endl;
      return false;
    }
    std::cout << "Test Case 6 Passed" << std::endl;
    return true;
  }

  bool test_case_7() {
    std::cout << "\nRunning Test Case 7 (CONF Decoding)..." << std::endl;
    // Construct: opcode = 0x6, rd = 1, rs1 = 0, rs2 = 0, imm = 0x0F0F.
    dut_->fetch_instruction = (0x6 << 28) | (1 << 24) | (0 << 20) | (0 << 16) | 0x0F0F;
    dut_->fetch_valid = 1;
    tick();

    if (!dut_->decode_valid) {
      std::cout << "Test Case 7 Failed: decode_valid is false" << std::endl;
      return false;
    }
    if (get_opcode() != 0x6) {
      std::cout << "Test Case 7 Failed: Expected opcode CONF (0x6), got 0x" << std::hex
                << static_cast<int>(get_opcode()) << std::dec << std::endl;
      return false;
    }
    if (get_rd() != 1) {
      std::cout << "Test Case 7 Failed: Expected rd = 1, got " << static_cast<int>(get_rd())
                << std::endl;
      return false;
    }
    if (get_imm() != 0x0F0F) {
      std::cout << "Test Case 7 Failed: Expected imm = 0x0F0F, got 0x" << std::hex
                << static_cast<int>(get_imm()) << std::dec << std::endl;
      return false;
    }
    if (get_is_matrix_op() || get_uses_dma() || get_needs_sync()) {
      std::cout << "Test Case 7 Failed: Expected all derived signals to be false" << std::endl;
      return false;
    }
    std::cout << "Test Case 7 Passed" << std::endl;
    return true;
  }

  bool test_case_8() {
    std::cout << "\nRunning Test Case 8 (SYNC Decoding)..." << std::endl;
    // Construct: opcode = 0x7, rd = 0, rs1 = 0, rs2 = 0, imm = 0x0000.
    dut_->fetch_instruction = (0x7 << 28) | (0 << 24) | (0 << 20) | (0 << 16) | 0x0000;
    dut_->fetch_valid = 1;
    tick();

    if (!dut_->decode_valid) {
      std::cout << "Test Case 8 Failed: decode_valid is false" << std::endl;
      return false;
    }
    if (get_opcode() != 0x7) {
      std::cout << "Test Case 8 Failed: Expected opcode SYNC (0x7), got 0x" << std::hex
                << static_cast<int>(get_opcode()) << std::dec << std::endl;
      return false;
    }
    if (get_is_matrix_op() || get_uses_dma()) {
      std::cout << "Test Case 8 Failed: Expected is_matrix_op and uses_dma to be false"
                << std::endl;
      return false;
    }
    if (!get_needs_sync()) {
      std::cout << "Test Case 8 Failed: Expected needs_sync to be true" << std::endl;
      return false;
    }
    std::cout << "Test Case 8 Passed" << std::endl;
    return true;
  }

  bool test_case_9() {
    std::cout << "\nRunning Test Case 9 (LD Decoding)..." << std::endl;
    // Construct: opcode = 0x8, rd = 4, rs1 = 2, rs2 = 0, imm = 0xAAAA.
    dut_->fetch_instruction = (0x8 << 28) | (4 << 24) | (2 << 20) | (0 << 16) | 0xAAAA;
    dut_->fetch_valid = 1;
    tick();

    if (!dut_->decode_valid) {
      std::cout << "Test Case 9 Failed: decode_valid is false" << std::endl;
      return false;
    }
    if (get_opcode() != 0x8) {
      std::cout << "Test Case 9 Failed: Expected opcode LD (0x8), got 0x" << std::hex
                << static_cast<int>(get_opcode()) << std::dec << std::endl;
      return false;
    }
    if (get_rd() != 4) {
      std::cout << "Test Case 9 Failed: Expected rd = 4, got " << static_cast<int>(get_rd())
                << std::endl;
      return false;
    }
    if (get_rs1() != 2) {
      std::cout << "Test Case 9 Failed: Expected rs1 = 2, got " << static_cast<int>(get_rs1())
                << std::endl;
      return false;
    }
    if (get_imm() != 0xAAAA) {
      std::cout << "Test Case 9 Failed: Expected imm = 0xAAAA, got 0x" << std::hex
                << static_cast<int>(get_imm()) << std::dec << std::endl;
      return false;
    }
    if (get_is_matrix_op() || get_uses_dma() || get_needs_sync()) {
      std::cout << "Test Case 9 Failed: Expected all derived signals to be false" << std::endl;
      return false;
    }
    std::cout << "Test Case 9 Passed" << std::endl;
    return true;
  }

  bool test_case_10() {
    std::cout << "\nRunning Test Case 10 (ST Decoding)..." << std::endl;
    // Construct: opcode = 0x9, (rd = Rs1) = 5, rs1 = Rs2 = 6, rs2 = 0 (unused), imm = 0x5555.
    dut_->fetch_instruction = (0x9 << 28) | (5 << 24) | (6 << 20) | (0 << 16) | 0x5555;
    dut_->fetch_valid = 1;
    tick();

    if (!dut_->decode_valid) {
      std::cout << "Test Case 10 Failed: decode_valid is false" << std::endl;
      return false;
    }
    if (get_opcode() != 0x9) {
      std::cout << "Test Case 10 Failed: Expected opcode ST (0x9), got 0x" << std::hex
                << static_cast<int>(get_opcode()) << std::dec << std::endl;
      return false;
    }
    if (get_rd() != 5) {
      std::cout << "Test Case 10 Failed: Expected rd (Rs1) = 5, got " << static_cast<int>(get_rd())
                << std::endl;
      return false;
    }
    if (get_rs1() != 6) {
      std::cout << "Test Case 10 Failed: Expected rs1 (Rs2) = 6, got "
                << static_cast<int>(get_rs1()) << std::endl;
      return false;
    }
    if (get_imm() != 0x5555) {
      std::cout << "Test Case 10 Failed: Expected imm = 0x5555, got 0x" << std::hex
                << static_cast<int>(get_imm()) << std::dec << std::endl;
      return false;
    }
    if (get_is_matrix_op() || get_uses_dma() || get_needs_sync()) {
      std::cout << "Test Case 10 Failed: Expected all derived signals to be false" << std::endl;
      return false;
    }
    std::cout << "Test Case 10 Passed" << std::endl;
    return true;
  }
};
