ROOT_DIR ?= $(CURDIR)/../..
CORE_DIR := $(ROOT_DIR)/core
SIMULATION_DIR := $(ROOT_DIR)/simulation

VERILATOR = verilator
MODULE_NAME = adder32
BUILD_DIR = build_$(MODULE_NAME)
BIN_NAME = sim_$(MODULE_NAME)

VERILATOR_FLAGS = --cc --exe --build --trace \
                  -I$(CORE_DIR) \
                  -I$(SIMULATION_DIR) \
                  --top-module $(MODULE_NAME) \
                  -DVM_SC=0

SRC = $(MODULE_NAME)_main.cc

all: build run

build:
	$(VERILATOR) $(VERILATOR_FLAGS) $(SRC) $(CORE_DIR)/$(MODULE_NAME).sv \
	-o $(BIN_NAME) --Mdir $(BUILD_DIR)

run: build
	./$(BUILD_DIR)/$(BIN_NAME)

clean:
	rm -rf $(BUILD_DIR) $(BIN_NAME) *.vcd

.PHONY: all build run clean