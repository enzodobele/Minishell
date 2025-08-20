/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_pipeline.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoum <zoum@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/08/21 01:49:50 by zoum             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "m_minishell.h"



int fork_and_exec(t_env *env, t_command *command, int in_fd, int out_fd)
{
	int	pid;
	int	pipe_fd[2];
	int	is_piped;


	is_piped = (command->next != NULL);
	if (ft_strcmp(command->cmd->string, "exit") == 0 && !is_piped)
		return (exec_builtins(command, env));
	if (is_piped && pipe(pipe_fd) < 0)
		return (handle_system_error("Pipe creation failed"), -1);
	pid = fork();
	if (pid < 0)
		return (handle_system_error("Fork failed"), -1);
	if (pid == 0)
		exec_child(env, command, in_fd, out_fd, pipe_fd, is_piped);
	else
	{
		env->last_pid = pid;
		if (in_fd > 0)
			close(in_fd);
		if (out_fd > 1)
			close(out_fd);
		if (is_piped)
		{
			close(pipe_fd[1]);
			return (pipe_fd[0]);
		}
	}
	return (0);
}

static int	_get_last_output_fd(t_command *cmd, int *outfile_error)
{
	t_redirect	*redir;
	int			fd;

	fd = 1;
	*outfile_error = 0;
	redir = cmd->redirects;
	while (redir)
	{
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
		if ((redir->type == REDIR_OUT || redir->type == REDIR_APPEND) && fd < 0)
		{
			perror(redir->filename);
			*outfile_error = 1;
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

int	pipexecution(t_env *env, t_command *cmd)
{
	int	in_fd;
	int	out_fd;
	int	outfile_error;
	int	redir_in_fd;

	in_fd = 0;
	while (cmd)
	{
		redir_in_fd = _get_last_input_fd(cmd);
		out_fd = _get_last_output_fd(cmd, &outfile_error);
		if (outfile_error)
		{
			if (redir_in_fd > 0)
				close(redir_in_fd);
			if (in_fd > 0)
				close(in_fd);
			return (1);
		}
		if (redir_in_fd > 0)
		{
			if (in_fd > 0)
				close(in_fd);
			in_fd = redir_in_fd;
		}
		in_fd = fork_and_exec(env, cmd, in_fd, out_fd);
		if (in_fd < 0)
			return (-1);

		cmd = cmd->next;
	}
	return (wait_for_children());
}

int	wait_for_children(void)
{
	int		status;
	int		last_exit_status;
	int		sig;
	pid_t	waited_pid;

	last_exit_status = 0;
	waited_pid = 1;
	while (waited_pid > 0)
	{
		if (WIFEXITED(status))
			last_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			sig = WTERMSIG(status);
			if (sig != SIGPIPE)
				last_exit_status = 128 + sig;
		}
		waited_pid = wait(&status);
	}
	return (last_exit_status);
}

