ROOT_DIR := $(CURDIR)
CORE_DIR := $(ROOT_DIR)/core

HW_FILES := $(wildcard $(CORE_DIR)/*.sv)
MODULES := $(basename $(notdir $(HW_FILES)))
SIM_DIRS := $(addprefix simulation/, $(MODULES))

all: generate_all run_all

generate_all: $(SIM_DIRS)

$(SIM_DIRS): simulation/%: core/%.sv
	@echo "\n=== Generating simulation for $* ==="
	python3 autogen -t testbench -o simulation -n $*

run_all: generate_all
	@echo "\n=== Running all simulations ==="
	@for dir in $(MODULES); do \
		$(MAKE) -C simulation/$$dir -f $$dir.mk || exit 1; \
	done
	@echo "\n✓ All simulations completed successfully"

clean:
	rm -rf simulation/*/

list:
	@echo "Available hardware modules:"
	@for module in $(MODULES); do \
		echo "  - $$module"; \
	done

format:
	@echo "Formatting files..."
	@find . -type f \( -name "*.cc" -o -name "*.h" -o -name "*.c" \) -exec clang-format -i -style=file {} +

.PHONY: all generate_all run_all clean list format
