#include "minishell.h"

int	tokenizer(char *input, t_token **token)
{
	int	i;
	int start;
	int ret;

	i = 0;
	while (input[i])
	{
		while (input[i] == ' ')
			i++;
		start = i;
		while (input[i] && !is_special_char(input[i]))
			i++;
		if (i > start)
		{
			if (!add_new_token_word(start, i, token, input))
				return (ft_tokenlstclear(token), 0);
		}
		if (is_special_char(input[i]) && input[i])
		{
			ret = is_char_symbol(input, token, i);
			if (ret == 0)
				return (ft_tokenlstclear(token), 0);
			i += ret;
		}
	}
	return (1);
}
int	is_char_symbol(char *input, t_token **token, int i)
{
	if (input[i] == '|')
	{
		if (!add_new_token_symbol_solo(PIPE, i, token, input))
			return (0);
	}
	else if (input[i] == '<')
	{
		if (input[i + 1] && input[i + 1] != '<' )
		{
			if (!add_new_token_symbol_solo(REDIR_IN, i, token, input))
				return (0);
		}
		if (input[i + 1] && input[i + 1] == '<')
		{
			if (!add_new_token_symbol_duo(HEREDOC, i, token, input))
				return (0);
			return (2);
		}
	}
	else if (input[i] == '>')
	{
		if (input[i + 1] && input[i + 1] != '>' )
		{
			if (!add_new_token_symbol_solo(REDIR_OUT, i, token, input))
				return (0);
		}
		if (input[i + 1] && input[i + 1] == '>')
		{
			if (!add_new_token_symbol_duo(REDIR_APPEND, i, token, input))
				return (0);
			return (2);
		}
	}
	return (1);
}
int	add_new_token_symbol_duo(t_token_type symbol, int i, t_token **token, char *input)
{
	t_token *new_token;

	new_token = malloc(sizeof (t_token));
	if (!new_token)
		return (perror("Malloc failed"), 0);
	new_token->string = ft_strdup(input, 2, i);
	if (!new_token->string)
		return(perror("Malloc failed"), free(new_token), 0);
	new_token->type = symbol;
	new_token->next = NULL;
	ft_lstadd_back(token, new_token);
	return (1);
}
int	add_new_token_symbol_solo(t_token_type symbol, int i, t_token **token, char *input)
{
	t_token *new_token;

	new_token = malloc(sizeof (t_token));
	if (!new_token)
		return (perror("Malloc failed"), 0);
	new_token->string = ft_strdup(input, 1, i);
	if (!new_token->string)
		return(perror("Malloc failed"), free(new_token), 0);
	new_token->type = symbol;
	new_token->next = NULL;
	ft_lstadd_back(token, new_token);
	return (1);
}
int	add_new_token_word(int start, int i, t_token **token, char *input)
{
	t_token *new_token;

	new_token = malloc(sizeof (t_token));
	if (!new_token)
		return (perror("Malloc failed"), 0);
	new_token->string = ft_strdup(input, (i - start), start);
	if (!new_token->string)
		return(perror("Malloc failed"), free(new_token), 0);
	new_token->type = WORD;
	new_token->next = NULL;
	ft_lstadd_back(token, new_token);
	return (1);
}
int is_special_char(char c)
{
    return (c == '<' || c == '>' || c == '|' ||
            c == '$' || c == '"' || c == '\'' || c == ' ');
}
