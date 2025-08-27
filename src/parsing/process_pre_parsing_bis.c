#include "minishell.h"

int	has_leading_pipe(char *input)
{
	int	i;

	i = 0;
	while (input[i] == ' ' || input[i] == '\t')
		i++;
	if (input[i] == '|' && input[i + 1] && input[i + 1] == '|')
	{
		printf("Minishell : syntax error near unexpected token `||'\n");
		return (1);
	}
	if (input[i] == '|')
	{
		add_history(input);
		printf("Minishell: syntax error near unexpected token `|'\n");
		return (1);
	}
	return (0);
}

int	process_tokens(char *input, t_token **token)
{
	if (!tokenizer(input, token, 0))
		return (0);
	if (!is_token_valid(*token))
	{
		add_history(input);
		ft_tokenlstclear(token);
		return (0);
	}
	return (1);
}

char	*get_token_symbol(t_token_type type)
{
	if (type == PIPE)
		return ("|");
	else if (type == REDIR_OUT)
		return (">");
	else if (type == REDIR_IN)
		return ("<");
	else if (type == REDIR_APPEND)
		return (">>");
	else if (type == HEREDOC)
		return ("<<");
	else if (type == LOGICAL_OR)
		return ("||");
	return ("unknown");
}
