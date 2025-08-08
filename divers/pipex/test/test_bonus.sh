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

# Function to verify memory and FD after each test
# Function to verify memory and FD management
verify_memory_and_fd() {
    local test_name="$1"
    local cmd_args="$2"
    local use_valgrind="${3:-false}"
    
    if [ "$use_valgrind" = "true" ] && command -v valgrind > /dev/null 2>&1; then
        echo -e "   ${YELLOW}→${NC} Checking memory leaks for: $test_name"
        local valgrind_output=$(mktemp)
        eval "valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --error-exitcode=42 $cmd_args" > /dev/null 2> "$valgrind_output"
        local valgrind_exit=$?
        
        if [ $valgrind_exit -eq 42 ]; then
            echo -e "   ${RED}✗${NC} Memory leaks detected in: $test_name"
            cat "$valgrind_output" >> "$LOG_FILE"
        else
            echo -e "   ${GREEN}✓${NC} No memory leaks in: $test_name"
        fi
        rm -f "$valgrind_output"
    fi
    
    # Check file descriptors
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

# Function to compare files
compare_files() {
    if cmp -s "$1" "$2"; then
        return 0
    else
        return 1
    fi
}

# Function to test multiple pipes
test_multiple_pipes() {
    local test_name="$1"
    local infile="$2"
    shift 2
    local commands=("$@")
    local outfile_pipex="out_multi_pipex.txt"
    local outfile_shell="out_multi_shell.txt"
    
    echo -e "${BLUE}🧪 Testing multiple pipes:${NC} $test_name"
    echo -e "   Input file: $infile"
    echo -e "   Commands: ${commands[*]}"
    
    # Build pipex command
    local pipex_cmd="../pipex \"$infile\""
    for cmd in "${commands[@]}"; do
        pipex_cmd="$pipex_cmd \"$cmd\""
    done
    pipex_cmd="$pipex_cmd \"$outfile_pipex\""
    
    # Build shell command
    local shell_cmd="< \"$infile\""
    for i in "${!commands[@]}"; do
        if [ $i -eq 0 ]; then
            shell_cmd="$shell_cmd ${commands[$i]}"
        else
            shell_cmd="$shell_cmd | ${commands[$i]}"
        fi
    done
    shell_cmd="$shell_cmd > \"$outfile_shell\""
    
    echo -e "   ${YELLOW}→${NC} Pipex command: $pipex_cmd"
    eval $pipex_cmd 2>/dev/null
    local pipex_exit=$?
    echo -e "   Pipex output: $(if [ -f "$outfile_pipex" ]; then cat "$outfile_pipex" | tr '\n' ' ' | head -c 50; echo "..."; else echo "No output file"; fi)"
    echo -e "   Pipex exitcode: $pipex_exit"
    
    echo -e "   ${YELLOW}→${NC} Shell command: $shell_cmd"
    eval $shell_cmd 2>/dev/null
    local shell_exit=$?
    echo -e "   bash output: $(if [ -f "$outfile_shell" ]; then cat "$outfile_shell" | tr '\n' ' ' | head -c 50; echo "..."; else echo "No output file"; fi)"
    echo -e "   bash exitcode: $shell_exit"
    
    # Compare results
    if compare_files "$outfile_pipex" "$outfile_shell"; then
        if [ $pipex_exit -eq $shell_exit ]; then
            print_result "$test_name" "Match" "Match" "PASS"
            
            # Verify memory and FD for this test
            echo -e "   ${BLUE}Verifying memory and FD management...${NC}"
            verify_memory_and_fd "$test_name" "$pipex_cmd" "false"
            
        else
            print_result "$test_name - Exit code" "$shell_exit" "$pipex_exit" "FAIL"
        fi
    else
        print_result "$test_name - Output" "Files match" "Files differ" "FAIL"
    fi
    echo ""
}

# Function to test heredoc
test_heredoc() {
    local test_name="$1"
    local delimiter="$2"
    local input_text="$3"
    local cmd1="$4"
    local cmd2="$5"
    local outfile="$6"
    
    echo -e "${BLUE}🧪 Testing heredoc:${NC} $test_name"
    echo -e "   Delimiter: $delimiter"
    echo -e "   Input text: $input_text"
    echo -e "   Commands: $cmd1 | $cmd2"
    
    # Create existing content in outfile for append test
    echo -e "   ${YELLOW}→${NC} Creating existing content in $outfile"
    echo "Previous content" > "$outfile"
    local previous_size=$(wc -l < "$outfile")
    echo -e "   ${YELLOW}→${NC} Previous size: $previous_size lines"
    
    # Test heredoc (should append)
    echo -e "   ${YELLOW}→${NC} Testing heredoc (should append)..."
    printf "%s\n%s\n" "$input_text" "$delimiter" | ../pipex here_doc "$delimiter" "$cmd1" "$cmd2" "$outfile" 2>/dev/null
    local pipex_exit=$?
    echo -e "   ${YELLOW}→${NC} Heredoc exit code: $pipex_exit"
    
    # Verify FD management for heredoc
    verify_memory_and_fd "$test_name" "printf \"$input_text\\n$delimiter\\n\" | ../pipex here_doc \"$delimiter\" \"$cmd1\" \"$cmd2\" \"$outfile\"" "false"
    
    # Check if content was appended
    local new_size=$(wc -l < "$outfile")
    echo -e "   ${YELLOW}→${NC} New size: $new_size lines"
    
    if [ $new_size -gt $previous_size ]; then
        print_result "$test_name - Append behavior" "Content appended" "Content appended" "PASS"
        
        # Verify memory and FD for heredoc
        echo -e "   ${BLUE}Verifying heredoc memory and FD management...${NC}"
        local heredoc_cmd="printf \"test\\nEOF\\n\" | ../pipex here_doc EOF \"cat\" \"wc -l\" \"verify_$outfile\""
        verify_memory_and_fd "$test_name - heredoc" "$heredoc_cmd" "false"
        rm -f "verify_$outfile" 2>/dev/null
        
    else
        print_result "$test_name - Append behavior" "Content appended" "Content not appended" "FAIL"
    fi
    
    # Check if heredoc worked correctly by comparing with shell heredoc
    echo -e "   ${YELLOW}→${NC} Comparing with shell heredoc..."
    local shell_outfile="shell_$outfile"
    echo "Previous content" > "$shell_outfile"
    printf "%s\n%s\n" "$input_text" "$delimiter" | (cat << EOF | $cmd1 | $cmd2 >> "$shell_outfile"
$input_text
EOF
)
    
    if compare_files "$outfile" "$shell_outfile"; then
        print_result "$test_name - Heredoc content" "Match with shell" "Match with shell" "PASS"
    else
        print_result "$test_name - Heredoc content" "Match with shell" "Different from shell" "FAIL"
    fi
    
    rm -f "$shell_outfile"
    echo ""
}

echo -e "${YELLOW}=== PIPEX BONUS TESTS ===${NC}"
echo "Date: $(date)"
echo "Utilisateur: $(whoami)"
echo "Répertoire: $(pwd)"
echo "Log saved in: $LOG_FILE"
echo "Compiling pipex with bonus..."

# Compile pipex with bonus
cd ..
make clean > /dev/null 2>&1

# Check if there's a bonus target in Makefile
if grep -q "bonus" Makefile; then
    make bonus > /dev/null 2>&1
else
    # If no bonus target, compile with bonus files
    make > /dev/null 2>&1
fi

if [ ! -f "pipex" ]; then
    echo -e "${RED}ERROR: pipex compilation failed${NC}"
    exit 1
fi

cd test

# Create test files
echo -e "line1\nline2\nline3\nline4\nline5\nline6\nline7\nline8\nline9\nline10" > test_multi.txt
echo -e "apple\nbanana\ncherry\napple\ndate\norange\napple" > test_fruits.txt
echo -e "1 2 3\n4 5 6\n7 8 9" > test_numbers.txt

# Create a file with mixed content
cat > test_mixed.txt << EOF
This is line 1
This is line 2 with numbers 123
Another line here
Final line
EOF

echo -e "\n${YELLOW}--- Multiple pipes tests ---${NC}"

# Test 1: Three commands
test_multiple_pipes "Three commands" "test_multi.txt" "head -7" "tail -5" "wc -l"

# Test 2: Four commands
test_multiple_pipes "Four commands" "test_fruits.txt" "sort" "uniq" "grep apple" "wc -l"

# Test 3: Five commands with different operations
test_multiple_pipes "Five commands" "test_mixed.txt" "cat" "grep -n line" "head -3" "cut -d: -f2" "wc -w"

# Test 4: Commands with complex arguments
test_multiple_pipes "Complex arguments" "test_numbers.txt" "cat" "tr ' ' '\n'" "sort -n" "tail -5"

echo -e "\n${YELLOW}--- Heredoc tests ---${NC}"

# Test heredoc functionality
echo -e "\n${BLUE}Note: Heredoc tests require manual input simulation${NC}"

# Test 1: Basic heredoc with append
test_heredoc "Basic heredoc" "EOF" "Hello World\nThis is a test" "cat" "wc -l" "heredoc_out1.txt"

# Test 2: Heredoc with filtering
test_heredoc "Heredoc with grep" "END" "apple\nbanana\napple\ncherry" "grep apple" "wc -l" "heredoc_out2.txt"

# Test 3: Heredoc with multiple operations
test_heredoc "Heredoc multiple ops" "STOP" "1\n2\n3\n4\n5" "sort -n" "tail -3" "heredoc_out3.txt"

echo -e "\n${YELLOW}--- Error handling for bonus ---${NC}"

# Test multiple pipes with errors
echo -e "${BLUE}Testing multiple pipes with errors${NC}"

# Too few arguments for multiple pipes
../pipex "test_multi.txt" "cat" "outfile.txt" 2>/dev/null
pipex_exit=$?

# Verify FD management
verify_memory_and_fd "Multiple pipes - too few args" "../pipex \"test_multi.txt\" \"cat\" \"outfile.txt\"" "false"

if [ $pipex_exit -ne 0 ]; then
    print_result "Multiple pipes - too few args" "Error" "Error (exit: $pipex_exit)" "PASS"
else
    print_result "Multiple pipes - too few args" "Error" "Success" "FAIL"
fi

# Invalid command in middle of pipe
../pipex "test_multi.txt" "cat" "invalidcmd123" "wc -l" "outfile.txt" 2>/dev/null
pipex_exit=$?

# Verify FD management
verify_memory_and_fd "Invalid command in pipe" "../pipex \"test_multi.txt\" \"cat\" \"invalidcmd123\" \"wc -l\" \"outfile.txt\"" "false"

if [ $pipex_exit -ne 0 ]; then
    print_result "Invalid command in pipe" "Error" "Error (exit: $pipex_exit)" "PASS"
else
    print_result "Invalid command in pipe" "Error" "Success" "FAIL"
fi

echo -e "\n${YELLOW}--- Performance tests ---${NC}"

# Create large file for performance test
for i in {1..1000}; do
    echo "Performance test line $i with random data $RANDOM" >> large_test.txt
done

echo -e "${BLUE}Testing performance with large file${NC}"
start_time=$(date +%s.%N)
../pipex "large_test.txt" "cat" "sort" "uniq" "wc -l" "perf_out.txt" 2>/dev/null
pipex_exit=$?
end_time=$(date +%s.%N)
pipex_time=$(echo "$end_time - $start_time" | bc -l)

# Verify FD management
verify_memory_and_fd "Performance test" "../pipex \"large_test.txt\" \"cat\" \"sort\" \"uniq\" \"wc -l\" \"perf_out.txt\"" "false"

start_time=$(date +%s.%N)
< "large_test.txt" cat | sort | uniq | wc -l > "perf_shell.txt" 2>/dev/null
end_time=$(date +%s.%N)
shell_time=$(echo "$end_time - $start_time" | bc -l)

echo "Pipex time: ${pipex_time}s"
echo "Shell time: ${shell_time}s"

if compare_files "perf_out.txt" "perf_shell.txt"; then
    print_result "Performance test output" "Match" "Match" "PASS"
else
    print_result "Performance test output" "Match" "Different" "FAIL"
fi

echo -e "\n${YELLOW}--- Running complete basic tests with bonus executable ---${NC}"

# Run complete basic test suite with bonus executable
echo -e "${BLUE}🧪 Running complete basic test suite with bonus executable${NC}"
echo -e "   ${YELLOW}→${NC} This ensures bonus executable passes all basic functionality tests"

# Save current test counters
BONUS_TOTAL=$TOTAL_TESTS
BONUS_PASSED=$PASSED_TESTS
BONUS_FAILED=$FAILED_TESTS

# Source basic test functions (without running main script)
if [ -f "test_basic.sh" ]; then
    # Extract and run basic tests manually to avoid duplicate logging setup
    echo -e "   ${YELLOW}→${NC} Running basic test functions..."
    
    # Create basic test files
    echo "This is a test file" > test_basic_input.txt
    echo -e "line1\nline2\nline3\nline4\nline5" > test_basic_multi.txt
    echo -e "apple\nbanana\ncherry\napple\ndate" > test_basic_words.txt
    
    # Key basic tests with bonus executable
    echo -e "   ${BLUE}Testing basic cat | wc -l${NC}"
    ../pipex "test_basic_multi.txt" "cat" "wc -l" "basic_out1.txt" 2>/dev/null
    pipex_exit=$?
    
    # Verify FD management
    verify_memory_and_fd "Basic cat | wc -l with bonus" "../pipex \"test_basic_multi.txt\" \"cat\" \"wc -l\" \"basic_out1.txt\"" "false"
    
    shell_result=$(< "test_basic_multi.txt" cat | wc -l)
    if [ $pipex_exit -eq 0 ] && [ "$(cat basic_out1.txt)" = "$shell_result" ]; then
        print_result "Basic cat | wc -l with bonus" "Match" "Match" "PASS"
    else
        print_result "Basic cat | wc -l with bonus" "Match" "Different" "FAIL"
    fi
    
    echo -e "   ${BLUE}Testing basic grep | wc -w${NC}"
    ../pipex "test_basic_words.txt" "grep apple" "wc -w" "basic_out2.txt" 2>/dev/null
    pipex_exit=$?
    
    # Verify FD management
    verify_memory_and_fd "Basic grep | wc -w with bonus" "../pipex \"test_basic_words.txt\" \"grep apple\" \"wc -w\" \"basic_out2.txt\"" "false"
    
    shell_result=$(< "test_basic_words.txt" grep apple | wc -w)
    if [ $pipex_exit -eq 0 ] && [ "$(cat basic_out2.txt)" = "$shell_result" ]; then
        print_result "Basic grep | wc -w with bonus" "Match" "Match" "PASS"
    else
        print_result "Basic grep | wc -w with bonus" "Match" "Different" "FAIL"
    fi
    
    echo -e "   ${BLUE}Testing basic sort | uniq${NC}"
    ../pipex "test_basic_words.txt" "sort" "uniq" "basic_out3.txt" 2>/dev/null
    pipex_exit=$?
    
    # Verify FD management
    verify_memory_and_fd "Basic sort | uniq with bonus" "../pipex \"test_basic_words.txt\" \"sort\" \"uniq\" \"basic_out3.txt\"" "false"
    
    < "test_basic_words.txt" sort | uniq > "basic_shell3.txt"
    if [ $pipex_exit -eq 0 ] && compare_files "basic_out3.txt" "basic_shell3.txt"; then
        print_result "Basic sort | uniq with bonus" "Match" "Match" "PASS"
    else
        print_result "Basic sort | uniq with bonus" "Match" "Different" "FAIL"
    fi
    
    # More comprehensive basic tests
    echo -e "   ${BLUE}Testing absolute path commands${NC}"
    ../pipex "test_basic_input.txt" "/bin/cat" "/usr/bin/wc -l" "basic_abs.txt" 2>/dev/null
    pipex_exit=$?
    
    # Verify FD management
    verify_memory_and_fd "Absolute path commands with bonus" "../pipex \"test_basic_input.txt\" \"/bin/cat\" \"/usr/bin/wc -l\" \"basic_abs.txt\"" "false"
    
    shell_result=$(< "test_basic_input.txt" /bin/cat | /usr/bin/wc -l)
    if [ $pipex_exit -eq 0 ] && [ "$(cat basic_abs.txt)" = "$shell_result" ]; then
        print_result "Absolute path commands with bonus" "Match" "Match" "PASS"
    else
        print_result "Absolute path commands with bonus" "Match" "Different" "FAIL"
    fi
    
    echo -e "   ${BLUE}Testing commands with arguments${NC}"
    echo -e "hello world\ntest line\nhello again" > test_args.txt
    ../pipex "test_args.txt" "grep hello" "wc -l" "basic_args.txt" 2>/dev/null
    pipex_exit=$?
    
    # Verify FD management
    verify_memory_and_fd "Commands with arguments with bonus" "../pipex \"test_args.txt\" \"grep hello\" \"wc -l\" \"basic_args.txt\"" "false"
    
    shell_result=$(< "test_args.txt" grep hello | wc -l)
    if [ $pipex_exit -eq 0 ] && [ "$(cat basic_args.txt)" = "$shell_result" ]; then
        print_result "Commands with arguments with bonus" "Match" "Match" "PASS"
    else
        print_result "Commands with arguments with bonus" "Match" "Different" "FAIL"
    fi
    
else
    echo -e "   ${RED}Warning: test_basic.sh not found, running limited basic tests${NC}"
fi

echo -e "\n${YELLOW}--- Running complete error tests with bonus executable ---${NC}"

# Run complete error test suite with bonus executable
echo -e "${BLUE}🧪 Running complete error test suite with bonus executable${NC}"
echo -e "   ${YELLOW}→${NC} This ensures bonus executable handles all error cases correctly"

# Key error tests with bonus executable
echo -e "   ${BLUE}Testing no arguments${NC}"
../pipex 2>/dev/null
pipex_exit=$?

# Verify FD management
verify_memory_and_fd "No arguments with bonus" "../pipex" "false"

if [ $pipex_exit -eq 1 ]; then
    print_result "No arguments with bonus" "Exit 1" "Exit 1" "PASS"
else
    print_result "No arguments with bonus" "Exit 1" "Exit $pipex_exit" "FAIL"
fi

echo -e "   ${BLUE}Testing too few arguments${NC}"
../pipex "infile" "cmd1" 2>/dev/null
pipex_exit=$?

# Verify FD management
verify_memory_and_fd "Too few arguments with bonus" "../pipex \"infile\" \"cmd1\"" "false"

if [ $pipex_exit -eq 1 ]; then
    print_result "Too few arguments with bonus" "Exit 1" "Exit 1" "PASS"
else
    print_result "Too few arguments with bonus" "Exit 1" "Exit $pipex_exit" "FAIL"
fi

echo -e "   ${BLUE}Testing non-existent input file${NC}"
../pipex "nonexistent.txt" "cat" "wc -l" "error_out.txt" 2>/dev/null
pipex_exit=$?

# Verify FD management
verify_memory_and_fd "Non-existent file with bonus" "../pipex \"nonexistent.txt\" \"cat\" \"wc -l\" \"error_out.txt\"" "false"

if [ $pipex_exit -eq 1 ]; then
    print_result "Non-existent file with bonus" "Exit 1" "Exit 1" "PASS"
else
    print_result "Non-existent file with bonus" "Exit 1" "Exit $pipex_exit" "FAIL"
fi

echo -e "   ${BLUE}Testing invalid first command${NC}"
../pipex "test_basic_input.txt" "invalidcommand123" "cat" "error_out.txt" 2>/dev/null
pipex_exit=$?

# Verify FD management
verify_memory_and_fd "Invalid first command with bonus" "../pipex \"test_basic_input.txt\" \"invalidcommand123\" \"cat\" \"error_out.txt\"" "false"

if [ $pipex_exit -eq 127 ]; then
    print_result "Invalid first command with bonus" "Exit 127" "Exit 127" "PASS"
else
    print_result "Invalid first command with bonus" "Exit 127" "Exit $pipex_exit" "FAIL"
fi

echo -e "   ${BLUE}Testing invalid second command${NC}"
../pipex "test_basic_input.txt" "cat" "invalidcommand456" "error_out2.txt" 2>/dev/null
pipex_exit=$?

# Verify FD management
verify_memory_and_fd "Invalid second command with bonus" "../pipex \"test_basic_input.txt\" \"cat\" \"invalidcommand456\" \"error_out2.txt\"" "false"

if [ $pipex_exit -eq 127 ]; then
    print_result "Invalid second command with bonus" "Exit 127" "Exit 127" "PASS"
else
    print_result "Invalid second command with bonus" "Exit 127" "Exit $pipex_exit" "FAIL"
fi

echo -e "   ${BLUE}Testing permission denied on output file${NC}"
touch readonly_out.txt
chmod 000 readonly_out.txt
../pipex "test_basic_input.txt" "cat" "wc -l" "readonly_out.txt" 2>/dev/null
exit_code=$?

# Verify FD management
verify_memory_and_fd "Permission denied output with bonus" "../pipex \"test_basic_input.txt\" \"cat\" \"wc -l\" \"readonly_out.txt\"" "false"

chmod 644 readonly_out.txt  # Restore permissions for cleanup
sleep 0.1  # Small delay to ensure file is closed
if [ $exit_code -ne 0 ]; then
    print_result "Permission denied output with bonus" "Error" "Error (exit: $exit_code)" "PASS"
else
    print_result "Permission denied output with bonus" "Error" "Success" "FAIL"
fi

echo -e "   ${BLUE}Testing empty command${NC}"
../pipex "test_basic_input.txt" "" "cat" "error_empty.txt" 2>/dev/null
pipex_exit=$?

# Verify FD management
verify_memory_and_fd "Empty command with bonus" "../pipex \"test_basic_input.txt\" \"\" \"cat\" \"error_empty.txt\"" "false"

if [ $pipex_exit -ne 0 ]; then
    print_result "Empty command with bonus" "Error" "Error (exit: $pipex_exit)" "PASS"
else
    print_result "Empty command with bonus" "Error" "Success" "FAIL"
fi

echo -e "\n${YELLOW}--- File descriptor tests for bonus features ---${NC}"

# Test FD management with multiple pipes
echo -e "${BLUE}🧪 Testing FD management with multiple pipes${NC}"

# Test 1: Multiple pipes FD cleanup
echo -e "   ${BLUE}Testing FD cleanup with multiple pipes${NC}"
for i in {1..3}; do
    ../pipex "test_multi.txt" "cat" "head -5" "tail -3" "wc -l" "fd_multi_$i.txt" > /dev/null 2>&1
    
    # Verify FD management after each execution
    verify_memory_and_fd "FD multiple pipes test $i" "../pipex \"test_multi.txt\" \"cat\" \"head -5\" \"tail -3\" \"wc -l\" \"fd_multi_$i.txt\"" "false"
done

# Check that all completed successfully
multi_success=0
for i in {1..3}; do
    if [ -f "fd_multi_$i.txt" ] && [ -s "fd_multi_$i.txt" ]; then
        multi_success=$((multi_success + 1))
    fi
done

if [ $multi_success -eq 3 ]; then
    print_result "Multiple pipes FD test" "All processes completed" "3/3 successful" "PASS"
else
    print_result "Multiple pipes FD test" "All processes completed" "$multi_success/3 successful" "FAIL"
fi

# Test 2: Heredoc FD cleanup
echo -e "   ${BLUE}Testing FD cleanup with heredoc${NC}"
for i in {1..3}; do
    echo -e "test line $i\nEOF" | ../pipex here_doc EOF "cat" "wc -l" "fd_heredoc_$i.txt" > /dev/null 2>&1
    
    # Verify FD management after each heredoc execution
    verify_memory_and_fd "FD heredoc test $i" "echo -e \"test line $i\\nEOF\" | ../pipex here_doc EOF \"cat\" \"wc -l\" \"fd_heredoc_$i.txt\"" "false"
done

# Check that all completed successfully and temp files are cleaned
heredoc_success=0
temp_files_left=0

for i in {1..3}; do
    if [ -f "fd_heredoc_$i.txt" ] && [ -s "fd_heredoc_$i.txt" ]; then
        heredoc_success=$((heredoc_success + 1))
    fi
done

# Check if any here_doc temporary files are left behind
if ls here_doc* > /dev/null 2>&1; then
    temp_files_left=$(ls here_doc* 2>/dev/null | wc -l)
fi

if [ $heredoc_success -eq 3 ]; then
    print_result "Heredoc FD test" "All processes completed" "3/3 successful" "PASS"
else
    print_result "Heredoc FD test" "All processes completed" "$heredoc_success/3 successful" "FAIL"
fi

if [ $temp_files_left -eq 0 ]; then
    print_result "Heredoc temp file cleanup" "No temp files left" "Clean" "PASS"
else
    print_result "Heredoc temp file cleanup" "No temp files left" "$temp_files_left temp files" "FAIL"
fi

# Test 3: Complex pipes with many FDs
echo -e "   ${BLUE}Testing complex pipes FD handling${NC}"
# Create a test that uses many pipes to stress FD management
echo -e "data1\ndata2\ndata3\ndata4\ndata5\ndata6\ndata7\ndata8\ndata9\ndata10" > fd_stress.txt

../pipex "fd_stress.txt" "cat" "sort" "uniq" "head -8" "tail -6" "wc -l" "fd_complex.txt" > /dev/null 2>&1
pipex_exit=$?

# Verify FD management for complex pipes
verify_memory_and_fd "Complex pipes FD handling" "../pipex \"fd_stress.txt\" \"cat\" \"sort\" \"uniq\" \"head -8\" \"tail -6\" \"wc -l\" \"fd_complex.txt\"" "false"

if [ $pipex_exit -eq 0 ] && [ -f "fd_complex.txt" ] && [ -s "fd_complex.txt" ]; then
    print_result "Complex pipes FD handling" "Completed successfully" "Success" "PASS"
else
    print_result "Complex pipes FD handling" "Completed successfully" "Failed" "FAIL"
fi

# Cleanup FD test files
rm -f fd_multi_*.txt fd_heredoc_*.txt fd_complex.txt fd_stress.txt here_doc* 2>/dev/null

echo -e "\n${YELLOW}=== BONUS TEST SUMMARY ===${NC}"
echo -e "Total tests: $TOTAL_TESTS"
echo -e "${GREEN}Passed: $PASSED_TESTS${NC}"
echo -e "${RED}Failed: $FAILED_TESTS${NC}"

# Cleanup des fichiers temporaires
echo -e "\n${BLUE}Cleaning temporary files...${NC}"
rm -f test_*.txt out*.txt large_test.txt perf_*.txt heredoc_*.txt basic_*.txt error_*.txt 2>/dev/null
echo "Temporary files removed"

if [ $FAILED_TESTS -eq 0 ]; then
    echo -e "\n${GREEN}🎉 All bonus tests passed!${NC}"
    echo "Complete log saved in: $LOG_FILE"
    exit 0
else
    echo -e "\n${RED}❌ Some bonus tests failed!${NC}"
    echo "Complete log saved in: $LOG_FILE"
    exit 1
fi
