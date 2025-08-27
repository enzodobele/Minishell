/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzimeris <mzimeris@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 19:21:34 by mzimeris          #+#    #+#             */
/*   Updated: 2025/08/27 20:49:07 by mzimeris         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	_is_delimiter_quoted(char *str)
{
	size_t	len;

	if (!str || !str[0])
		return (0);
	len = ft_strlen(str);
	if ((str[0] == '\'' && str[len - 1] == '\'')
		|| (str[0] == '"' && str[len - 1] == '"'))
		return (1);
	return (0);
}

static int	clean_delimiter(char *delimiter, char **clean_delim)
{
	int		is_quoted;

	is_quoted = _is_delimiter_quoted(delimiter);
	if (is_quoted)
		*clean_delim = ft_substr(delimiter, 1, ft_strlen(delimiter) - 2);
	else
		*clean_delim = ft_strdup2(delimiter);
	if (!*clean_delim)
		return (handle_sys_error("Memory allocation failed"), -1);
	return (is_quoted);
}

int	heredoc_handler(t_env *env, char *delimiter)
{
	int		pipe_fd[2];
	char	*clean_delim;
	int		pid;
	int		is_quoted;
	int		status;

	if (!delimiter || !delimiter[0])
		return (-1);
	if (pipe(pipe_fd) == -1)
		return (handle_sys_error("Pipe creation failed"), -1);
	clean_delim = NULL;
	is_quoted = clean_delimiter(delimiter, &clean_delim);
	if (!clean_delim)
		return (close(pipe_fd[0]), close(pipe_fd[1]), -1);
	pid = fork();
	if (pid == -1)
		return (close(pipe_fd[0]), close(pipe_fd[1]), -1);
	if (pid == 0)
		heredoc_child(env, pipe_fd, clean_delim, is_quoted);
	free(clean_delim);
	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		return (close(pipe_fd[0]), -1);
	return (pipe_fd[0]);
}
