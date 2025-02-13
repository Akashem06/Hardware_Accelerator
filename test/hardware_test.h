#pragma once

#include <cstdint>
#include <iostream>
#include <memory>

class HardwareTestBase {
 protected:
  uint64_t sim_time_;

 public:
  HardwareTestBase() : sim_time_(0) {}

  virtual ~HardwareTestBase() {}

  void tick(int count = 1) {
    for (int i = 0; i < count; i++) {
      eval_clock_low();
      eval_clock_high();
      sim_time_++;
    }
  }

  virtual void eval_clock_low() = 0;
  virtual void eval_clock_high() = 0;
  virtual void reset() = 0;
  virtual bool run_all_tests() = 0;

 protected:
  void report_test(const std::string &test_name, bool passed) {
    std::cout << test_name << ": " << (passed ? "PASSED" : "FAILED") << std::endl;
  }
};
