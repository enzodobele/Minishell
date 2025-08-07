#!/bin/bash

# Cleanup script for test directory
# Usage: ./cleanup.sh

GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${BLUE}🧹 Cleaning test directory${NC}"
echo "Date: $(date)"
echo "Répertoire: $(pwd)"

# Nettoyer les logs
echo -e "\n${YELLOW}📄 Cleaning logs...${NC}"

# Fichier de log principal
LOG_FILE="pipex_tests.log"

if [ -f "$LOG_FILE" ]; then
    echo "Log trouvé:"
    ls -lh "$LOG_FILE"
    echo -e "\n${BLUE}Supprimer le fichier de log? (y/N)${NC}"
    read -r response
    if [[ "$response" =~ ^[Yy]$ ]]; then
        rm -f "$LOG_FILE"
        echo -e "${GREEN}✓ Log supprimé${NC}"
    else
        echo -e "${YELLOW}ℹ Log conservé${NC}"
    fi
else
    echo -e "${GREEN}✓ Aucun log à nettoyer${NC}"
fi

# Nettoyer les fichiers temporaires
echo -e "\n${YELLOW}🗂️ Cleaning temporary files...${NC}"
temp_files=(
    "*.txt"
    "out*"
    "test_input*"
    "test_multiline*"
    "test_words*"
    "test_empty*"
    "test_large*"
    "test_multi*"
    "test_fruits*"
    "test_mixed*"
    "test_numbers*"
    "valid_*"
    "heredoc_*"
    "large_*"
    "perf_*"
    "signal_*"
    "valgrind_*"
    "fd_test*"
    "read_only*"
    "*.tmp"
)

files_found=0
for pattern in "${temp_files[@]}"; do
    if ls $pattern 1> /dev/null 2>&1; then
        files_found=1
        echo "Trouvé: $pattern"
        ls -la $pattern
    fi
done

if [ "$files_found" -eq 1 ]; then
    echo -e "\n${BLUE}Supprimer les fichiers temporaires? (y/N)${NC}"
    read -r response
    if [[ "$response" =~ ^[Yy]$ ]]; then
        for pattern in "${temp_files[@]}"; do
            rm -f $pattern 2>/dev/null
        done
        echo -e "${GREEN}✓ Temporary files removed${NC}"
    else
        echo -e "${YELLOW}ℹ Fichiers temporaires conservés${NC}"
    fi
else
    echo -e "${GREEN}✓ Aucun fichier temporaire à nettoyer${NC}"
fi

# Nettoyer les répertoires temporaires
echo -e "\n${YELLOW}📁 Cleaning temporary directories...${NC}"
if [ -d "temp_dir" ]; then
    rmdir temp_dir 2>/dev/null && echo -e "${GREEN}✓ Répertoire temp_dir supprimé${NC}"
fi

echo -e "\n${GREEN}🎉 Cleanup completed!${NC}"
