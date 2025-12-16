# Makefile for FoodGo Multithreading System
# Operating Systems Final Project

CC = gcc
CFLAGS = -Wall -Wextra -pthread -g

# Executables
RACE = foodgo_race
SYNC = foodgo_sync
MMAP = foodgo_mmap

# Source files
SRC_RACE = main_race_condition.c
SRC_SYNC = main_synchronized.c
SRC_MMAP = main_mmap_demo.c

# Default target - build all
all: $(RACE) $(SYNC) $(MMAP)
	@echo "=========================================="
	@echo "Build successful!"
	@echo "  - $(RACE) (Race Condition Demo)"
	@echo "  - $(SYNC) (Synchronized Version)"
	@echo "  - $(MMAP) (Memory Mapping Demo)"
	@echo "=========================================="

# Build race condition version
$(RACE): $(SRC_RACE)
	$(CC) $(CFLAGS) -o $(RACE) $(SRC_RACE)
	@echo "Built: $(RACE)"

# Build synchronized version
$(SYNC): $(SRC_SYNC)
	$(CC) $(CFLAGS) -o $(SYNC) $(SRC_SYNC)
	@echo "Built: $(SYNC)"

# Build memory mapping demo
$(MMAP): $(SRC_MMAP)
	$(CC) $(CFLAGS) -o $(MMAP) $(SRC_MMAP)
	@echo "Built: $(MMAP)"

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

# Run memory mapping demo
run-mmap: $(MMAP)
	@echo "=========================================="
	@echo "Running MEMORY MAPPING Demo..."
	@echo "=========================================="
	./$(MMAP)

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
	rm -f $(RACE) $(SYNC) $(MMAP) *.o order_log.dat
	@echo "Clean complete"

# Help target
help:
	@echo "FoodGo Multithreading System - Makefile"
	@echo ""
	@echo "Available targets:"
	@echo "  make          - Build all versions"
	@echo "  make run-race - Run race condition demo (TANPA sync)"
	@echo "  make run-sync - Run synchronized version (DENGAN sync)"
	@echo "  make run-mmap - Run memory mapping demo"
	@echo "  make run-both - Run both for comparison"
	@echo "  make clean    - Remove build artifacts"
	@echo "  make help     - Show this help message"
	@echo ""
	@echo "Executables:"
	@echo "  ./foodgo_race - Race condition demo"
	@echo "  ./foodgo_sync - Synchronized version"
	@echo "  ./foodgo_mmap - Memory mapping demo"

.PHONY: all clean run-race run-sync run-mmap run-both help
