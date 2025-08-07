#!/bin/bash

# Advanced test runner with real-time progress and animations

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
MAGENTA='\033[0;35m'
NC='\033[0m' # No Color

# Generate log file name
LOG_FILE="pipex_tests.log"

# Clear log file at the beginning
> "$LOG_FILE"

# Set up logging - capture all output to log file
exec > >(tee -a "$LOG_FILE") 2>&1

echo "=== PIPEX ADVANCED TEST LOG ==="
echo "Date: $(date)"
echo "User: $(whoami)"
echo "Directory: $(pwd)"
echo "Log file: $LOG_FILE"
echo "==============================="

# Animation frames for spinner
SPINNER_FRAMES=('⠋' '⠙' '⠹' '⠸' '⠼' '⠴' '⠦' '⠧' '⠇' '⠏')

# Function to cleanup temporary files
cleanup_temp_files() {
    # Remove test output files (silent cleanup)
    local files_to_remove=(
        "out*.txt" "heredoc_out*.txt" "perf_*.txt" "test_*.txt"
        "*_pipex.txt" "*_shell.txt" "outfile.txt"
    )
    
    for pattern in "${files_to_remove[@]}"; do
        for file in $pattern; do
            if [ -f "$file" ] && [ "$file" != "$LOG_FILE" ]; then
                rm -f "$file" 2>/dev/null
            fi
        done 2>/dev/null
    done
}

# Trap to ensure cleanup on exit
trap 'cleanup_temp_files' EXIT

