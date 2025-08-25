
echo "Test 1 : echo simple"
echo Hello World > ./tests/bash/test1.out
echo $? >> ./tests/bash/test1.out

echo "Test 2 : redirection d'entrée"
cat < ./Makefile > ./tests/bash/test2.out
echo $? >> ./tests/bash/test2.out

echo "Test 3 : pipeline"
ls | wc -l > ./tests/bash/test3.out
echo $? >> ./tests/bash/test3.out

exit 0