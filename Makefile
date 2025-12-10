# Makefile for Go-Food Multithreading System
# Operating Systems Final Project

CC = gcc
CFLAGS = -Wall -Wextra -pthread -g
TARGET = gofood_system
SRC = main.c

# Default target
all: $(TARGET)

# Build the main executable
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)
	@echo "Build successful! Executable: $(TARGET)"

# Run without synchronization (demonstrate race condition)
run-race:
	./$(TARGET)

# Run with synchronization (proper implementation)
run-sync:
	./$(TARGET) --sync

# Run both modes for comparison
run-both: $(TARGET)
	@echo "=========================================="
	@echo "Running UNSYNCHRONIZED mode (Race Condition)..."
	@echo "=========================================="
	./$(TARGET)
	@echo ""
	@echo "=========================================="
	@echo "Running SYNCHRONIZED mode (Fixed)..."
	@echo "=========================================="
	./$(TARGET) --sync

# Clean build artifacts
clean:
	rm -f $(TARGET) *.o
	@echo "Clean complete"

# Help target
help:
	@echo "Go-Food Multithreading System - Makefile"
	@echo ""
	@echo "Available targets:"
	@echo "  make          - Build the project"
	@echo "  make run-race - Run without synchronization (race condition demo)"
	@echo "  make run-sync - Run with synchronization (proper implementation)"
	@echo "  make run-both - Run both modes for comparison"
	@echo "  make clean    - Remove build artifacts"
	@echo "  make help     - Show this help message"

.PHONY: all clean run-race run-sync run-both help
