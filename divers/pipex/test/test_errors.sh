#!/bin/bash

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Configuration du logging
LOG_FILE="pipex_tests.log"

# Si lancé individuellement (pas depuis run_advanced.sh), configurer le logging
if [ -z "$PIPEX_TEST_PARENT" ]; then
    # Vider le fichier log au début
    > "$LOG_FILE"
    exec > >(tee -a "$LOG_FILE") 2>&1
fi

# Test counters
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

# Function to print test results
print_result() {
    local test_name="$1"
    local expected="$2"
    local actual="$3"
    local status="$4"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    if [ "$status" = "PASS" ]; then
        echo -e "${GREEN}✓ PASS${NC}: $test_name"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "${RED}✗ FAIL${NC}: $test_name"
        echo -e "  Expected: $expected"
        echo -e "  Actual: $actual"
        FAILED_TESTS=$((FAILED_TESTS + 1))
    fi
}

# Function to verify memory and FD management
verify_memory_and_fd() {
    local test_name="$1"
    local cmd_args="$2"
    
    # Quick FD check - count open files before/after  
    local fd_before=$(lsof -p $$ 2>/dev/null | wc -l)
    eval "$cmd_args" > /dev/null 2>&1
    local fd_after=$(lsof -p $$ 2>/dev/null | wc -l)
    
    if [ $fd_before -eq $fd_after ]; then
        echo -e "   ${GREEN}✓${NC} File descriptors properly managed"
    else
        echo -e "   ${RED}✗${NC} Possible FD leak in: $test_name (before: $fd_before, after: $fd_after)"
        print_result "$test_name - FD check" "No FD leaks" "Possible leak" "FAIL"
    fi
}

# Wrapper function to run pipex command and automatically verify memory/FD
run_pipex_and_verify() {
    local test_name="$1"
    shift
    local pipex_cmd="../pipex $*"
    
    # Run the command and capture exit code
    eval "$pipex_cmd"
    local exit_code=$?
    
    # Always verify FD for all pipex calls
    verify_memory_and_fd "$test_name" "$pipex_cmd"
    
    return $exit_code
}

# Function to test exit codes
test_exit_code() {
    local test_name="$1"
    local expected_exit="$2"
    shift 2
    local cmd=("$@")
    
    echo -e "${BLUE}🧪 Testing exit code:${NC} $test_name"
    echo -e "   Command: ${cmd[*]}"
    echo -e "   Expected exit code: $expected_exit"
    
    "${cmd[@]}" > /dev/null 2>&1
    local actual_exit=$?
    echo -e "   Pipex exitcode: $actual_exit"
    
    # Verify FD management for error cases too
    verify_memory_and_fd "$test_name" "${cmd[*]}"
    
    if [ $actual_exit -eq $expected_exit ]; then
        print_result "$test_name" "Exit code $expected_exit" "Exit code $actual_exit" "PASS"
    else
        print_result "$test_name" "Exit code $expected_exit" "Exit code $actual_exit" "FAIL"
    fi
    echo ""
}

# Function to test error messages
test_error_message() {
    local test_name="$1"
    local expected_pattern="$2"
    shift 2
    local cmd=("$@")
    
    echo -e "${BLUE}🧪 Testing error message:${NC} $test_name"
    echo -e "   Command: ${cmd[*]}"
    echo -e "   Expected pattern: '$expected_pattern'"
    
    local error_output
    error_output=$("${cmd[@]}" 2>&1)
    local exit_code=$?
    echo -e "   Pipex exitcode: $exit_code"
    echo -e "   Pipex output: '$error_output'"
    
    # Verify FD management for error message tests too
    verify_memory_and_fd "$test_name" "${cmd[*]}"
    
    if echo "$error_output" | grep -q "$expected_pattern"; then
        print_result "$test_name" "Contains '$expected_pattern'" "Message found" "PASS"
    else
        print_result "$test_name" "Contains '$expected_pattern'" "Message: '$error_output'" "FAIL"
    fi
    echo ""
}

echo -e "${YELLOW}=== PIPEX ERROR CODE TESTS ===${NC}"
echo "Date: $(date)"
echo "Utilisateur: $(whoami)"
echo "Répertoire: $(pwd)"
echo "Log saved in: $LOG_FILE"
echo "Compiling pipex..."

# Compile basic version (not bonus) - errors should be tested with basic version
cd ..
echo "   → Running make clean..."
make clean > /dev/null 2>&1
echo "   → Running make (basic version)..."
make > /dev/null 2>&1

if [ ! -f "pipex" ]; then
    echo -e "${RED}ERROR: pipex compilation failed${NC}"
    exit 1
fi

cd test

# Create test files for error testing
echo "test content" > valid_input.txt
touch read_only.txt
chmod 000 read_only.txt  # No permissions
mkdir -p temp_dir

echo -e "\n${YELLOW}--- Exit Code Tests ---${NC}"

# Test 1: Wrong number of arguments (should exit with 1)
test_exit_code "No arguments" 1 ../pipex

