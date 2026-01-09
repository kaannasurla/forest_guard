#!/bin/bash

set -e

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

EXECUTABLE="./forest_detector"
DB_FILE="sound_database.dat"

TESTS_RUN=0
TESTS_PASSED=0
TESTS_FAILED=0

print_test() {
    echo -e "${YELLOW}[TEST ${TESTS_RUN}]${NC} $1"
}

print_pass() {
    echo -e "${GREEN}[PASS]${NC} $1"
    ((TESTS_PASSED++))
}

print_fail() {
    echo -e "${RED}[FAIL]${NC} $1"
    ((TESTS_FAILED++))
}

run_test() {
    ((TESTS_RUN++))
    print_test "$1"
}

cleanup() {
    rm -f "$DB_FILE"
}

trap cleanup EXIT

echo "========================================"
echo "Forest Danger Sound Detection Test Suite"
echo "========================================"
echo ""

run_test "Checking if executable exists"
if [ -f "$EXECUTABLE" ]; then
    print_pass "Executable found"
else
    print_fail "Executable not found. Run 'make' first."
    exit 1
fi

run_test "Testing database creation"
if $EXECUTABLE create > /dev/null 2>&1; then
    if [ -f "$DB_FILE" ]; then
        print_pass "Database created successfully"
    else
        print_fail "Database file not created"
    fi
else
    print_fail "Database creation failed"
fi

run_test "Testing list on empty database"
if $EXECUTABLE list | grep -q "Current count: 0"; then
    print_pass "Empty database listed correctly"
else
    print_fail "List command failed on empty database"
fi

run_test "Testing database capacity"
if $EXECUTABLE list | grep -q "Capacity: 100"; then
    print_pass "Database capacity is correct (100)"
else
    print_fail "Database capacity check failed"
fi

run_test "Testing database magic number"
MAGIC=$(xxd -p -l 4 "$DB_FILE" 2>/dev/null)
if [ "$MAGIC" = "53524f46" ] || [ "$MAGIC" = "464f5253" ]; then
    print_pass "Magic number is correct"
else
    print_fail "Magic number is incorrect: $MAGIC"
fi

run_test "Testing database file permissions"
if [ -r "$DB_FILE" ] && [ -w "$DB_FILE" ]; then
    print_pass "Database file has correct permissions"
else
    print_fail "Database file permissions incorrect"
fi

run_test "Testing database file size"
EXPECTED_SIZE=110416
ACTUAL_SIZE=$(stat -f%z "$DB_FILE" 2>/dev/null || stat -c%s "$DB_FILE" 2>/dev/null)
if [ "$ACTUAL_SIZE" -eq "$EXPECTED_SIZE" ]; then
    print_pass "Database file size is correct ($ACTUAL_SIZE bytes)"
else
    print_fail "Database size incorrect. Expected: $EXPECTED_SIZE, Got: $ACTUAL_SIZE"
fi

run_test "Testing invalid command handling"
if ! $EXECUTABLE invalid_command > /dev/null 2>&1; then
    print_pass "Invalid command rejected correctly"
else
    print_fail "Invalid command not handled properly"
fi

run_test "Testing add command without parameters"
if ! $EXECUTABLE add > /dev/null 2>&1; then
    print_pass "Add command requires parameters"
else
    print_fail "Add command should fail without parameters"
fi

run_test "Testing match command without parameters"
if ! $EXECUTABLE match > /dev/null 2>&1; then
    print_pass "Match command requires parameters"
else
    print_fail "Match command should fail without parameters"
fi

if command -v valgrind > /dev/null 2>&1; then
    run_test "Testing for memory leaks with Valgrind"
    VALGRIND_OUTPUT=$(valgrind --leak-check=full --error-exitcode=1 $EXECUTABLE list 2>&1)
    if [ $? -eq 0 ]; then
        print_pass "No memory leaks detected"
    else
        print_fail "Memory leaks detected"
        echo "$VALGRIND_OUTPUT" | grep "definitely lost"
    fi
else
    echo -e "${YELLOW}[SKIP]${NC} Valgrind not installed, skipping memory leak test"
fi

run_test "Testing database recreation"
if $EXECUTABLE create > /dev/null 2>&1; then
    if $EXECUTABLE list | grep -q "Current count: 0"; then
        print_pass "Database recreated successfully"
    else
        print_fail "Database recreation failed"
    fi
else
    print_fail "Database recreation command failed"
fi

echo ""
echo "========================================"
echo "Test Summary"
echo "========================================"
echo "Total tests run: $TESTS_RUN"
echo -e "Tests passed: ${GREEN}$TESTS_PASSED${NC}"
echo -e "Tests failed: ${RED}$TESTS_FAILED${NC}"
echo ""

if [ $TESTS_FAILED -eq 0 ]; then
    echo -e "${GREEN}All tests passed! ✓${NC}"
    exit 0
else
    echo -e "${RED}Some tests failed! ✗${NC}"
    exit 1
fi
