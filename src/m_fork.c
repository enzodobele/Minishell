/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_fork.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzimeris <mzimeris@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 13:12:16 by mzimeris          #+#    #+#             */
/*   Updated: 2025/08/26 17:07:52 by mzimeris         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	_clean_parent(t_env *env, t_pipe_data *pipe_data, int pid)
{
	env->last_pid = pid;
	if (pipe_data->in_fd > 0 && close(pipe_data->in_fd) == -1)
		perror("close in_fd (parent)");
	if (pipe_data->out_fd > 1 && close(pipe_data->out_fd) == -1)
		perror("close out_fd (parent)");
	if (pipe_data->is_piped && close(pipe_data->pipe_fd[1]) == -1)
		perror("close pipe_fd[1] (parent)");
}

int	fork_and_exec(t_env *env, t_command *command, t_pipe_data *pipe_data)
{
	int	pid;
	int	saved_stdout;
	int	ret;

	pipe_data->is_piped = (command->next != NULL);
	if ((ft_strcmp(command->cmd->string, "exit") == 0 && !pipe_data->is_piped)
		|| ft_strcmp(command->cmd->string, "cd") == 0
		|| (ft_strcmp(command->cmd->string, "export") == 0)
		|| ft_strcmp(command->cmd->string, "unset") == 0)
	{
		saved_stdout = -1;
		if (pipe_data->out_fd != -1 && pipe_data->out_fd != STDOUT_FILENO)
		{
			saved_stdout = dup(STDOUT_FILENO);
			dup2(pipe_data->out_fd, STDOUT_FILENO);
			close(pipe_data->out_fd);
		}
		ret = exec_builtins(command, env);
		if (saved_stdout != -1)
		{
			dup2(saved_stdout, STDOUT_FILENO);
			close(saved_stdout);
		}
		env->last_exit_status = ret;
		return (ret);
	}
	if (pipe_data->is_piped && pipe(pipe_data->pipe_fd) < 0)
		return (handle_system_error("Pipe creation failed"), -1);
	pid = fork();
	if (pid < 0)
		return (handle_system_error("Fork failed"), -1);
	if (pid == 0)
		env->last_exit_status = exec_child(env, command, pipe_data);
	else
		return (_clean_parent(env, pipe_data, pid), pipe_data->pipe_fd[0]);
	return (-1);
}

int	wait_for_children(void)
{
	int		status;
	int		last_exit_status;
	int		sig;
	int		waited_pid;

	last_exit_status = 0;
	waited_pid = 1;
	while (waited_pid > 0)
	{
		waited_pid = wait(&status);
		if (waited_pid < 0)
			break ;
		if (WIFEXITED(status))
			last_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			sig = WTERMSIG(status);
			if (sig != SIGPIPE)
				last_exit_status = 128 + sig;
		}
	}
	return (last_exit_status);
}
