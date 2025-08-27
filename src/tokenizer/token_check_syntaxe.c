#include "../includes/minishell.h"

int	is_token_valid(t_token *token)
{
	t_token	*next_token;

	if (!token)
		return (0);
	while (token)
	{
		next_token = token->next;
		if (token->type == PIPE || token->type == LOGICAL_OR)
		{
			if (token->type == LOGICAL_OR)
				return (printf("LOGICAL OR not handled\n"), 0);
			if (!next_token || next_token->type == PIPE)
				return (print_syntax_error_pipe(), 0);
		}
		if (token->type == REDIR_IN)
		{
			if (!next_token || next_token->type == PIPE)
				return (print_syntax_error_pipe(), 0);
		}
		token = token->next;
	}
	return (1);
}

int	check_redirection_error(char *input, int i)
{
	if (input[i] == '\0')
	{
		print_syntax_error_newlines();
		return (1);
	}
	if (input[i] == '|')
	{
		printf("minishell: syntax error near unexpected token `|'\n");
		return (1);
	}
	if (input[i] == '>' || input[i] == '<')
	{
		if (input[i + 1] && input[i + 1] == input[i])
		{
			print_syntax_error_doubles(input[i]);
			return (1);
		}
		else
		{
			print_syntax_error_singles(input[i]);
			return (1);
		}
	}
	return (0);
}

int	validate_redirection_at(char *input, int i)
{
	int		j;
	char	redir_char;

	j = i;
	redir_char = input[i];
	if (input[j + 1] && input[j + 1] == redir_char)
		j++;
	j++;
	while (input[j] && (input[j] == ' ' || input[j] == '\t'))
		j++;
	if (check_redirection_error(input, j))
		return (1);
	return (j);
}

int	is_redirection_syntax_valid(char *input, int i)
{
	char	c;
	char	in_quote;
	int		next;

	in_quote = 0;
	while (input[++i])
	{
		c = input[i];
		if ((c == '\'' || c == '"'))
		{
			if (!in_quote)
				in_quote = c;
			else if (in_quote == c)
				in_quote = 0;
		}
		else if (!in_quote && (c == '>' || c == '<'))
		{
			next = validate_redirection_at(input, i);
			if (next == 1)
				return (1);
			i = next - 1;
		}
	}
	return (0);
}

int	is_redirection_valid(t_token *token)
{
	t_token	*current;

	current = token;
	while (current)
	{
		if (current->type == REDIR_OUT || current->type == REDIR_APPEND
			|| current->type == REDIR_IN || current->type == HEREDOC)
		{
			if (!current->next)
			{
				print_syntax_error_newlines();
				return (0);
			}
			if (current->next->type != WORD)
			{
				printf("minishell: syntax error near unexpected token `%s'\n",
					get_token_symbol(current->next->type));
				return (0);
			}
		}
		current = current->next;
	}
	return (1);
}
