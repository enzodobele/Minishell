#!/bin/bash

# Script de test rapide pour pipex
# Usage: ./quick_test.sh

GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Configuration du logging
LOG_FILE="pipex_tests.log"
exec > >(tee "$LOG_FILE") 2>&1

echo -e "${BLUE}=== Test Rapide Pipex ===${NC}"
echo "Date: $(date)"
echo "Utilisateur: $(whoami)"
echo "Répertoire: $(pwd)"
echo "Log saved in: $LOG_FILE"

# Compilation
echo -e "\n${BLUE}Compiling...${NC}"
cd ..
make clean > /dev/null 2>&1
if make > /dev/null 2>&1; then
    echo -e "${GREEN}✓ Compilation successful${NC}"
else
    echo -e "${RED}✗ Compilation error${NC}"
    exit 1
fi
cd test

# Préparation des fichiers de test
echo "hello world" > /tmp/test_input.txt

echo -e "\n${BLUE}Tests de base...${NC}"

# Test 1: Commandes valides
echo -e "\n${BLUE}Test 1: Commandes valides${NC}"
../pipex /tmp/test_input.txt "cat" "wc -w" /tmp/test_output.txt
pipex_exit=$?
bash_result=$(cat /tmp/test_input.txt | cat | wc -w)

echo "Pipex output: $(cat /tmp/test_output.txt 2>/dev/null || echo 'erreur')"
echo "Pipex exit: $pipex_exit"
echo "Bash result: $bash_result"

if [ "$pipex_exit" -eq 0 ] && [ "$(cat /tmp/test_output.txt)" = "$bash_result" ]; then
    echo -e "${GREEN}✓ Test réussi${NC}"
else
    echo -e "${RED}✗ Test échoué${NC}"
fi

# Test 2: Fichier inexistant
echo -e "\n${BLUE}Test 2: Fichier inexistant${NC}"
../pipex /tmp/nonexistent.txt "cat" "wc -l" /tmp/test_output.txt 2>/dev/null
pipex_exit=$?

echo "Pipex exit: $pipex_exit"
if [ "$pipex_exit" -eq 1 ]; then
    echo -e "${GREEN}✓ Code de sortie correct (1)${NC}"
else
    echo -e "${RED}✗ Code de sortie incorrect (attendu: 1, reçu: $pipex_exit)${NC}"
fi

# Test 3: Commande invalide
echo -e "\n${BLUE}Test 3: Commande invalide${NC}"
../pipex /tmp/test_input.txt "invalidcommand" "cat" /tmp/test_output.txt 2>/dev/null
pipex_exit=$?

echo "Pipex exit: $pipex_exit"
if [ "$pipex_exit" -eq 127 ]; then
    echo -e "${GREEN}✓ Code de sortie correct (127)${NC}"
else
    echo -e "${RED}✗ Code de sortie incorrect (attendu: 127, reçu: $pipex_exit)${NC}"
fi

# Cleanup temporary files
echo -e "\n${BLUE}Cleaning...${NC}"
rm -f /tmp/test_input.txt /tmp/test_output.txt /tmp/nonexistent.txt
echo "Temporary files removed"

echo -e "\n${BLUE}=== Quick test completed ===${NC}"
echo "Complete log saved in: $LOG_FILE"
