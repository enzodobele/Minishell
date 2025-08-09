#include "../includes/minishell.h"

int is_token_valid(t_token *token)
{
	t_token *next_token;

	while (token)
	{
		next_token = token->next;
		if (token -> type == PIPE || token -> type == LOGICAL_OR)
		{
			if (token -> type == LOGICAL_OR)
				return (printf("LOGICAL OR not handled\n"), 0);
			if (next_token -> type == PIPE)
				return (printf("Minishell: syntax error near unexpected token `|'"), 0);
		}
		if (token -> type == REDIR_IN && next_token -> type == PIPE)
			return (printf("Minishell: syntax error near unexpected token `|'"), 0);
		token = token -> next;
	}
	return (1);
}
int is_input_valid(char *input)
{
	int i;

	i = 0;
	while(input[i])
	{
		while(input[i] != '>')
			i++;
		if (input[i] && input[i] == '>')
		{
			i++;
			while (input[i] && (input[i] == ' ' || input[i] == '\t'))
				i++;
			if (input[i] == '|')
				return (printf("Minishell: syntax error near unexpected token `|'\n"), 1);
		}
		i++;
	}
	return (0);
}
