# Compiler and flags
CC := gcc
CXX := g++
CFLAGS := -Wall -Wextra -g -I./src
CXXFLAGS := $(CFLAGS)
LDFLAGS :=

# Directories
SRC_DIR := src
OBJ_DIR := obj/debug
BIN_DIR := bin/debug

# Find all source files
C_SRCS := $(shell find $(SRC_DIR) -name '*.c')
CPP_SRCS := $(shell find $(SRC_DIR) -name '*.cpp')

# Generate object file paths
C_OBJS := $(C_SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
CPP_OBJS := $(CPP_SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
OBJS := $(C_OBJS) $(CPP_OBJS)

# Output executable
TARGET := $(BIN_DIR)/gosilang

# Default target
all: directories $(TARGET)

# Create necessary directories
directories:
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(dir $(OBJS))

# Link the executable
$(TARGET): $(OBJS)
	@echo "Linking $@"
	@$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# Compile C source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) -c $< -o $@

# Compile C++ source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compiling $<"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
clean:
	@echo "Cleaning build files"
	@rm -rf $(OBJ_DIR)/* $(TARGET)

# Deep clean (including all generated files)
distclean: clean
	@echo "Deep cleaning"
	@rm -rf $(OBJ_DIR) $(BIN_DIR)

# Run tests
test: all
	@echo "Running tests..."
	@for test in $(wildcard tests/unit/*); do \
		if [ -x $$test ]; then \
			$$test || exit 1; \
		fi \
	done

# Generate documentation
docs:
	@echo "Generating documentation..."
	# Add documentation generation commands here

# Install
install: all
	@echo "Installing..."
	# Add installation commands here

# Debugging target
debug: CFLAGS += -DDEBUG -g
debug: CXXFLAGS += -DDEBUG -g
debug: all

# Release target
release: CFLAGS += -O2 -DNDEBUG
release: CXXFLAGS += -O2 -DNDEBUG
release: all

# Dependencies
deps:
	@echo "Installing dependencies..."
	# Add dependency installation commands here

# Format source code
format:
	@echo "Formatting source code..."
	@find $(SRC_DIR) -name '*.c' -o -name '*.h' -o -name '*.cpp' | xargs clang-format -i

# Static analysis
analyze:
	@echo "Running static analysis..."
	@cppcheck --enable=all $(SRC_DIR)

# Help target
help:
	@echo "Available targets:"
	@echo "  all        - Build the project (default)"
	@echo "  clean      - Remove build files"
	@echo "  distclean  - Remove all generated files"
	@echo "  test       - Run tests"
	@echo "  docs       - Generate documentation"
	@echo "  install    - Install the project"
	@echo "  debug      - Build with debug symbols"
	@echo "  release    - Build with optimizations"
	@echo "  deps       - Install dependencies"
	@echo "  format     - Format source code"
	@echo "  analyze    - Run static analysis"

# Phony targets
.PHONY: all clean distclean test docs install debug release deps format analyze help directories

# Include generated dependencies
-include $(OBJS:.o=.d)
