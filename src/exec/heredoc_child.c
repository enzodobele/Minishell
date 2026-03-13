/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_child.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzimeris <mzimeris@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 20:42:55 by mzimeris          #+#    #+#             */
/*   Updated: 2025/08/27 20:52:16 by mzimeris         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	_has_variable(char *str)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
			return (1);
		i++;
	}
	return (0);
}

static void	_expand_heredoc(t_env *env, char *str, int *pipe_fd)
{
	int			i;
	int			start;
	char		*key;
	t_env_node	*node;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			start = ++i;
			while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
				i++;
			key = ft_substr(str, start, i - start);
			if (!key)
				return (handle_sys_error("Memory allocation failed"), (void)0);
			node = get_env(env, key);
			if (node && node->value)
				write(pipe_fd[1], node->value, ft_strlen(node->value));
			free(key);
		}
		else
			write(pipe_fd[1], &str[i++], 1);
	}
	write(pipe_fd[1], "\n", 1);
}

static int	_heredoc_loop(t_env *env, int pipe_fd[2], char *delimiter,
				int is_quoted)
{
	char	*str;
	int		heredoc_fd;

	while (1)
	{
		str = readline("> ");
		if (!str || ft_strcmp(str, delimiter) == 0)
		{
			free(str);
			close(pipe_fd[1]);
			heredoc_fd = pipe_fd[0];
			return (heredoc_fd);
		}
		if (!is_quoted && _has_variable(str))
			_expand_heredoc(env, str, pipe_fd);
		else
		{
			write(pipe_fd[1], str, ft_strlen(str));
			write(pipe_fd[1], "\n", 1);
		}
		free(str);
	}
	return (-1);
}

int	heredoc_child(t_env *env, int pipe_fd[2],
			char *clean_delim, int is_quoted)
{
	signal(SIGINT, SIG_DFL);
	close(pipe_fd[0]);
	_heredoc_loop(env, pipe_fd, clean_delim, is_quoted);
	free(clean_delim);
	exit(0);
}
