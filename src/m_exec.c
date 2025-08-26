/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_exec.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzimeris <mzimeris@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 13:13:37 by mzimeris          #+#    #+#             */
/*   Updated: 2025/08/26 21:35:29 by mzimeris         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_builtins(t_command *command, t_env *env)
{
	if (!command || !command->cmd)
		return (0);
	if (ft_strcmp(command->cmd->string, "cd") == 0)
		return (handle_cd(env, command));
	if (ft_strcmp(command->cmd->string, "pwd") == 0)
		return (handle_pwd());
	if (ft_strcmp(command->cmd->string, "echo") == 0
		&& command->args && command->args[0]
		&& ft_strcmp(command->args[0]->string, "-n") == 0)
		return (handle_echo_n(command));
	if (ft_strcmp(command->cmd->string, "export") == 0)
		return (handle_export(env, command));
	if (ft_strcmp(command->cmd->string, "unset") == 0)
		return (handle_unset(env, command));
	if (ft_strcmp(command->cmd->string, "env") == 0)
		return (handle_env(env));
	if (ft_strcmp(command->cmd->string, "exit") == 0)
		return (handle_exit(command, env));
	return (-1);
}

char	**build_argv(t_command *command)
{
	char	**argv;
	int		i;
	int		arg_count;

	if (!command)
		return (NULL);
	arg_count = 0;
	while (command && command->args && command->args[arg_count])
		arg_count++;
	argv = malloc(sizeof(char *) * (arg_count + 2));
	if (!argv)
		return (NULL);
	argv[0] = command->cmd->string;
	i = 0;
	while (i < arg_count)
	{
		argv[i + 1] = command->args[i]->string;
		i++;
	}
	argv[arg_count + 1] = NULL;
	i = 0;
	while (i < arg_count + 1)
		i++;
	return (argv);
}

int	exec_command(t_command *command, t_env *env)
{
	int		builtin_result;
	char	**argv;

	if (!command || !env)
		return (1);
	builtin_result = exec_builtins(command, env);
	if (builtin_result >= 0)
		return (builtin_result);
	else
	{
		if (check_command(env, command) < 0)
			return (handle_command_error(command, 1), 1);
		argv = build_argv(command);
		if (!argv)
			exit(127);
		execve(command->cmd->string, argv, env->envp);
		free(argv);
		perror("execve");
		return (127);
	}
}

static void	_clean_child_fds(t_pipe_data *pipe_data)
{
	if (pipe_data->in_fd > 0)
	{
		if (dup2(pipe_data->in_fd, STDIN_FILENO) == -1)
			perror("dup2 STDIN");
		if (close(pipe_data->in_fd) == -1)
			perror("close in_fd");
	}
	if (pipe_data->is_piped)
	{
		if (close(pipe_data->pipe_fd[0]) == -1)
			perror("close pipe_fd[0]");
		if (dup2(pipe_data->pipe_fd[1], STDOUT_FILENO) == -1)
			perror("dup2 STDOUT");
		if (close(pipe_data->pipe_fd[1]) == -1)
			perror("close pipe_fd[1]");
	}
	else if (pipe_data->out_fd > 1)
	{
		if (dup2(pipe_data->out_fd, STDOUT_FILENO) == -1)
			perror("dup2 STDOUT");
		if (close(pipe_data->out_fd) == -1)
			perror("close out_fd");
	}
}

int	exec_child(t_env *env, t_command *command, t_pipe_data *pipe_data)
{
	if (!command || !command->cmd || !command->cmd->string)
		exit(127);
	_clean_child_fds(pipe_data);
	expand_last_exit_status(env, command);
	exit(exec_command(command, env));
}
