echo "Test 1 : echo"
echo "Hello World" > "test1.out"
echo $? >> "test1.out"

echo "Test 1.1 : echo Hello World"
echo Hello World > "test1.1.out"
echo $? >> "test1.1.out"

echo "Test 1.2 : echo -n"
echo -n "Hello World" > "test1.2.out"
echo $? >> "test1.2.out"

echo "Test 1.3 : echo -n Hello World"
echo -n Hello World > "test1.3.out"
echo $? >> "test1.3.out"

echo "Test 1.4 : echo -n Hello -n World"
echo -n Hello -n World > "test1.4.out"
echo $? >> "test1.4.out"

echo "Test 1.5 : echo (sans argument)"
echo > "test1.5.out"
echo $? >> "test1.5.out"

echo "Test 1.6 : echo avec espaces"
echo "   Hello    World   " > "test1.6.out"
echo $? >> "test1.6.out"

echo "Test 1.7 : echo -n -n -n Hello"
echo -n -n -n Hello > "test1.7.out"
echo $? >> "test1.7.out"

echo "Test 1.8 : echo \"\""
echo "" > "test1.8.out"
echo $? >> "test1.8.out"

echo "Test 1.9 : echo \"Hello $USER\""
echo "Hello $USER" > "test1.9.out"
echo $? >> "test1.9.out"

echo "Test 1.10 : echo \"Hello\nWorld\""
echo -e "Hello\nWorld" > "test1.10.out"
echo $? >> "test1.10.out"

echo "Test 1.11 : echo $USER"
echo $USER > "test1.11.out"
echo $? >> "test1.11.out"

echo "Test 1.12 : echo $PATH"
echo $PATH > "test1.12.out"
echo $? >> "test1.12.out"

echo "Test 1.13 : echo \"Hello $USER\""
echo "Hello $USER" > "test1.13.out"
echo $? >> "test1.13.out"

echo "Test 1.14 : echo \"$USER $PATH\""
echo "$USER $PATH" > "test1.14.out"
echo $? >> "test1.14.out"

echo "Test 1.15 : echo $? (code de sortie précédent)"
echo $? > "test1.15.out"
echo $? >> "test1.15.out"

echo "Test 1.16 : echo \"$MINISHELL\""
echo "$MINISHELL" > "test1.16.out"
echo $? >> "test1.16.out"

echo "Test 1.17 : echo Hello World minishell"
echo Hello World minishell > "test1.17.out"
echo $? >> "test1.17.out"

echo "Test 1.18 : echo \"Hello     World\""
echo "Hello     World" > "test1.18.out"
echo $? >> "test1.18.out"

echo "Test 1.19 : echo \"\" \"\""
echo "" "" > "test1.19.out"
echo $? >> "test1.19.out"

echo "Test 1.20 : echo \$UNDEFINED"
echo $UNDEFINED > "test1.20.out"
echo $? >> "test1.20.out"

echo "Test 1.21 : export TESTVAR=abc ; echo \$TESTVAR"
export TESTVAR=abc
echo $TESTVAR > "test1.21.out"
echo $? >> "test1.21.out"

echo "Test 1.22 : echo \"Value: \$USER\""
echo "Value: $USER" > "test1.22.out"
echo $? >> "test1.22.out"

echo "Test 1.23 : echo \"Hello\$USERWorld\""
echo "Hello$USERWorld" > "test1.23.out"
echo $? >> "test1.23.out"

echo "Test 1.24 : echo \"\$USER-\$PATH-\$HOME\""
echo "$USER-$PATH-$HOME" > "test1.24.out"
echo $? >> "test1.24.out"

echo "Test 1.25 : echo -n \$USER"
echo -n $USER > "test1.25.out"
echo $? >> "test1.25.out"

echo "Test 1.26 : echo \"\$USER!\""
echo "$USER!" > "test1.26.out"
echo $? >> "test1.26.out"

echo "Test 1.27 : echo '\$USER'"
echo '$USER' > "test1.27.out"
echo $? >> "test1.27.out"

echo "Test 1.28 : echo \"\$USER!\""
echo "$USER"$USER'$USER' > "test1.28.out"
echo $? >> "test1.28.out"

exit