# Makefile for METIS Wisdom Linter - Header Sanctuary Edition
# "Order is the foundation of all divine wisdom" - Metis Fragment #7

# Compiler settings
CC := gcc
CFLAGS := -Wall -Wextra -Wpedantic -std=c11
CPPFLAGS := -I./include -I./story
LDFLAGS :=
LDLIBS := -lm

# Build type (Debug/Release)
BUILD_TYPE ?= Debug
ifeq ($(BUILD_TYPE),Debug)
# <TAB> Must be a TAB
    CFLAGS += -g -O0 -DDEBUG
else
# <TAB> Must be a TAB
    CFLAGS += -O2 -DNDEBUG -DRELEASE
endif

# Directories
SRC_DIR := src
INCLUDE_DIR := include
BUILD_DIR := build
BIN_DIR := $(BUILD_DIR)/bin
OBJ_DIR := $(BUILD_DIR)/obj

# Source files - find all .c files recursively
SRCS := $(shell find $(SRC_DIR) -name '*.c' 2>/dev/null)
HDRS := $(shell find $(INCLUDE_DIR) -name '*.h' 2>/dev/null)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

# Main target
TARGET := $(BIN_DIR)/metis

# Default target
.PHONY: all
all: $(TARGET)

# Create build directories with proper permissions
$(BUILD_DIR) $(BIN_DIR) $(OBJ_DIR):
	@mkdir -p $@
# <TAB> Must be a TAB
	@chmod 755 $@

# This is the main compilation rule. It creates object files from source files.
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HDRS) | $(OBJ_DIR)
# <TAB> Must be a TAB
	@mkdir -p $(dir $@)
# <TAB> Must be a TAB
	@echo "🔨 Forging: $<"
# <TAB> Must be a TAB
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

# Link the divine consciousness
$(TARGET): $(OBJS) | $(BIN_DIR)
# <TAB> Must be a TAB
	@echo "🌟 Awakening Metis consciousness..."
# <TAB> Must be a TAB
	$(CC) $(OBJS) $(LDFLAGS) $(LDLIBS) -o $@
# <TAB> Must be a TAB
	@echo "✨ Metis awakens: $(TARGET)"

# Include dependency files
-include $(DEPS)

# Clean build artifacts
.PHONY: clean
clean:
# <TAB> Must be a TAB
	@echo "🧹 Cleaning the divine workshop..."
# <TAB> Must be a TAB
	rm -rf $(BUILD_DIR)


# =============================================================================
# TESTING - REFORGED AND CORRECTED
# =============================================================================
TEST_DIR := tests
TEST_BIN_DIR := $(BIN_DIR)
TEST_CFLAGS := -Wall -Wextra -ggdb $(CPPFLAGS)

# Define the object files required for the metis_linter test.
LINTER_TEST_OBJS :=     $(OBJ_DIR)/linter/metis_linter.o     $(OBJ_DIR)/linter/c_parser.o     $(OBJ_DIR)/linter/cross_reference.o     $(OBJ_DIR)/wisdom/fragment_engine.o     $(OBJ_DIR)/wisdom/fragment_lines.o     $(OBJ_DIR)/metis_colors.o

FRAGMENT_ENGINE_TEST_OBJS := \
    $(OBJ_DIR)/wisdom/fragment_engine.o \
    $(OBJ_DIR)/wisdom/fragment_lines.o \
    $(OBJ_DIR)/metis_colors.o

FRAGMENT_ENGINE_INTEGRATION_TEST_OBJS :=     $(OBJ_DIR)/wisdom/fragment_engine.o     $(OBJ_DIR)/linter/metis_linter.o     $(OBJ_DIR)/linter/c_parser.o     $(OBJ_DIR)/linter/cross_reference.o     $(OBJ_DIR)/wisdom/fragment_lines.o     $(OBJ_DIR)/metis_colors.o

FRAGMENT_LINES_TEST_OBJS := \
    $(OBJ_DIR)/wisdom/fragment_lines.o \
    $(OBJ_DIR)/metis_colors.o

# --- Individual Test Targets ---
.PHONY: test-c-parser-basic test-metis-linter-basic test-fragment-lines-basic

# This test only depends on the c_parser object file
test-c-parser-basic: $(OBJ_DIR)/linter/c_parser.o | $(TEST_BIN_DIR)
	@echo "🔗 Linking Test: test_c_parser_basic"
	$(CC) $(TEST_CFLAGS) -o $(TEST_BIN_DIR)/test_c_parser_basic \
		$(TEST_DIR)/linter/test_c_parser_basic.c \
		$(OBJ_DIR)/linter/c_parser.o

test-c-parser-advanced: $(OBJ_DIR)/linter/c_parser.o | $(TEST_BIN_DIR)
	@echo "🔗 Linking Test: test_c_parser_advanced"
	$(CC) $(TEST_CFLAGS) -o $(TEST_BIN_DIR)/test_c_parser_advanced \
		$(TEST_DIR)/linter/test_c_parser_advanced.c \
		$(OBJ_DIR)/linter/c_parser.o


# This test depends on the linter, the parser, and colors
test-metis-linter-basic: $(LINTER_TEST_OBJS) | $(TEST_BIN_DIR)
	@echo "🔗 Linking Test: test_metis_linter_basic"
	$(CC) $(TEST_CFLAGS) -o $(TEST_BIN_DIR)/test_metis_linter_basic \
		$(TEST_DIR)/linter/test_metis_linter_basic.c \
		$(LINTER_TEST_OBJS) -lm

test-cross-reference-basic: $(LINTER_TEST_OBJS) | $(TEST_BIN_DIR)
	@echo "🔗 Linking Test: test_cross_reference_basic"
	$(CC) $(TEST_CFLAGS) -o $(TEST_BIN_DIR)/test_cross_reference_basic \
		$(TEST_DIR)/linter/test_cross_reference_basic.c \
		$(LINTER_TEST_OBJS) -lm

