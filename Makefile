# Makefile for Go-Food Multithreading System
# Operating Systems Final Project

CC = gcc
CFLAGS = -Wall -Wextra -pthread -g

# Executables
RACE = gofood_race
SYNC = gofood_sync

# Source files
SRC_RACE = main_race_condition.c
SRC_SYNC = main_synchronized.c

# Default target - build both
all: $(RACE) $(SYNC)
	@echo "=========================================="
	@echo "Build successful!"
	@echo "  - $(RACE) (Race Condition Demo)"
	@echo "  - $(SYNC) (Synchronized Version)"
	@echo "=========================================="

# Build race condition version
$(RACE): $(SRC_RACE)
	$(CC) $(CFLAGS) -o $(RACE) $(SRC_RACE)
	@echo "Built: $(RACE)"

# Build synchronized version
$(SYNC): $(SRC_SYNC)
	$(CC) $(CFLAGS) -o $(SYNC) $(SRC_SYNC)
	@echo "Built: $(SYNC)"

# Run race condition demo
run-race: $(RACE)
	@echo "=========================================="
	@echo "Running RACE CONDITION Demo..."
	@echo "=========================================="
	./$(RACE)

# Run synchronized version
run-sync: $(SYNC)
	@echo "=========================================="
	@echo "Running SYNCHRONIZED Version..."
	@echo "=========================================="
	./$(SYNC)

# Run both for comparison
run-both: $(RACE) $(SYNC)
	@echo "=========================================="
	@echo "1. RACE CONDITION Demo (TANPA Sync)"
	@echo "=========================================="
	./$(RACE)
	@echo ""
	@echo ""
	@echo "=========================================="
	@echo "2. SYNCHRONIZED Version (DENGAN Sync)"
	@echo "=========================================="
	./$(SYNC)

# Clean build artifacts
clean:
	rm -f $(RACE) $(SYNC) *.o
	@echo "Clean complete"

# Help target
help:
	@echo "Go-Food Multithreading System - Makefile"
	@echo ""
	@echo "Available targets:"
	@echo "  make          - Build both versions"
	@echo "  make run-race - Run race condition demo (TANPA sync)"
	@echo "  make run-sync - Run synchronized version (DENGAN sync)"
	@echo "  make run-both - Run both for comparison"
	@echo "  make clean    - Remove build artifacts"
	@echo "  make help     - Show this help message"
	@echo ""
	@echo "Executables:"
	@echo "  ./gofood_race - Race condition demo"
	@echo "  ./gofood_sync - Synchronized version"

.PHONY: all clean run-race run-sync run-both help
