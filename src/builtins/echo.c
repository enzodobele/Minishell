/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzimeris <mzimeris@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 15:07:19 by mzimeris          #+#    #+#             */
/*   Updated: 2025/08/28 15:13:12 by mzimeris         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_echo_args(t_command *command)
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

static t_token	*_join_loop(t_token **new_args, t_token **args, int *i, int *j)
{
	t_token	*tmp;

	tmp = join_two_tokens(new_args[*j], args[*i]);
	if (!tmp)
		return (ft_tokenlstclear(new_args), NULL);
	free(new_args[*j]->string);
	free(new_args[*j]);
	new_args[*j] = tmp;
	(*i)++;
	return (new_args[*j]);
}

static t_token	**create_new_args_array(t_token **args, t_token **new_args,
					int i, int j)
{
	while (args[i])
	{
		if (args[i]->have_space == 0 && args[i + 1])
		{
			new_args[j] = join_two_tokens(args[i], args[i + 1]);
			if (!new_args[j])
				return (ft_tokenlstclear(new_args), NULL);
			i += 2;
			while (args[i] && new_args[j]->have_space == 0)
				_join_loop(new_args, args, &i, &j);
		}
		else
		{
			new_args[j] = duplicate_token(args[i]);
			if (!new_args[j])
				return (ft_tokenlstclear(new_args), NULL);
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
	new_args = create_new_args_array(command->args, new_args, 0, 0);
	if (!new_args)
		return ;
	ft_tokenlstclear(command->args);
	command->args = new_args;
	return ;
}
