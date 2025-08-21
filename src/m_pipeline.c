/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_pipeline.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzimeris <mzimeris@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 13:13:37 by mzimeris          #+#    #+#             */
/*   Updated: 2025/08/21 13:24:42 by mzimeris         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	_handle_out_redir(t_redirect *redir)
{
	int	fd;

	fd = -1;
	if (redir->type == REDIR_OUT)
	{
		if (fd > 1)
			close(fd);
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	}
	else if (redir->type == REDIR_APPEND)
	{
		if (fd > 1)
			close(fd);
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	}
	return (fd);
}

static int	_get_last_output_fd(t_command *cmd, t_pipe_data *pipe_data)
{
	t_redirect	*redir;
	int			fd;

	pipe_data->outfile_error = 0;
	redir = cmd->redirects;
	while (redir)
	{
		fd = _handle_out_redir(redir);
		if ((redir->type == REDIR_OUT || redir->type == REDIR_APPEND) && fd < 0)
		{
			perror(redir->filename);
			pipe_data->outfile_error = 1;
		}
		redir = redir->next;
	}
	return (fd);
}

static int	_get_last_input_fd(t_command *cmd)
{
	t_redirect	*redir;
	int			fd;

	fd = 0;
	redir = cmd->redirects;
	while (redir)
	{
		if (redir->type == REDIR_IN)
		{
			if (fd > 0)
				close(fd);
			fd = open(redir->filename, O_RDONLY);
			if (fd < 0)
				perror(redir->filename);
		}
		redir = redir->next;
	}
	return (fd);
}

static void	_pipexecution_clean_fds(t_pipe_data *pipe_data)
{
	if (pipe_data->outfile_error)
	{
		if (pipe_data->redir_in_fd > 0)
			close(pipe_data->redir_in_fd);
		if (pipe_data->in_fd > 0)
			close(pipe_data->in_fd);
	}
	if (pipe_data->redir_in_fd > 0)
	{
		if (pipe_data->in_fd > 0)
			close(pipe_data->in_fd);
		pipe_data->in_fd = pipe_data->redir_in_fd;
	}
}

int	pipexecution(t_env *env, t_command *cmd)
{
	t_pipe_data	*pipe_data;

	pipe_data = malloc(sizeof(t_pipe_data));
	if (!pipe_data)
		return (-1);
	*pipe_data = (t_pipe_data){0};
	pipe_data->in_fd = 0;
	while (cmd)
	{
		pipe_data->redir_in_fd = _get_last_input_fd(cmd);
		pipe_data->out_fd = _get_last_output_fd(cmd, pipe_data);
		_pipexecution_clean_fds(pipe_data);
		pipe_data->in_fd = fork_and_exec(env, cmd, pipe_data);
		if (pipe_data->in_fd < 0)
			return (-1);
		cmd = cmd->next;
	}
	return (wait_for_children());
}
