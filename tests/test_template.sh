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

echo "Test 2 : cat < ../../Makefile"
cat < ../../Makefile > "test2.out"
echo $? >> "test2.out"

echo "Test 3 : ls | wc -l"
ls | wc -l > "test3.out"
echo $? >> "test3.out"

exit