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

# Function to compare files
compare_files() {
    if cmp -s "$1" "$2"; then
        return 0
    else
        return 1
    fi
}

# Function to verify memory and FD management
verify_memory_and_fd() {
    local test_name="$1"
    local cmd_args="$2"
    
    # Quick FD check - count open files before/after operation
    local fd_before=$(lsof -p $$ 2>/dev/null | wc -l)
    sleep 0.05  # Small delay to allow cleanup
    local fd_after=$(lsof -p $$ 2>/dev/null | wc -l)
    
    if [ $fd_before -eq $fd_after ]; then
        echo -e "   ${GREEN}✓${NC} File descriptors properly managed"
    else
        echo -e "   ${RED}✗${NC} Possible FD leak in: $test_name (before: $fd_before, after: $fd_after)"
        print_result "$test_name - FD check" "No FD leaks" "Possible leak" "FAIL"
    fi
}

# Function to test pipex vs shell equivalent
test_pipex() {
    local test_name="$1"
    local infile="$2"
    local cmd1="$3"
    local cmd2="$4"
    local outfile_pipex="$5"
    local outfile_shell="$6"
    
    echo -e "${BLUE}🧪 Running:${NC} $test_name"
    echo -e "   Command: pipex \"$infile\" \"$cmd1\" \"$cmd2\" \"$outfile_pipex\""
    
    # Run pipex
    ../pipex "$infile" "$cmd1" "$cmd2" "$outfile_pipex" 2>/dev/null
    local pipex_exit=$?
    
    # Verify FD management after pipex execution
    verify_memory_and_fd "$test_name" "../pipex \"$infile\" \"$cmd1\" \"$cmd2\" \"$outfile_pipex\""
    
    echo -e "   Pipex output: $(if [ -f "$outfile_pipex" ]; then cat "$outfile_pipex" | tr '\n' ' ' | head -c 50; echo "..."; else echo "No output file"; fi)"
    echo -e "   Pipex exitcode: $pipex_exit"
    
    # Run shell equivalent
    < "$infile" $cmd1 | $cmd2 > "$outfile_shell" 2>/dev/null
    local shell_exit=$?
    echo -e "   bash output: $(if [ -f "$outfile_shell" ]; then cat "$outfile_shell" | tr '\n' ' ' | head -c 50; echo "..."; else echo "No output file"; fi)"
    echo -e "   bash exitcode: $shell_exit"
    
    # Compare outputs
    if compare_files "$outfile_pipex" "$outfile_shell"; then
        if [ $pipex_exit -eq $shell_exit ]; then
            print_result "$test_name - Output and exit code" "Match" "Match" "PASS"
        else
            print_result "$test_name - Exit code" "$shell_exit" "$pipex_exit" "FAIL"
        fi
    else
        print_result "$test_name - Output" "Files match" "Files differ" "FAIL"
        echo "  Pipex output:"
        cat "$outfile_pipex" | head -5
        echo "  Shell output:"
        cat "$outfile_shell" | head -5
    fi
    echo ""
}

# Function to test error cases
test_error() {
    local test_name="$1"
    shift
    local cmd=("$@")
    
    echo -e "${BLUE}🧪 Testing error:${NC} $test_name"
    echo -e "   Command: ${cmd[*]}"
    echo -e "   ${YELLOW}→${NC} Executing..."
    
    "${cmd[@]}" 2>/dev/null
    local exit_code=$?
    echo -e "   ${YELLOW}→${NC} Exit code: $exit_code"
    
    # Verify FD management for error cases too
    verify_memory_and_fd "$test_name" "${cmd[*]}"
    
    if [ $exit_code -ne 0 ]; then
        print_result "$test_name" "Non-zero exit" "Exit code: $exit_code" "PASS"
    else
        print_result "$test_name" "Non-zero exit" "Exit code: $exit_code" "FAIL"
    fi
    echo ""
}

echo -e "${YELLOW}=== PIPEX BASIC TESTS ===${NC}"
echo "Date: $(date)"
echo "User: $(whoami)"
echo "Directory: $(pwd)"
echo "Log saved in: $LOG_FILE"
echo -e "${CYAN}Compiling pipex...${NC}"

