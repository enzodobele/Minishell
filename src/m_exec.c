/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_exec.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoum <zoum@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 13:13:37 by mzimeris          #+#    #+#             */
/*   Updated: 2025/08/20 18:26:58 by zoum             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "m_minishell.h"

int	exec_builtins(t_command *command, t_env *env)
{
	if (!command || !command->cmd)
		return (0);
	if (ft_strcmp(command->cmd->string, "cd") == 0)
		return (handle_cd(env, command), 1);
	if (ft_strcmp(command->cmd->string, "pwd") == 0)
		return (handle_pwd(), 1);
	if (ft_strcmp(command->cmd->string, "echo") == 0
		&& command->args && command->args[0]
		&& ft_strcmp(command->args[0]->string, "-n") == 0)
		return (handle_echo_n(command), 1);
	if (ft_strcmp(command->cmd->string, "export") == 0)
		return (handle_export(env, command), 1);
	if (ft_strcmp(command->cmd->string, "unset") == 0)
		return (handle_unset(env, command), 1);
	if (ft_strcmp(command->cmd->string, "env") == 0)
		return (handle_env(env), 1);
	if (ft_strcmp(command->cmd->string, "exit") == 0)
		return (handle_exit(command, env));
	return (0);
}

char	**build_argv(t_command *cmd)
{
	char	**argv;
	int		i;
	int		arg_count;

	if (!cmd)
		return (NULL);
	arg_count = 0;
	while (cmd && cmd->args && cmd->args[arg_count])
		arg_count++;
	argv = malloc(sizeof(char *) * (arg_count + 2));
	if (!argv)
		return (NULL);
	argv[0] = cmd->cmd->string;
	i = 0;
	while (i < arg_count)
	{
		argv[i + 1] = cmd->args[i]->string;
		i++;
	}
	argv[arg_count + 1] = NULL;
	return (argv);
}

int	exec_command(t_command *command, t_env *env)
{
	int		builtin_result;
	char	**argv;

	if (!command || !env)
		return (-1);
	builtin_result = exec_builtins(command, env);
	if (builtin_result == 42)
		return (42);
	else if (builtin_result)
		return (0);
	else
	{
		argv = build_argv(command);
		if (!argv)
			exit(127);
		execve(command->cmd->string, argv, env->envp);
		free(argv);
		perror("execve");
	}
	return (0);
}

int	exec_child(t_env *env, t_command *command, int in_fd, int pipe_fd[2],
		char *outfile)
{
	int		check_result;

	if (!command || !command->cmd || !command->cmd->string)
		exit(127);
	check_result = check_command(env, command);
	if (check_result < 0)
	{
		handle_command_error(command, check_result);
		exit(127);
	}
	setup_input_redirect(in_fd);
	if (setup_output_redirect(command, pipe_fd, outfile))
		exit(1);
	exec_command(command, env);
	exit(127);
}

int	fork_and_exec(t_env *env, t_command *command, int in_fd, char *outfile)
{
	int	pid;
	int	pipe_fd[2];

	if (ft_strcmp(command->cmd->string, "exit") == 0 && !command->pipe_out)
		return (exec_builtins(command, env));
	pipe_fd[0] = -1;
	pipe_fd[1] = -1;
	if (command->next != NULL && pipe(pipe_fd) < 0)
		return (handle_system_error("Pipe creation failed"), -1);
	pid = fork();
	if (pid < 0)
		return (handle_system_error("Fork failed"), -1);
	if (pid == 0)
		return (exec_child(env, command, in_fd, pipe_fd, outfile));
	env->last_pid = pid;
	if (in_fd > 0)
		close(in_fd);
	if (command->next != NULL)
		return (close(pipe_fd[1]), pipe_fd[0]);
	waitpid(pid, NULL, 0);
	return (0);
}

// int	exec(t_command *commands, t_env **env, t_token **token)
// {
// 	(void)token;

// 	if (!commands || !env || !(*env))
// 		return (-1);
// 	if (commands->pipe_out)
// 		return (pipexecution(env, &commands, NULL, NULL));
// 	return (fork_and_exec(env, commands, -1, NULL));
// }
