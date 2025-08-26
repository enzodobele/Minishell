#!/bin/bash

mkdir -p ./tests/minishell
mkdir -p ./tests/bash

rm -f ./tests/minishell/*
rm -f ./tests/bash/*
rm -f ./tests/diff_*

make

export PROJECT_PATH="$(pwd)"

cd $PROJECT_PATH/tests/bash
export DIR="$(pwd)"
bash --posix < ../test_template.sh

cd $PROJECT_PATH/tests/minishell
export DIR="$(pwd)"
../../minishell < ../test_template.sh

cd $PROJECT_PATH

total_tests=0
success_tests=0
fail_tests=0

for bash_file in ./tests/bash/test*.out; do
    test_name=$(basename "$bash_file")
    minishell_file="./tests/minishell/$test_name"
    diff_file="./tests/$test_name"
    total_tests=$((total_tests + 1))
    diff -u "$bash_file" "$minishell_file" > "$diff_file"
    if [ ! -s "$diff_file" ]; then
        rm -f "$diff_file" "$bash_file" "$minishell_file"
        success_tests=$((success_tests + 1))
    else
        fail_tests=$((fail_tests + 1))
    fi
done

echo "Total tests : $total_tests"
echo "Succès      : $success_tests"
echo "Échecs      : $fail_tests"

if [ "$fail_tests" -gt 0 ]; then
    echo "❌ Des différences ont été trouvées entre bash et minishell pour $fail_tests test(s)."
else
    echo "✅ Tous les tests sont identiques entre bash et minishell."
fi

unset DIR
unset PROJECT_PATH