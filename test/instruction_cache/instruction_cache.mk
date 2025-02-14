ROOT_DIR ?= $(CURDIR)/../..
CORE_DIR := $(ROOT_DIR)/core
TEST_DIR := $(ROOT_DIR)/test

VERILATOR = verilator
MODULE_NAME = instruction_cache
BUILD_DIR = build_$(MODULE_NAME)
BIN_NAME = test_$(MODULE_NAME)

VERILATOR_FLAGS = --cc --exe --build --trace \
                  -I$(CORE_DIR) \
                  -I$(TEST_DIR) \
                  --top-module $(MODULE_NAME) \
                  -DVM_SC=0

SRC = $(MODULE_NAME)_main.cc

all: build run

build:
	$(VERILATOR) $(VERILATOR_FLAGS) $(SRC) $(CORE_DIR)/$(MODULE_NAME).sv -o $(BIN_NAME) --Mdir $(BUILD_DIR)

run: build
	./$(BUILD_DIR)/$(BIN_NAME)

clean:
	rm -rf $(BUILD_DIR) $(BIN_NAME) *.vcd

.PHONY: all build run clean