#include "minishell.h"

void	process_args(t_token **args, t_env **env)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		process_token(args[i], env);
		i++;
	}
}

void	expand_structure_env_vars(t_command *commands, t_env **env)
{
	t_command	*current;

	current = commands;
	while (current)
	{
		process_token(current->cmd, env);
		process_args(current->args, env);
		process_redirects(current->redirects, env);
		current = current->next;
	}
}

void	expand_any_string_with_quotes(char **str, t_env **env,
		t_quote_type quote_type)
{
	if (quote_type != SINGLE_QUOTE)
		process_string(str, env);
}

int	contains_env_vars(const char *str)
{
	if (str && ft_strchr(str, '$') != NULL)
		return (1);
	return (0);
}
