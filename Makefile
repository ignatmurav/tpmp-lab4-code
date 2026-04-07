CC = gcc
CFLAGS = -Wall -Wextra -O0 -fprofile-arcs -ftest-coverage -Iincludes
LDFLAGS = -lgcov -lsqlite3

SRC_DIR = src
TEST_DIR = tests
BUILD_DIR = build

SOURCES = $(wildcard $(SRC_DIR)/*.c)
TEST_SOURCES = $(wildcard $(TEST_DIR)/*.c)

OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SOURCES))
TEST_OBJECTS = $(patsubst $(TEST_DIR)/%.c,$(BUILD_DIR)/%.o,$(TEST_SOURCES))

TARGET = $(BUILD_DIR)/test_runner

all: $(TARGET)

$(TARGET): $(OBJECTS) $(TEST_OBJECTS)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TARGET)
	./$(TARGET)

coverage: test
	gcovr -r . --branches --cobertura coverage.xml
	gcovr -r . --branches --html coverage.html
	gcovr -r . --branches

clean:
	rm -rf $(BUILD_DIR) *.gcda *.gcno *.gcov coverage.xml coverage.html

check: test

distcheck: test coverage

.PHONY: all test coverage clean check distcheck