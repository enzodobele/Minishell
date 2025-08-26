#include "minishell.h"

void	print_syntax_error_doubles(char c)
{
	printf("minishell: syntax error near unexpected token `%c%c'\n", c, c);
}

void	print_syntax_error_singles(char c)
{
	printf("minishell: syntax error near unexpected token `%c'\n", c);
}

void	print_syntax_error_newlines(void)
{
	printf("minishell: syntax error near unexpected token `newline'\n");
}

void	print_syntax_error_pipe(void)
{
	printf("Minishell: syntax error near unexpected token `|'\n");
}
