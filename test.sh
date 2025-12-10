#!/bin/bash

# Test Script for Go-Food Multithreading System
# This script runs various tests to verify the system

echo "=========================================="
echo "  Go-Food Multithreading Test Suite"
echo "=========================================="
echo ""

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Check if executable exists
if [ ! -f "./gofood_system" ]; then
    echo -e "${RED}Error: gofood_system executable not found${NC}"
    echo "Please run 'make' first to compile the program"
    exit 1
fi

echo "✓ Executable found"
echo ""

# Test 1: Race Condition Test
echo "=========================================="
echo "Test 1: Race Condition Verification"
echo "=========================================="
echo "Running program 3 times WITHOUT synchronization..."
echo ""

for i in {1..3}
do
    echo "Run $i:"
    ./gofood_system 2>/dev/null | grep "Total Orders Received:"
done

echo ""
echo -e "${YELLOW}Expected: Different order counts (race condition)${NC}"
echo ""

# Test 2: Synchronization Test
echo "=========================================="
echo "Test 2: Synchronization Verification"
echo "=========================================="
echo "Running program 3 times WITH synchronization..."
echo ""

for i in {1..3}
do
    echo "Run $i:"
    ./gofood_system --sync 2>/dev/null | grep "Total Orders Received:"
done

echo ""
echo -e "${GREEN}Expected: All runs show 30 orders (consistent)${NC}"
echo ""

# Test 3: Performance Comparison
echo "=========================================="
echo "Test 3: Performance Comparison"
echo "=========================================="
echo ""

echo "Without synchronization:"
time ./gofood_system > /dev/null 2>&1
echo ""

echo "With synchronization:"
time ./gofood_system --sync > /dev/null 2>&1
echo ""

echo -e "${YELLOW}Expected: Synchronized mode is slower (overhead)${NC}"
echo ""

# Test 4: Memory Mapping Verification
echo "=========================================="
echo "Test 4: Memory Mapping Verification"
echo "=========================================="
echo ""

./gofood_system --sync 2>/dev/null | grep -A 10 "MEMORY MAPPING"

echo ""
echo -e "${GREEN}Expected: Memory addresses displayed${NC}"
echo ""

# Test 5: Data Consistency Test
echo "=========================================="
echo "Test 5: Data Consistency Test"
echo "=========================================="
echo ""

echo "Running synchronized mode and checking final statistics..."
OUTPUT=$(./gofood_system --sync 2>/dev/null)

ORDERS=$(echo "$OUTPUT" | grep "Total Orders Received:" | awk '{print $4}')
PAYMENTS=$(echo "$OUTPUT" | grep "Total Payments Processed:" | awk '{print $4}')
KITCHEN=$(echo "$OUTPUT" | grep "Total Kitchen Processed:" | awk '{print $4}')

echo "Orders Received: $ORDERS"
echo "Payments Processed: $PAYMENTS"
echo "Kitchen Processed: $KITCHEN"

if [ "$ORDERS" == "30" ] && [ "$PAYMENTS" == "30" ] && [ "$KITCHEN" == "30" ]; then
    echo -e "${GREEN}✓ PASS: All counters are consistent (30)${NC}"
else
    echo -e "${RED}✗ FAIL: Counters are inconsistent${NC}"
fi

echo ""

# Summary
echo "=========================================="
echo "  Test Summary"
echo "=========================================="
echo ""
echo "All tests completed!"
echo ""
echo "Next steps:"
echo "1. Review test results"
echo "2. Take screenshots of outputs"
echo "3. Document results in LAPORAN.md"
echo ""
echo "=========================================="
