/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_pipeline.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzimeris <mzimeris@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 13:13:37 by mzimeris          #+#    #+#             */
/*   Updated: 2025/08/25 15:46:54 by mzimeris         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	_handle_out_redir(t_redirect *redir)
{
	int	fd;

	fd = -1;
	if (redir->type == REDIR_OUT)
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->type == REDIR_APPEND)
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	return (fd);
}

static int	_get_last_output_fd(t_command *cmd, t_pipe_data *pipe_data)
{
	t_redirect	*redir;
	int			fd;
	int			prev_fd;

	fd = -1;
	prev_fd = -1;
	pipe_data->outfile_error = 0;
	redir = cmd->redirects;
	while (redir)
	{
		if (redir->type == REDIR_OUT || redir->type == REDIR_APPEND)
		{
			if (prev_fd != -1)
				close(prev_fd);
			fd = _handle_out_redir(redir);
			if (fd < 0)
			{
				perror(redir->filename);
				pipe_data->outfile_error = 1;
				return (fd);
			}
			prev_fd = fd;
		}
		redir = redir->next;
	}
	if (prev_fd != -1)
		return (prev_fd);
	return (-1);
}

static int	_input_fd(t_redirect *redir, t_pipe_data *pipe_data)
{
	int			fd;

	fd = -1;
	if (redir->type == REDIR_IN)
	{
		if (pipe_data->in_fd > 0)
			close(pipe_data->in_fd);
		fd = open(redir->filename, O_RDONLY);
		if (fd < 0)
			return (perror(redir->filename), -1);
	}
	pipe_data->redir_in_fd = fd;
}

static void	_pipexecution_clean_fds(t_pipe_data *pipe_data)
{
	if (pipe_data->outfile_error)
	{
		if (pipe_data->redir_in_fd > 0)
			close(pipe_data->redir_in_fd);
		if (pipe_data->in_fd > 0)
			close(pipe_data->in_fd);
		pipe_data->in_fd = -1;
		return ;
	}
	if (pipe_data->redir_in_fd != -1)
	{
		if (pipe_data->in_fd > 0)
			close(pipe_data->in_fd);
		pipe_data->in_fd = pipe_data->redir_in_fd;
	}
	else if (pipe_data->in_fd == -1)
		pipe_data->in_fd = STDIN_FILENO;
}

static int	setup_redirections(t_redirect *redirect, t_pipe_data *pipe_data)
{
	pipe_data->redir_in_fd = -1;
	pipe_data->out_fd = -1;
	while (redirect && redirect->type)
	{
		if (redirect->type == REDIR_IN)
		{
			_input_fd(redirect, pipe_data);
			if (pipe_data->redir_in_fd < 0)
				return (-1);
		}
		if (redirect->type == REDIR_OUT || redirect->type == REDIR_APPEND)
		{
			pipe_data->out_fd = _get_last_output_fd(redirect, pipe_data);
			if (pipe_data->outfile_error)
				return (-1);
		}
		redirect = redirect->next;
	}
	return (0);
}

int	pipexecution(t_env *env, t_command *cmd)
{
	t_pipe_data	*pipe_data;

	pipe_data = malloc(sizeof(t_pipe_data));
	if (!pipe_data)
		return (-1);
	*pipe_data = (t_pipe_data){0};
	while (cmd)
	{
		if (setup_redirections(cmd, pipe_data) < 0)
		{
			free(pipe_data);
			return (-1);
		}
		_pipexecution_clean_fds(pipe_data);
		pipe_data->in_fd = fork_and_exec(env, cmd, pipe_data);
		if (pipe_data->in_fd < 0 || pipe_data->outfile_error)
		{
			free(pipe_data);
			return (env->last_exit_status = wait_for_children(), -1);
		}
		cmd = cmd->next;
	}
	free(pipe_data);
	return (env->last_exit_status = wait_for_children());
}
