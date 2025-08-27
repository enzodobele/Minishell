/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_echo.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzimeris <mzimeris@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 15:07:19 by mzimeris          #+#    #+#             */
/*   Updated: 2025/08/27 16:45:34 by mzimeris         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_echo_args(t_command *command)
{
	int	i;
	int	count;

	if (!command || !command->args)
		return (0);
	i = 0;
	count = 0;
	while (command->args[i])
	{
		count++;
		while (command->args[i]->have_space == 0 && command->args[i + 1])
			i++;
		i++;
	}
	if (count == i)
		return (0);
	return (count);
}

t_token	*join_two_tokens(t_token *first, t_token *second)
{
	char	*new_string;
	t_token	*new_token;

	if (!first || !second)
		return (NULL);
	new_string = ft_strjoin(first->string, second->string);
	if (!new_string)
		return (NULL);
	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (free(new_string), NULL);
	new_token->string = new_string;
	new_token->type = WORD;
	if (first->quote_type == DOUBLE_QUOTE || second->quote_type == DOUBLE_QUOTE)
		new_token->quote_type = DOUBLE_QUOTE;
	else if (first->quote_type == SINGLE_QUOTE
		|| second->quote_type == SINGLE_QUOTE)
		new_token->quote_type = SINGLE_QUOTE;
	else
		new_token->quote_type = NO_QUOTE;
	new_token->next = NULL;
	new_token->prev = NULL;
	new_token->have_space = second->have_space;
	return (new_token);
}

t_token	*duplicate_token(t_token *token)
{
	t_token	*new_token;

	if (!token)
		return (NULL);
	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->string = ft_strdup(token->string, ft_strlen(token->string), 0);
	if (!new_token->string)
		return (free(new_token), NULL);
	new_token->type = token->type;
	new_token->quote_type = token->quote_type;
	new_token->next = NULL;
	new_token->prev = NULL;
	new_token->have_space = token->have_space;
	return (new_token);
}

t_token	**create_new_args_array(t_command *command, t_token **new_args)
{
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (command->args[i])
	{
		if (command->args[i]->have_space == 0 && command->args[i + 1])
		{
			new_args[j] = join_two_tokens(command->args[i],
					command->args[i + 1]);
			if (!new_args[j])
				return (free(new_args), NULL);
			i += 2;
		}
		else
		{
			new_args[j] = duplicate_token(command->args[i]);
			if (!new_args[j])
				return (free(new_args), NULL);
			i++;
		}
		j++;
	}
	return (new_args);
}

void	clean_echo_args(t_command *command)
{
	int		count;
	t_token	**new_args;


	if (!command || !command->args)
		return ;
	count = count_echo_args(command);
	if (count == 0)
		return ;
	new_args = malloc(sizeof(t_token *) * (count + 1));
	if (!new_args)
		return ;
	new_args[count] = NULL;
	new_args = create_new_args_array(command, new_args);
	if (!new_args)
		return ;
	free(command->args);
	command->args = new_args;
	return ;
}
