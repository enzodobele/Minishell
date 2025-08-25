echo "Test 1 : echo simple"
echo "Hello World" > "test1.out"
echo $? >> "test1.out"

echo "Test 2 : redirection d'entrée"
cat < ../../Makefile > "test2.out"
echo $? >> "test2.out"

echo "Test 3 : pipeline"
ls | wc -l > "test3.out"
echo $? >> "test3.out"

exit 0