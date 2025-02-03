VERILATOR = verilator
VERILATOR_FLAGS = --trace -cc -Irtl/include
TOP_MODULE = mux_2_tb
SRC_DIR = rtl/core
TB_DIR = testbench
BUILD_DIR = obj_dir
VCD_FILE = dump.vcd

VERILOG_SOURCES = $(SRC_DIR)/mux_2.sv
TESTBENCH =  $(wildcard $(TB_DIR)/*.sv)
CPP_SOURCES = $(TB_DIR)/sim_main.cpp

all: compile run

compile:
	$(VERILATOR) $(VERILATOR_FLAGS) --top-module $(TOP_MODULE) --exe $(CPP_SOURCES) $(VERILOG_SOURCES) $(TESTBENCH)
	make -C $(BUILD_DIR) -f V$(TOP_MODULE).mk

run:
	@echo "Running simulation..."
	./$(BUILD_DIR)/V$(TOP_MODULE)

wave:
	gtkwave $(VCD_FILE) &

clean:
	rm -rf $(BUILD_DIR) $(VCD_FILE)

.PHONY: all compile run wave clean