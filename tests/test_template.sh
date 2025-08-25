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

echo "Test 2 : cat < ../../Makefile"
cat < ../../Makefile > "test2.out"
echo $? >> "test2.out"

echo "Test 3 : ls | wc -l"
ls | wc -l > "test3.out"
echo $? >> "test3.out"

exit 0