# Function to show spinner
show_spinner() {
    local pid=$1
    local message="$2"
    local i=0
    
    while kill -0 $pid 2>/dev/null; do
        printf "\r${YELLOW}%s${NC} %s" "${SPINNER_FRAMES[i]}" "$message"
        i=$(( (i + 1) % ${#SPINNER_FRAMES[@]} ))
        sleep 0.1
    done
    printf "\r${GREEN}✓${NC} %s\n" "$message"
}

# Function to run command with spinner
run_with_spinner() {
    local message="$1"
    shift
    local cmd=("$@")
    
    "${cmd[@]}" &>/dev/null &
    local pid=$!
    show_spinner $pid "$message"
    wait $pid
    return $?
}

echo -e "${CYAN}╔══════════════════════════════════════╗${NC}"
echo -e "${CYAN}║      PIPEX ADVANCED TEST RUNNER      ║${NC}"
echo -e "${CYAN}╚══════════════════════════════════════╝${NC}"

# Function to run test with live progress
run_advanced_test() {
    local test_script="$1"
    local test_name="$2"
    
    if [ ! -f "$test_script" ]; then
        echo -e "${RED}ERROR: Test script $test_script not found${NC}"
        return 1
    fi
    
    echo -e "\n${MAGENTA}🚀 Launching $test_name${NC}"
    echo -e "${CYAN}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
    echo -e "${BLUE}📂 Script: $test_script${NC}"
    echo -e "${BLUE}⏰ Started: $(date '+%Y-%m-%d %H:%M:%S')${NC}"
    echo -e "${CYAN}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
    
    chmod +x "$test_script"
    
    # Start timer
    local start_time=$(date +%s)
    
    # Run test with unbuffered output and set parent flag
    if PIPEX_TEST_PARENT=1 stdbuf -oL -eL "./$test_script"; then
        local end_time=$(date +%s)
        local duration=$((end_time - start_time))
        echo -e "\n${GREEN}🎉 $test_name completed successfully!${NC}"
        echo -e "${CYAN}⏱️  Duration: ${duration}s${NC}"
        echo -e "${CYAN}⏰ Finished: $(date '+%Y-%m-%d %H:%M:%S')${NC}"
        return 0
    else
        local end_time=$(date +%s)
        local duration=$((end_time - start_time))
        echo -e "\n${RED}💥 $test_name failed!${NC}"
        echo -e "${CYAN}⏱️  Duration: ${duration}s${NC}"
        echo -e "${CYAN}⏰ Finished: $(date '+%Y-%m-%d %H:%M:%S')${NC}"
        return 1
    fi
}

# Change to test directory
cd "$(dirname "$0")"

# Check if pipex exists
if [ ! -f "../pipex" ]; then
    echo -e "${YELLOW}🔨 Pipex not found, compiling...${NC}"
    if run_with_spinner "Compiling pipex" make -C .. clean && run_with_spinner "Building pipex" make -C ..; then
        echo -e "${GREEN}✅ Compilation successful${NC}"
    else
        echo -e "${RED}❌ Compilation failed${NC}"
        exit 1
    fi
fi

# Interactive menu with enhanced options
echo -e "\n${MAGENTA}🎯 Select test suite to run:${NC}"
echo -e "${BLUE}1)${NC} 🔧 Basic version tests (basic + errors)"
echo -e "${BLUE}2)${NC} 🌟 Bonus version tests (basic + errors + bonus features)"
echo -e "${BLUE}3)${NC} ⚠️  Error handling tests only"
echo -e "${BLUE}4)${NC} 🚀 Full test suite (basic version + bonus version)"
echo -e "${BLUE}5)${NC} 📊 Quick validation test"
echo -e "${BLUE}6)${NC} 🚪 Exit"

read -p "$(echo -e "${YELLOW}Enter your choice (1-6): ${NC}")" choice

case $choice in
    1)
        echo -e "\n${CYAN}🔧 Running Basic Version Test Suite...${NC}"
        echo -e "\n${YELLOW}📋 Test Plan:${NC}"
        echo -e "  1️⃣  Basic functionality tests (basic executable)"
        echo -e "  2️⃣  Error handling tests (basic executable)"
        
        run_advanced_test "test_basic.sh" "Basic Tests"
        BASIC_RESULT=$?
        
        sleep 1
        
        run_advanced_test "test_errors.sh" "Error Tests"
        ERROR_RESULT=$?
        
        echo -e "\n${CYAN}╔══════════════════════════════════════╗${NC}"
        echo -e "${CYAN}║       BASIC VERSION SUMMARY          ║${NC}"
        echo -e "${CYAN}╚══════════════════════════════════════╝${NC}"
        
        if [ $BASIC_RESULT -eq 0 ]; then
            echo -e "${GREEN}✅ Basic Tests: PASSED${NC}"
        else
            echo -e "${RED}❌ Basic Tests: FAILED${NC}"
        fi
        
        if [ $ERROR_RESULT -eq 0 ]; then
            echo -e "${GREEN}✅ Error Tests: PASSED${NC}"
        else
            echo -e "${RED}❌ Error Tests: FAILED${NC}"
        fi
        
        echo -e "\n${CYAN}📝 Complete log available in: $LOG_FILE${NC}"
        ;;
    2)
        echo -e "\n${CYAN}🌟 Running Bonus Version Test Suite...${NC}"
        echo -e "\n${YELLOW}📋 Test Plan:${NC}"
        echo -e "  1️⃣  Bonus features tests (bonus executable)"
        echo -e "  2️⃣  All compatibility tests included in bonus script"
        
        run_advanced_test "test_bonus.sh" "Bonus Tests"
        echo -e "\n${CYAN}📝 Complete log available in: $LOG_FILE${NC}"
        ;;
    3)
        run_advanced_test "test_errors.sh" "Error Handling Tests"
        echo -e "\n${CYAN}📝 Complete log available in: $LOG_FILE${NC}"
        ;;
    4)
        echo -e "\n${CYAN}� Running Complete Test Suite...${NC}"
        
        BASIC_RESULT=0
        BONUS_RESULT=0
        ERROR_RESULT=0
        
        echo -e "\n${YELLOW}📋 Test Plan:${NC}"
        echo -e "  1️⃣  Basic version tests (basic + errors)"
        echo -e "  2️⃣  Bonus version tests (bonus features)"
        
        sleep 2
        
        echo -e "\n${CYAN}🔧 Testing Basic Version...${NC}"
        run_advanced_test "test_basic.sh" "Basic Tests"
        BASIC_RESULT=$?
        
        sleep 1
        
        run_advanced_test "test_errors.sh" "Error Tests (Basic)"
        ERROR_RESULT=$?
        
        sleep 1
        
        echo -e "\n${CYAN}🌟 Testing Bonus Version...${NC}"
        run_advanced_test "test_bonus.sh" "Bonus Tests"
        BONUS_RESULT=$?
        
        # Final comprehensive summary
        echo -e "\n${CYAN}╔══════════════════════════════════════╗${NC}"
        echo -e "${CYAN}║         COMPREHENSIVE SUMMARY        ║${NC}"
        echo -e "${CYAN}╚══════════════════════════════════════╝${NC}"
        
        if [ $BASIC_RESULT -eq 0 ]; then
            echo -e "${GREEN}✅ Basic Tests: PASSED${NC}"
        else
            echo -e "${RED}❌ Basic Tests: FAILED${NC}"
        fi
        
        if [ $ERROR_RESULT -eq 0 ]; then
            echo -e "${GREEN}✅ Error Tests (Basic): PASSED${NC}"
        else
            echo -e "${RED}❌ Error Tests (Basic): FAILED${NC}"
        fi
        
        if [ $BONUS_RESULT -eq 0 ]; then
            echo -e "${GREEN}✅ Bonus Tests: PASSED${NC}"
        else
            echo -e "${RED}❌ Bonus Tests: FAILED${NC}"
        fi
        
        total_passed=0
        [ $BASIC_RESULT -eq 0 ] && total_passed=$((total_passed + 1))
        [ $BONUS_RESULT -eq 0 ] && total_passed=$((total_passed + 1))
        [ $ERROR_RESULT -eq 0 ] && total_passed=$((total_passed + 1))
        
        echo -e "\n${CYAN}📊 Overall Score: $total_passed/3 test suites passed${NC}"
        
        if [ $total_passed -eq 3 ]; then
            echo -e "${GREEN}🏆 PERFECT SCORE! Both basic and bonus versions work perfectly! 🏆${NC}"
        elif [ $total_passed -eq 2 ]; then
            echo -e "${YELLOW}🥈 Good job! Minor issues to fix.${NC}"
        elif [ $total_passed -eq 1 ]; then
            echo -e "${YELLOW}🥉 Decent start, but needs work.${NC}"
        else
            echo -e "${RED}📈 Keep working, you'll get there!${NC}"
        fi
        
        echo -e "\n${CYAN}📝 Complete log available in: $LOG_FILE${NC}"
        ;;
    5)
        echo -e "\n${CYAN}⚡ Running quick validation...${NC}"
        if run_with_spinner "Quick validation test" ./quick_test.sh; then
            echo -e "${GREEN}✅ Quick validation passed!${NC}"
        else
            echo -e "${RED}❌ Quick validation failed!${NC}"
        fi
        echo -e "\n${CYAN}📝 Complete log available in: $LOG_FILE${NC}"
        ;;
    6)
        echo -e "${YELLOW}👋 Goodbye!${NC}"
        echo -e "${CYAN}📝 Complete log available in: $LOG_FILE${NC}"
        exit 0
        ;;
    *)
        echo -e "${RED}❌ Invalid choice. Please try again.${NC}"
        echo -e "\n${CYAN}📝 Complete log available in: $LOG_FILE${NC}"
        exit 1
        ;;
esac
