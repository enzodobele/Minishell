/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_parser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzimeris <mzimeris@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 16:45:00 by mzimeris          #+#    #+#             */
/*   Updated: 2025/08/05 16:42:37 by mzimeris         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	count_args(char *cmd)
{
	int	count;
	int	i;
	int	in_quotes;

	count = 0;
	i = 0;
	in_quotes = 0;
	while (cmd[i])
	{
		while (cmd[i] && cmd[i] == ' ')
			i++;
		if (cmd[i])
		{
			count++;
			while (cmd[i] && (cmd[i] != ' ' || in_quotes))
			{
				if (cmd[i] == '\'' && !in_quotes)
					in_quotes = 1;
				else if (cmd[i] == '\'' && in_quotes)
					in_quotes = 0;
				i++;
			}
		}
	}
	return (count);
}

static int	calculate_arg_length(char *cmd, int start)
{
	int	i;
	int	len;
	int	in_quotes;

	i = start;
	len = 0;
	in_quotes = 0;
	while (cmd[i] && (cmd[i] != ' ' || in_quotes))
	{
		if (cmd[i] == '\'' && !in_quotes)
			in_quotes = 1;
		else if (cmd[i] == '\'' && in_quotes)
			in_quotes = 0;
		else
			len++;
		i++;
	}
	return (len);
}

static void	copy_arg_content(char *cmd, int *i, char *result)
{
	int	j;
	int	in_quotes;

	j = 0;
	in_quotes = 0;
	while (cmd[*i] && (cmd[*i] != ' ' || in_quotes))
	{
		if (cmd[*i] == '\'' && !in_quotes)
			in_quotes = 1;
		else if (cmd[*i] == '\'' && in_quotes)
			in_quotes = 0;
		else
			result[j++] = cmd[*i];
		(*i)++;
	}
	result[j] = '\0';
}

static char	*extract_arg(t_lalloc *allocator, char *cmd, int *i)
{
	int		start;
	int		len;
	char	*result;

	while (cmd[*i] && cmd[*i] == ' ')
		(*i)++;
	start = *i;
	len = calculate_arg_length(cmd, start);
	result = ft_my_malloc(allocator, len + 1);
	if (!result)
		return (NULL);
	*i = start;
	copy_arg_content(cmd, i, result);
	return (result);
}

char	**parse_command_with_quotes(t_lalloc *allocator, char *cmd)
{
	char	**args;
	int		arg_count;
	int		i;
	int		j;

	if (!cmd || !*cmd)
		return (NULL);
	arg_count = count_args(cmd);
	args = ft_my_malloc(allocator, sizeof(char *) * (arg_count + 1));
	if (!args)
		return (NULL);
	i = 0;
	j = 0;
	while (j < arg_count)
	{
		args[j] = extract_arg(allocator, cmd, &i);
		if (!args[j])
			return (NULL);
		j++;
	}
	args[j] = NULL;
	return (args);
}