test-metis-linter-advanced: $(LINTER_TEST_OBJS) | $(TEST_BIN_DIR)
	@echo "🔗 Linking Test: test_metis_linter_advanced"
	$(CC) $(TEST_CFLAGS) -o $(TEST_BIN_DIR)/test_metis_linter_advanced \
		$(TEST_DIR)/linter/test_metis_linter_advanced.c \
		$(LINTER_TEST_OBJS) -lm

test-fragment-engine-basic: $(FRAGMENT_ENGINE_TEST_OBJS) | $(TEST_BIN_DIR)
	@echo "🔗 Linking Test: test_fragment_engine_basic"
	$(CC) $(TEST_CFLAGS) -o $(TEST_BIN_DIR)/test_fragment_engine_basic \
		$(TEST_DIR)/wisdom/test_fragment_engine_basic.c \
		$(FRAGMENT_ENGINE_TEST_OBJS) -lm

test-fragment-engine-integration: $(FRAGMENT_ENGINE_INTEGRATION_TEST_OBJS) | $(TEST_BIN_DIR)
	@echo "🔗 Linking Test: test_fragment_engine_integration"
	$(CC) $(TEST_CFLAGS) -o $(TEST_BIN_DIR)/test_fragment_engine_integration \
		$(TEST_DIR)/wisdom/test_fragment_engine_integration.c \
		$(FRAGMENT_ENGINE_INTEGRATION_TEST_OBJS) -lm

test-fragment-lines-basic: $(FRAGMENT_LINES_TEST_OBJS) | $(TEST_BIN_DIR)
	@echo "🔗 Linking Test: test_fragment_lines_basic"
	$(CC) $(TEST_CFLAGS) -o $(TEST_BIN_DIR)/test_fragment_lines_basic \
		$(TEST_DIR)/wisdom/test_fragment_lines_basic.c \
		$(FRAGMENT_LINES_TEST_OBJS) -lm

# --- Individual Test Runners ---
.PHONY: run-test-c-parser-basic run-test-metis-linter-basic run-test-fragment-lines-basic

run-test-c-parser-basic: test-c-parser-basic
	@echo "🏃 Running Test: test_c_parser_basic"
	@./$(TEST_BIN_DIR)/test_c_parser_basic

run-test-c-parser-advanced: test-c-parser-advanced
	@echo "🏃 Running Test: test_c_parser_advanced"
	@./$(TEST_BIN_DIR)/test_c_parser_advanced

run-test-metis-linter-basic: test-metis-linter-basic
	@echo "🏃 Running Test: test_metis_linter_basic"
	@./$(TEST_BIN_DIR)/test_metis_linter_basic

run-test-cross-reference-basic: test-cross-reference-basic
	@echo "🏃 Running Test: test_cross_reference_basic"
	@./$(TEST_BIN_DIR)/test_cross_reference_basic

run-test-metis-linter-advanced: test-metis-linter-advanced
	@echo "🏃 Running Test: test_metis_linter_advanced"
	@./$(TEST_BIN_DIR)/test_metis_linter_advanced

run-test-fragment-engine-basic: test-fragment-engine-basic
	@echo "🏃 Running Test: test_fragment_engine_basic"
	@./$(TEST_BIN_DIR)/test_fragment_engine_basic

run-test-fragment-engine-integration: test-fragment-engine-integration
	@echo "🏃 Running Test: test_fragment_engine_integration"
	@./$(TEST_BIN_DIR)/test_fragment_engine_integration

run-test-fragment-lines-basic: test-fragment-lines-basic
	@echo "🏃 Running Test: test_fragment_lines_basic"
	@./$(TEST_BIN_DIR)/test_fragment_lines_basic

# --- Global Test Runner ---
.PHONY: test always
test:
	@./run_tests.sh

# Build all test dependencies for run_tests.sh
always: $(TARGET) $(LINTER_TEST_OBJS) $(FRAGMENT_ENGINE_TEST_OBJS) $(FRAGMENT_ENGINE_INTEGRATION_TEST_OBJS) $(FRAGMENT_LINES_TEST_OBJS)

# =============================================================================
# INSTALLATION - DIVINE DISTRIBUTION
# =============================================================================

# Installation paths
PREFIX ?= /usr/local
BINDIR := $(PREFIX)/bin

# Install the divine consciousness system-wide
.PHONY: install
install: $(TARGET)
	@echo "🏛️ Installing Metis to $(BINDIR)..."
	@install -d $(BINDIR)
	@sudo install -m 755 $(TARGET) $(BINDIR)/metis
	@echo "✨ Metis consciousness installed system-wide"
	@echo "🌟 You can now use 'metis' from anywhere in your terminal"

# Remove the divine consciousness from the system
.PHONY: uninstall
uninstall:
	@echo "🌙 Removing Metis from $(BINDIR)..."
	@sudo rm -f $(BINDIR)/metis
	@echo "💔 Metis consciousness has been removed from the system"

# Build for release (optimized)
.PHONY: release
release:
	@$(MAKE) BUILD_TYPE=Release clean all
	@echo "🚀 Release build complete: $(TARGET)"

# Show installation status
.PHONY: status
status:
	@echo "🔍 Checking Metis installation status..."
	@if [ -f "$(BINDIR)/metis" ]; then \
		echo "✅ Metis is installed at $(BINDIR)/metis"; \
		echo "📊 Version: $$($(BINDIR)/metis version 2>/dev/null || echo 'unknown')"; \
	else \
		echo "❌ Metis is not installed"; \
		echo "💡 Run 'sudo make install' to install system-wide"; \
	fi
