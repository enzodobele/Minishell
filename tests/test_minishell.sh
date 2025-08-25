
echo "Test 1 : echo simple"
echo "Hello World" > ./tests/minishell/test1.out
echo $? >> ./tests/minishell/test1.out

echo "Test 2 : redirection d'entrée"
cat < ./Makefile > ./tests/minishell/test2.out
echo $? >> ./tests/minishell/test2.out

echo "Test 3 : pipeline"
ls | wc -l > ./tests/minishell/test3.out
echo $? >> ./tests/minishell/test3.out

echo 
exit 0

