# Default compiler and build mode
CC = gcc
BUILD_MODE = debug

# Flags for different build modes
DEBUG_FLAGS = -g -O0 -DDEBUG
RELEASE_FLAGS = -O3 -DRELEASE

# Directories
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = $(BUILD_DIR)/bin

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Object files
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%.o, $(SRCS))

# Executable name
EXEC = $(BUILD_DIR)/elc

# Determine CFLAGS based on build mode
ifeq ($(BUILD_MODE), debug)
	CFLAGS = -Wall -Wextra -std=c11 $(DEBUG_FLAGS)
else ifeq ($(BUILD_MODE), release)
	CFLAGS = -Wall -Wextra -std=c11 $(RELEASE_FLAGS)
else
	$(error Invalid build mode specified: $(BUILD_MODE))
endif

# Default target
all: $(EXEC)

# Link object files to create the executable
$(EXEC): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

# Compile source files to object files
$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -f $(OBJS) $(EXEC)

# Phony targets
.PHONY: all clean

# Allows overriding CC and BUILD_MODE from the command line
.PHONY: debug release

debug:
	$(MAKE) BUILD_MODE=debug

release:
	$(MAKE) BUILD_MODE=release

gcc:
	$(MAKE) CC=gcc

clang:
	$(MAKE) CC=clang

