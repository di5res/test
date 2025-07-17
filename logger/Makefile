CXX := g++
CXXFLAGS := -std=c++17 -g -Wall -Wextra -pedantic -Iinclude
LDFLAGS := -lpthread

BUILD_DIR := build
SRC_DIR := src
TEST_DIR := test

LIB_NAME := liblogger.so
TEST_EXE := logger_test

.PHONY: all lib test clean

all: lib test

lib: $(BUILD_DIR)/$(LIB_NAME)

test: $(BUILD_DIR)/$(TEST_EXE)
	@echo "Running tests..."
	@./$(BUILD_DIR)/$(TEST_EXE)

$(BUILD_DIR)/$(LIB_NAME): $(SRC_DIR)/Logger.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -shared -fPIC -o $@ $< $(LDFLAGS)

$(BUILD_DIR)/$(TEST_EXE): $(TEST_DIR)/test_logger.cpp $(BUILD_DIR)/$(LIB_NAME) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $< -L$(BUILD_DIR) -llogger $(LDFLAGS)
	@echo "Build complete. Run 'make test' to execute tests."

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)
	rm -f application.log