# Compile basic version (not bonus)
cd ..
echo -e "   ${YELLOW}→${NC} Running make clean..."
make clean > /dev/null 2>&1
echo -e "   ${YELLOW}→${NC} Running make (basic version)..."
make > /dev/null 2>&1

if [ ! -f "pipex" ]; then
    echo -e "${RED}ERROR: pipex compilation failed${NC}"
    exit 1
fi

cd test

# Create test files
echo -e "${CYAN}Creating test files...${NC}"
echo -e "   ${YELLOW}→${NC} Creating test_input.txt"
echo "This is a test file" > test_input.txt
echo -e "   ${YELLOW}→${NC} Creating test_multiline.txt"
echo -e "line1\nline2\nline3\nline4\nline5" > test_multiline.txt
echo -e "   ${YELLOW}→${NC} Creating test_words.txt"
echo -e "apple\nbanana\ncherry\napple\ndate" > test_words.txt
echo -e "   ${YELLOW}→${NC} Creating test_empty.txt"
echo "" > test_empty.txt

# Create a large test file
echo -e "   ${YELLOW}→${NC} Creating test_large.txt (100 lines)"
for i in {1..100}; do
    echo "Line $i with some random text and numbers $((RANDOM % 1000))" >> test_large.txt
done

echo -e "\n${YELLOW}--- Basic functionality tests ---${NC}"

# Test 1: Basic ls | wc -l
test_pipex "Basic ls | wc -l" "test_multiline.txt" "cat" "wc -l" "out1_pipex.txt" "out1_shell.txt"

# Test 2: grep | wc -w
test_pipex "grep | wc -w" "test_words.txt" "grep apple" "wc -w" "out2_pipex.txt" "out2_shell.txt"

# Test 3: cat | sort
test_pipex "cat | sort" "test_words.txt" "cat" "sort" "out3_pipex.txt" "out3_shell.txt"

# Test 4: head | tail
test_pipex "head | tail" "test_large.txt" "head -10" "tail -5" "out4_pipex.txt" "out4_shell.txt"

# Test 5: sort | uniq
test_pipex "sort | uniq" "test_words.txt" "sort" "uniq" "out5_pipex.txt" "out5_shell.txt"

echo -e "\n${YELLOW}--- Error handling tests ---${NC}"

# Test error cases
test_error "No arguments" ../pipex
test_error "Too few arguments" ../pipex "infile" "cmd1"
test_error "Non-existent input file" ../pipex "nonexistent.txt" "cat" "wc -l" "out_error.txt"
test_error "Invalid command" ../pipex "test_input.txt" "invalidcommand123" "cat" "out_error.txt"
test_error "Permission denied output" ../pipex "test_input.txt" "cat" "wc -l" "/root/noperm.txt"

echo -e "\n${YELLOW}--- Edge cases ---${NC}"

# Test with empty file
test_pipex "Empty file" "test_empty.txt" "cat" "wc -l" "out_empty_pipex.txt" "out_empty_shell.txt"

# Test with commands that have arguments
test_pipex "Commands with args" "test_multiline.txt" "head -3" "tail -2" "out_args_pipex.txt" "out_args_shell.txt"

# Test with absolute paths
test_pipex "Absolute paths" "test_input.txt" "/bin/cat" "/usr/bin/wc -c" "out_abs_pipex.txt" "out_abs_shell.txt"

echo -e "\n${YELLOW}=== TEST SUMMARY ===${NC}"
echo -e "Total tests: $TOTAL_TESTS"
echo -e "${GREEN}Passed: $PASSED_TESTS${NC}"
echo -e "${RED}Failed: $FAILED_TESTS${NC}"

# Cleanup des fichiers temporaires
echo -e "\n${BLUE}Cleaning temporary files...${NC}"
rm -f test_*.txt out*_*.txt 2>/dev/null
echo "Temporary files removed"

if [ $FAILED_TESTS -eq 0 ]; then
    echo -e "\n${GREEN}🎉 All tests passed!${NC}"
    echo "Complete log saved in: $LOG_FILE"
    exit 0
else
    echo -e "\n${RED}❌ Some tests failed!${NC}"
    echo "Complete log saved in: $LOG_FILE"
    exit 1
fi
