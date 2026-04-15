CC = gcc
CFLAGS = -Wall -Wextra -O0 -fprofile-arcs -ftest-coverage -Iincludes
LDFLAGS = -lgcov -lsqlite3

SRC_DIR = src
TEST_DIR = tests
BUILD_DIR = build

SOURCES = $(filter-out $(SRC_DIR)/main.c,$(wildcard $(SRC_DIR)/*.c))

TEST_FILES = $(wildcard $(TEST_DIR)/t_*.c)
TEST_TARGETS = $(patsubst $(TEST_DIR)/t_%.c,$(BUILD_DIR)/test_%,$(TEST_FILES))

all: $(TEST_TARGETS)

$(BUILD_DIR)/test_%: $(TEST_DIR)/t_%.c $(SOURCES)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test: $(TEST_TARGETS)
	@echo "Running all tests..."
	@for test in $(TEST_TARGETS); do \
		echo "=== $$test ==="; \
		./$$test; \
		if [ $$? -ne 0 ]; then \
			echo "Test $$test failed"; \
			exit 1; \
		fi \
	done

coverage: test
	gcovr -r . --branches --cobertura coverage.xml
	gcovr -r . --branches --html coverage.html
	gcovr -r . --branches

clean:
	rm -rf $(BUILD_DIR) *.gcda *.gcno *.gcov coverage.xml coverage.html test.db

check: test

distcheck: test coverage

.PHONY: all test coverage clean check distcheck