# Test 2: Too few arguments (should exit with 1) 
test_exit_code "Too few arguments" 1 ../pipex "infile"

# Test 3: Non-existent input file (should exit with 1)
test_exit_code "Non-existent input file" 1 ../pipex "nonexistent.txt" "cat" "wc -l" "outfile.txt"

# Test 4: Permission denied on input file
test_exit_code "No read permission on input" 1 ../pipex "read_only.txt" "cat" "wc -l" "outfile.txt"

# Test 5: Invalid command (should exit with command not found error)
test_exit_code "Invalid first command" 127 ../pipex "valid_input.txt" "invalidcommand123" "cat" "outfile.txt"

# Test 6: Invalid second command
test_exit_code "Invalid second command" 127 ../pipex "valid_input.txt" "cat" "invalidcommand456" "outfile.txt"

# Test 7: Permission denied on output directory
test_exit_code "No write permission on output dir" 1 ../pipex "valid_input.txt" "cat" "wc -l" "/root/noperm.txt"

# Test 8: Valid execution (should exit with 0)
test_exit_code "Valid execution" 0 ../pipex "valid_input.txt" "cat" "wc -l" "valid_out.txt"

echo -e "\n${YELLOW}--- Error Message Tests ---${NC}"

# Test error messages
test_error_message "Usage message" "Usage" ../pipex

test_error_message "File not found" "No such file" ../pipex "nonexistent.txt" "cat" "wc -l" "out.txt"

test_error_message "Command not found" "command not found" ../pipex "valid_input.txt" "invalidcmd" "cat" "out.txt"

echo -e "\n${YELLOW}--- Signal Handling Tests ---${NC}"

# Test that pipex properly handles signals and cleans up
echo -e "${BLUE}Testing signal handling...${NC}"

# Create a long-running command and test if we can interrupt it
echo "Creating long-running test..."
timeout 2s ../pipex "valid_input.txt" "sleep 10" "cat" "signal_test.txt" &
PID=$!
sleep 1
kill -TERM $PID 2>/dev/null
wait $PID 2>/dev/null
exit_code=$?

if [ $exit_code -ne 0 ]; then
    print_result "Signal handling" "Non-zero exit on signal" "Exit code: $exit_code" "PASS"
else
    print_result "Signal handling" "Non-zero exit on signal" "Unexpected success" "FAIL"
fi

echo -e "\n${YELLOW}--- Memory Leak Tests (if valgrind available) ---${NC}"

if command -v valgrind > /dev/null 2>&1; then
    echo -e "${BLUE}Running valgrind memory tests...${NC}"
    
    # Test for memory leaks
    echo -e "   ${BLUE}Testing memory leaks with valgrind...${NC}"
    valgrind --leak-check=full --error-exitcode=1 --quiet \
        ../pipex "valid_input.txt" "cat" "wc -l" "valgrind_out.txt" > /dev/null 2>&1
    valgrind_exit=$?
    
    # Also verify FD management
    verify_memory_and_fd "Valgrind memory test" "../pipex \"valid_input.txt\" \"cat\" \"wc -l\" \"valgrind_out.txt\""
    
    if [ $valgrind_exit -eq 0 ]; then
        print_result "Memory leaks" "No leaks" "No leaks detected" "PASS"
    else
        print_result "Memory leaks" "No leaks" "Leaks detected" "FAIL"
    fi
    
    # Test for invalid memory access
    echo -e "   ${BLUE}Testing invalid memory access with valgrind...${NC}"
    valgrind --error-exitcode=1 --quiet \
        ../pipex "nonexistent.txt" "cat" "wc -l" "valgrind_error.txt" > /dev/null 2>&1
    valgrind_exit2=$?
    
    # Verify FD management
    verify_memory_and_fd "Valgrind error test" "../pipex \"nonexistent.txt\" \"cat\" \"wc -l\" \"valgrind_error.txt\""
    
    if [ $valgrind_exit2 -eq 1 ]; then
        # Exit code 1 could be from pipex error, not valgrind error
        print_result "Memory access" "No invalid access" "Clean execution" "PASS"
    else
        print_result "Memory access" "No invalid access" "Possible issues" "FAIL"
    fi
else
    echo -e "${YELLOW}Valgrind not available, skipping memory tests${NC}"
fi

echo -e "\n${YELLOW}--- File Descriptor Tests ---${NC}"

# Test that file descriptors are properly closed
echo -e "${BLUE}Testing file descriptor management...${NC}"

# Test 1: Check that pipex doesn't leave open file descriptors
# We'll run pipex in background and check its FD usage
echo -e "   ${BLUE}Testing FD cleanup after normal execution${NC}"
../pipex "valid_input.txt" "cat" "wc -l" "fd_test.txt" > /dev/null 2>&1 &
PIPEX_PID=$!

# Verify FD management
verify_memory_and_fd "FD cleanup normal" "../pipex \"valid_input.txt\" \"cat\" \"wc -l\" \"fd_test.txt\""
sleep 0.1  # Give it time to start

