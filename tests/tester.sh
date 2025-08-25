#!/bin/bash

mkdir -p ./tests/minishell
mkdir -p ./tests/bash
rm -f ./tests/minishell/*
rm -f ./tests/bash/*
rm -f ./tests/diff_*

bash --posix < ./tests/test_bash.sh
./minishell < ./tests/test_minishell.sh

diff_found=0

for bash_file in ./tests/bash/test*.out; do
    test_name=$(basename "$bash_file")
    minishell_file="./tests/minishell/$test_name"
    diff_file="./tests/diff_${test_name}"
    diff -u "$bash_file" "$minishell_file" > "$diff_file"
    if [ ! -s "$diff_file" ]; then
        rm -f "$diff_file"
    else
        diff_found=1
    fi
done

if [ "$diff_found" -eq 1 ]; then
    echo "❌ Des différences ont été trouvées entre bash et minishell. Consultez les fichiers diff_*."
else
    echo "✅ Tous les tests sont identiques entre bash et minishell."
fi