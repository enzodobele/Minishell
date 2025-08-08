#!/bin/bash

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

echo -e "${CYAN}╔══════════════════════════════════════╗${NC}"
echo -e "${CYAN}║          PIPEX TEST SUITE            ║${NC}"
echo -e "${CYAN}╚══════════════════════════════════════╝${NC}"

# Function to cleanup old logs
cleanup_logs() {
    echo -e "\n${BLUE}🧹 Cleaning old logs...${NC}"
    local old_logs=$(find . -name "*.log" -mtime +1 2>/dev/null | wc -l)
    if [ "$old_logs" -gt 0 ]; then
        find . -name "*.log" -mtime +1 -delete 2>/dev/null
        echo -e "${GREEN}✓ $old_logs old logs removed${NC}"
    else
        echo -e "${YELLOW}ℹ No old logs to remove${NC}"
    fi
}

# Function to run a test suite
run_test_suite() {
    local test_script="$1"
    local test_name="$2"
    
    if [ ! -f "$test_script" ]; then
        echo -e "${RED}ERROR: Test script $test_script not found${NC}"
        return 1
    fi
    
    echo -e "\n${YELLOW}🚀 Starting $test_name...${NC}"
    echo -e "${YELLOW}=================================================${NC}"
    echo -e "${CYAN}📂 Test script: $test_script${NC}"
    echo -e "${CYAN}⏰ Start time: $(date '+%H:%M:%S')${NC}"
    
    chmod +x "$test_script"
    
    # Run with live output
    if "./$test_script"; then
        echo -e "\n${GREEN}✅ $test_name completed successfully${NC}"
        echo -e "${CYAN}⏰ End time: $(date '+%H:%M:%S')${NC}"
        return 0
    else
        echo -e "\n${RED}❌ $test_name failed${NC}"
        echo -e "${CYAN}⏰ End time: $(date '+%H:%M:%S')${NC}"
        return 1
    fi
}

# Change to test directory
cd "$(dirname "$0")"

# Initialize counters
BASIC_RESULT=0
BONUS_RESULT=0

# Ask user which tests to run
cleanup_logs
echo -e "${BLUE}Which tests would you like to run?${NC}"
echo "1) Basic tests only"
echo "2) Bonus tests only" 
echo "3) Both basic and bonus tests"
echo "4) Exit"

read -p "Enter your choice (1-4): " choice

case $choice in
    1)
        echo -e "\n${CYAN}Running basic tests only...${NC}"
        run_test_suite "test_basic.sh" "Basic Tests"
        BASIC_RESULT=$?
        ;;
    2)
        echo -e "\n${CYAN}Running bonus tests only...${NC}"
        run_test_suite "test_bonus.sh" "Bonus Tests"
        BONUS_RESULT=$?
        ;;
    3)
        echo -e "\n${CYAN}Running all tests...${NC}"
        
        # Run basic tests first
        run_test_suite "test_basic.sh" "Basic Tests"
        BASIC_RESULT=$?
        
        # Small delay between test suites
        sleep 2
        
        # Run bonus tests
        run_test_suite "test_bonus.sh" "Bonus Tests"  
        BONUS_RESULT=$?
        ;;
    4)
        echo -e "${YELLOW}Exiting...${NC}"
        exit 0
        ;;
    *)
        echo -e "${RED}Invalid choice. Exiting...${NC}"
        exit 1
        ;;
esac

# Final summary
echo -e "\n${CYAN}╔══════════════════════════════════════╗${NC}"
echo -e "${CYAN}║            FINAL SUMMARY             ║${NC}"
echo -e "${CYAN}╚══════════════════════════════════════╝${NC}"

if [ $choice -eq 1 ] || [ $choice -eq 3 ]; then
    if [ $BASIC_RESULT -eq 0 ]; then
        echo -e "${GREEN}Basic Tests: ✅ PASSED${NC}"
    else
        echo -e "${RED}Basic Tests: ❌ FAILED${NC}"
    fi
fi

if [ $choice -eq 2 ] || [ $choice -eq 3 ]; then
    if [ $BONUS_RESULT -eq 0 ]; then
        echo -e "${GREEN}Bonus Tests: ✅ PASSED${NC}"
    else
        echo -e "${RED}Bonus Tests: ❌ FAILED${NC}"
    fi
fi

# Overall result
if [ $choice -eq 3 ]; then
    if [ $BASIC_RESULT -eq 0 ] && [ $BONUS_RESULT -eq 0 ]; then
        echo -e "\n${GREEN}🎉 ALL TESTS PASSED! Your pipex is working correctly! 🎉${NC}"
        exit 0
    else
        echo -e "\n${RED}⚠️  Some tests failed. Please check the output above for details.${NC}"
        exit 1
    fi
elif [ $choice -eq 1 ]; then
    exit $BASIC_RESULT
elif [ $choice -eq 2 ]; then
    exit $BONUS_RESULT
fi