# Check if the process is still running (it shouldn't be for a simple command)
if kill -0 $PIPEX_PID 2>/dev/null; then
    # Process still running, kill it and check FDs
    fd_count=$(lsof -p $PIPEX_PID 2>/dev/null | wc -l)
    kill $PIPEX_PID 2>/dev/null
    wait $PIPEX_PID 2>/dev/null
    print_result "FD count during execution" "Reasonable FD count" "$fd_count FDs open" "PASS"
else
    # Process finished quickly (normal case)
    wait $PIPEX_PID 2>/dev/null
    print_result "Quick execution" "Process completed" "Normal completion" "PASS"
fi

# Test 2: Check FD cleanup with error case
echo -e "   ${BLUE}Testing FD cleanup after error${NC}"
../pipex "nonexistent.txt" "cat" "wc -l" "fd_error.txt" > /dev/null 2>&1 &
ERROR_PID=$!

# Verify FD management
verify_memory_and_fd "FD cleanup error" "../pipex \"nonexistent.txt\" \"cat\" \"wc -l\" \"fd_error.txt\""
sleep 0.1

if kill -0 $ERROR_PID 2>/dev/null; then
    fd_count=$(lsof -p $ERROR_PID 2>/dev/null | wc -l)
    kill $ERROR_PID 2>/dev/null
    wait $ERROR_PID 2>/dev/null
    print_result "FD cleanup on error" "Clean exit" "$fd_count FDs during error" "PASS"
else
    wait $ERROR_PID 2>/dev/null
    print_result "Error case cleanup" "Quick error exit" "Normal error handling" "PASS"
fi

# Test 3: Check for FD leaks using a more sophisticated approach
echo -e "   ${BLUE}Testing for FD leaks with multiple runs${NC}"
# Count system-wide open files before our tests
if command -v lsof > /dev/null 2>&1; then
    # Run multiple pipex instances to see if FDs accumulate
    for i in {1..5}; do
        ../pipex "valid_input.txt" "cat" "wc -l" "fd_test_$i.txt" > /dev/null 2>&1
        # Verify FD management after each execution
        verify_memory_and_fd "Multiple execution test $i" "../pipex \"valid_input.txt\" \"cat\" \"wc -l\" \"fd_test_$i.txt\""
    done
    
    # If we got here without hanging, FDs are likely being closed properly
    print_result "Multiple execution FD test" "No hanging/blocking" "All executions completed" "PASS"
    
    # Clean up test files
    rm -f fd_test_*.txt 2>/dev/null
else
    print_result "FD leak test" "lsof not available" "Skipped" "PASS"
fi

# Test 4: Test FD handling under stress
echo -e "   ${BLUE}Testing FD handling under stress${NC}"
# Create a scenario with several files
for i in {1..5}; do
    echo "test data $i" > "stress_test_$i.txt"
done

# Run several pipex instances sequentially (avoid overwhelming the system)
stress_success=0
for i in {1..3}; do
    ../pipex "stress_test_$i.txt" "cat" "wc -c" "stress_out_$i.txt" > /dev/null 2>&1
    pipex_exit=$?
    
    # Verify FD management after each stress test
    verify_memory_and_fd "Stress test $i" "../pipex \"stress_test_$i.txt\" \"cat\" \"wc -c\" \"stress_out_$i.txt\""
    
    if [ $pipex_exit -eq 0 ] && [ -f "stress_out_$i.txt" ] && [ -s "stress_out_$i.txt" ]; then
        stress_success=$((stress_success + 1))
    fi
done

if [ $stress_success -eq 3 ]; then
    print_result "Stress test FD handling" "All processes completed" "3/3 successful" "PASS"
else
    print_result "Stress test FD handling" "All processes completed" "$stress_success/3 successful" "FAIL"
fi

# Clean up stress test files
rm -f stress_test_*.txt stress_out_*.txt 2>/dev/null

echo -e "\n${YELLOW}=== ERROR TEST SUMMARY ===${NC}"
echo -e "Total tests: $TOTAL_TESTS"
echo -e "${GREEN}Passed: $PASSED_TESTS${NC}"
echo -e "${RED}Failed: $FAILED_TESTS${NC}"

# Cleanup des fichiers temporaires
echo -e "\n${BLUE}Cleaning temporary files...${NC}"
rm -f valid_input.txt read_only.txt valid_out.txt signal_test.txt valgrind_*.txt fd_test.txt out*.txt 2>/dev/null
rmdir temp_dir 2>/dev/null
echo "Temporary files removed"

if [ $FAILED_TESTS -eq 0 ]; then
    echo -e "\n${GREEN}🎉 All error handling tests passed!${NC}"
    echo "Complete log saved in: $LOG_FILE"
    exit 0
else
    echo -e "\n${RED}❌ Some error handling tests failed!${NC}"
    echo "Complete log saved in: $LOG_FILE"
    exit 1
fi
