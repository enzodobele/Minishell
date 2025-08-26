touch test_norights.out
chmod 000 test_norights.out

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
echo $? >> "test1.21.out"
echo $TESTVAR > "test1.21.out"
echo $? >> "test1.21.out"
unset TESTVAR

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

echo "Test 2.1 : cd /tmp ; pwd"
cd /tmp
echo $? >> "$DIR/test2.1.out"
pwd > "$DIR/test2.1.out"
echo $? >> "$DIR/test2.1.out"
cd "$DIR"

echo "Test 2.2 : cd .. ; pwd"
cd ..
echo $? >> "$DIR/test2.2.out"
pwd > "$DIR/test2.2.out"
echo $? >> "$DIR/test2.2.out"
cd "$DIR"

echo "Test 2.3 : cd . ; pwd"
cd .
echo $? >> "$DIR/test2.3.out"
cd "$DIR"

echo "Test 2.4 : cd - ; pwd"
cd -
echo $? >> "$DIR/test2.4.out"
cd "$DIR"

echo "Test 2.5 : cd \$HOME ; pwd"
cd $HOME
echo $? >> "$DIR/test2.5.out"
pwd > "$DIR/test2.5.out"
echo $? >> "$DIR/test2.5.out"
cd "$DIR"

echo "Test 2.6 : cd /chemin/invalide ; pwd"
cd /chemin/invalide
echo $? >> "$DIR/test2.6.out"
cd "$DIR"

echo "Test 2.7 : cd /tmp ; mkdir -p a/b ; cd a/b ; pwd"
cd /tmp
echo $? >> "$DIR/test2.7.out"
mkdir -p a/b
echo $? >> "$DIR/test2.7.out"
cd a/b
echo $? >> "$DIR/test2.7.out"
pwd > "$DIR/test2.7.out"
echo $? >> "$DIR/test2.7.out"
cd "$DIR"

echo "Test 3 : env"
env | grep "USER" > "test3.out"
echo $? >> "test3.out"

echo "Test 3.1 : export TESTVAR=abc"
export TESTVAR=abc
echo $? >> "test3.1.out"

echo "Test 3.2 : unset TESTVAR"
unset TESTVAR
echo $? >> "test3.2.out"

echo "Test 3.3 : export TESTVAR=abc ; env | grep TESTVAR"
export TESTVAR=abc
echo $? >> "test3.3.out"
env | grep TESTVAR >> "test3.3.out"
echo $? >> "test3.3.out"
unset TESTVAR
echo $? >> "test3.3.out"

echo "Test 3.4 : export VAR1=hello VAR2=world ; echo \$VAR1 \$VAR2 ; env | grep VAR1 ; unset VAR1 ; echo \$VAR1"
export VAR1=hello VAR2=world
echo $? >> "test3.4.out"
echo "$VAR1 $VAR2" >> "test3.4.out"
echo $? >> "test3.4.out"
env | grep VAR1 >> "test3.4.out"
echo $? >> "test3.4.out"
unset VAR1
echo $? >> "test3.4.out"
echo "$VAR1" >> "test3.4.out"
echo $? >> "test3.4.out"
unset VAR2

echo "Test 3.5 : export EMPTY ; echo \$EMPTY ; env | grep EMPTY"
export EMPTY
echo $? >> "test3.5.out"
echo "$EMPTY" >> "test3.5.out"
echo $? >> "test3.5.out"
env | grep EMPTY >> "test3.5.out"
echo $? >> "test3.5.out"
unset EMPTY

echo "Test 3.6 : export EMPTY= ; echo \$EMPTY ; env | grep EMPTY"
export EMPTY=
echo $? >> "test3.6.out"
echo "$EMPTY" >> "test3.6.out"
echo $? >> "test3.6.out"
env | grep EMPTY >> "test3.6.out"
echo $? >> "test3.6.out"
unset EMPTY

echo "Test 3.7 : export TESTVAR=abc ; export TESTVAR=def ; echo \$TESTVAR"
export TESTVAR=abc
echo $? >> "test3.7.out"
export TESTVAR=def
echo $? >> "test3.7.out"
echo "$TESTVAR" >> "test3.7.out"
echo $? >> "test3.7.out"
unset TESTVAR

echo "Test 3.8 : unset UNDEFINED"
unset UNDEFINED
echo $? >> "test3.8.out"

echo "Test 3.9 : export 1INVALID=abc"
export 1INVALID=abc
echo $? >> "test3.9.out"
unset 1INVALID

echo "Test 3.10 : export A=1 B=2 C=3 ; echo \$A \$B \$C"
export A=1 B=2 C=3
echo $? >> "test3.10.out"
echo "$A $B $C" >> "test3.10.out"
echo $? >> "test3.10.out"
unset A B C

echo "Test 3.11 : export SPACED = spaced"
export SPACED = spaced
echo $? >> "test3.11.out"
echo "$SPACED" >> "test3.11.out"
echo $? >> "test3.11.out"
unset SPACED

exit