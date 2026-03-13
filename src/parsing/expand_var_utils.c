/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edobele <edobele@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 16:53:21 by edobele           #+#    #+#             */
/*   Updated: 2025/08/28 16:53:22 by edobele          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

void	ft_itoa_into(char *buf, int n)
{
	long	num;
	int		len;
	int		tmp;

	num = n;
	len = (num <= 0);
	tmp = num;
	while (tmp != 0)
	{
		tmp /= 10;
		len++;
	}
	buf[len] = '\0';
	if (num == 0)
		buf[0] = '0';
	if (num < 0)
	{
		buf[0] = '-';
		num = -num;
	}
	while (num > 0)
	{
		buf[--len] = (num % 10) + '0';
		num /= 10;
	}
}
