/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoum <zoum@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 14:12:34 by mzimeris          #+#    #+#             */
/*   Updated: 2025/08/07 13:05:22 by zoum             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	exec_child(t_pipex *pipex, int i, int in_fd, int pipe_fd[2])
{
	setup_input_redirect(in_fd);
	if (setup_output_redirect(pipex, i, pipe_fd) < 0)
	{
		free_pipex(pipex);
		exit(1);
	}
	if (check_command(pipex, pipex->cmds[i][0], i) < 0)
	{
		ft_putstr_fd("command not found: ", 2);
		ft_putendl_fd(pipex->cmds[i][0], 2);
		free_pipex(pipex);
		exit(127);
	}
	execve(pipex->cmds[i][0], pipex->cmds[i], pipex->envp);
	perror("execve");
	free_pipex(pipex);
	exit(127);
}

int	exec(t_pipex *pipex, int i, int in_fd)
{
	int	pid;
	int	pipe_fd[2];

	pipe_fd[0] = -1;
	pipe_fd[1] = -1;
	if (pipex->cmds[i + 1] != NULL && pipe(pipe_fd) < 0)
		return ((ft_putstr_fd("Error: Pipe creation failed\n", 2)), -1);
	pid = fork();
	if (pid < 0)
		return (ft_putstr_fd("Error: Fork failed\n", 2), -1);
	if (pid == 0)
		return (exec_child(pipex, i, in_fd, pipe_fd));
	if (in_fd > 0)
		close(in_fd);
	if (pipex->cmds[i + 1] != NULL)
		return (close(pipe_fd[1]), pipe_fd[0]);
	return (0);
}

static int	wait_for_children(void)
{
	int	status;
	int	exit_status;
	int	last_exit_status;

	exit_status = 0;
	last_exit_status = 0;
	while (wait(&status) > 0)
	{
		if (WIFEXITED(status))
		{
			last_exit_status = WEXITSTATUS(status);
			if (last_exit_status != 0)
				exit_status = last_exit_status;
		}
	}
	if (exit_status == 0)
		exit_status = last_exit_status;
	return (exit_status);
}

int	pipex(t_pipex *pipex)
{
	int	i;
	int	in_fd;
	int	last_exit_status;
	int	has_errors;

	i = 0;
	in_fd = pipex->infile_fd;
	has_errors = 0;
	if (pipex->outfile_fd < 0)
		has_errors = 1;
	while (pipex->cmds[i])
	{
		in_fd = exec(pipex, i, in_fd);
		if (in_fd < 0)
			return (-1);
		i++;
	}
	last_exit_status = wait_for_children();
	if (has_errors)
		return (1);
	return (last_exit_status);
}
