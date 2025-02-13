ROOT_DIR := $(CURDIR)
CORE_DIR := $(ROOT_DIR)/core

HW_FILES := $(wildcard $(CORE_DIR)/*.sv)
MODULES := $(basename $(notdir $(HW_FILES)))
TEST_DIRS := $(addprefix test/, $(MODULES))

all: generate_all run_all

generate_all: $(TEST_DIRS)

$(TEST_DIRS): test/%: core/%.sv
	@echo "\n=== Generating verilator test for $* ==="
	python3 autogen -t testbench -o test -n $*

run_all: generate_all
	@echo "\n=== Running all simulations ==="
	@for dir in $(MODULES); do \
		$(MAKE) -C test/$$dir -f $$dir.mk || exit 1; \
	done
	@echo "\n✓ All tests completed successfully"

clean:
	rm -rf test/*/

list:
	@echo "Available hardware modules:"
	@for module in $(MODULES); do \
		echo "  - $$module"; \
	done

format:
	@echo "Formatting files..."
	@find . -type f \( -name "*.cc" -o -name "*.h" -o -name "*.c" \) -exec clang-format -i -style=file {} +

.PHONY: all generate_all run_all clean list format
