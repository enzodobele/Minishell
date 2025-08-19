/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_exec.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzimeris <mzimeris@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 13:13:37 by mzimeris          #+#    #+#             */
/*   Updated: 2025/08/19 13:04:08 by mzimeris         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "m_minishell.h"

// il faut que je voie comment je sais si je suis a la dernière commande
// int	exec_child(t_command *cmd, int in_fd, int pipe_fd[2])
// {
// 	int	check_result;

// 	setup_input_redirect(in_fd);
// 	if (setup_output_redirect(cmd, pipe_fd, cmd->outfile) < 0)
// 		check_result = check_command(pipex, pipex->cmds[i][0], i);
// 	handle_command_error(pipex, check_result, i);
// 	execve(pipex->cmds[i][0], pipex->cmds[i], pipex->envp);
// 	perror("execve");
// 	free_pipex(pipex);
// 	exit(127);
// }

// int	wait_for_children(pid_t last_pid)
// {
// 	int		status;
// 	int		last_exit_status;
// 	int		sig;
// 	pid_t	waited_pid;

// 	last_exit_status = 0;
// 	waited_pid = wait(&status);
// 	while (waited_pid > 0)
// 	{
// 		if (waited_pid == last_pid)
// 		{
// 			if (WIFEXITED(status))
// 				last_exit_status = WEXITSTATUS(status);
// 			else if (WIFSIGNALED(status))
// 			{
// 				sig = WTERMSIG(status);
// 				if (sig != SIGPIPE)
// 					last_exit_status = 128 + sig;
// 			}
// 		}
// 		waited_pid = wait(&status);
// 	}
// 	return (last_exit_status);
// }

// int	exec_system(t_command *cmd, t_env **env, t_token **token, int in_fd)
// {
// 	int	pid;
// 	int	pipe_fd[2];

// 	in_fd = 1;

// 	if (check_command(*env, cmd) < 0)
// 		return (printf("Command '%s' not found\n", cmd->cmd->string), 127);
// 	pipe_fd[0] = -1;
// 	pipe_fd[1] = -1;
// 	if (cmd->pipe_out && pipe(pipe_fd) < 0)
// 		return (-1);
// 	pid = fork();
// 	if (pid < 0)
// 		return (-1); // Fork failed
// 	if (pid == 0)
// 		return (execve(cmd->cmd->string, cmd->args, *env));
// 		// return (exec_child(cmd, in_fd, pipe_fd));
// 	// pipex->last_pid = pid;
// 	if (in_fd > 0)
// 		close(in_fd);
// 	if (cmd->next->cmd != NULL)
// 		return (close(pipe_fd[1]), pipe_fd[0]);
// 	return (0);
// }

int	exec(t_command *commands, t_env **env, t_token **token)
{
	t_command	*current;

	if (!commands)
		return (0);
	current = commands;
	while (current)
	{
		if (!current->cmd)
		{
			printf("Error: command is NULL (should not happen)\n");
			current = current->next;
			continue ;
		}
		exec_builtins(current, env, token);
		current = current->next;
	}
	return (0);
}
