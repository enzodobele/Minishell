/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_exec.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoum <zoum@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 13:13:37 by mzimeris          #+#    #+#             */
/*   Updated: 2025/08/22 01:04:23 by zoum             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_builtins(t_command *command, t_env *env)
{
	// printf("[exec_builtins] Entrée dans la fonction.\n");
	if (!command || !command->cmd)
	{
		// printf("[exec_builtins] Commande ou cmd est NULL, retourne 0.\n");
		return (0);
	}
	// printf("[exec_builtins] Commande à exécuter: %s\n", command->cmd->string);
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
	// printf("[exec_builtins] Commande n'est pas un builtin, retourne 0.\n");
	return (0);
}

char	**build_argv(t_command *command)
{
	char **argv;
	int i;
	int arg_count;

	// printf("[build_argv] Entrée dans la fonction.\n");
	if (!command)
	{
		// printf("[build_argv] Commande est NULL, retourne NULL.\n");
		return (NULL);
	}
	arg_count = 0;
	while (command && command->args && command->args[arg_count])
		arg_count++;
	// printf("[build_argv] Nombre d'arguments trouvés: %d\n", arg_count);
	argv = malloc(sizeof(char *) * (arg_count + 2));
	if (!argv)
	{
		// printf("[build_argv] Échec de l'allocation mémoire pour argv.\n");
		return (NULL);
	}
	argv[0] = command->cmd->string;
	// printf("[build_argv] argv[0] = %s\n", argv[0]);
	i = 0;
	while (i < arg_count)
	{
		argv[i + 1] = command->args[i]->string;
		// printf("[build_argv] argv[%d] = %s\n", i + 1, argv[i + 1]);
		i++;
	}
	argv[arg_count + 1] = NULL;
	// printf("[build_argv] Dernier élément de argv mis à NULL. argv est prêt.\n");
	return (argv);
}

int	exec_command(t_command *command, t_env *env)
{
	int builtin_result;
	char **argv;

	// printf("[exec_command] Entrée dans la fonction pour la commande: %s\n", command->cmd->string);
	if (!command || !env)
	{
		// printf("[exec_command] Commande ou env est NULL, retourne -1.\n");
		return (-1);
	}
	builtin_result = exec_builtins(command, env);
	// printf("[exec_command] Résultat de exec_builtins: %d\n", builtin_result);
	if (builtin_result == 42)
	{
		// printf("[exec_command] C'est un builtin 'exit', retourne 42.\n");
		return (42);
	}
	else if (builtin_result)
	{
		// printf("[exec_command] C'est un builtin, retourne 0 (pas d'erreur).\n");
		return (0);
	}
	else
	{
		// printf("[exec_command] Ce n'est pas un builtin. Préparation pour execve.\n");
		if (check_command(env, command) < 0)
		{
			// printf("[exec_command] Commande introuvable ou problème de permissions, retourne -1.\n");
			return (handle_command_error(command, -1), -1);
		}
		argv = build_argv(command);
		if (!argv)
		{
			// printf("[exec_command] Échec de build_argv, exit(127).\n");
			exit(127);
		}
		// printf("[exec_command] Appel de execve avec la commande: %s\n", command->cmd->string);
		execve(command->cmd->string, argv, env->envp);
		// Cette partie du code n'est exécutée qu'en cas d'échec d'execve.
		free(argv);
		perror("execve");
	}
	// printf("[exec_command] Fin de la fonction.\n");
	return (0);
}

static void	_clean_child_fds(t_pipe_data *pipe_data)
{
	// printf("[_clean_child_fds] Nettoyage des FDs pour l'enfant.\n");
	// printf("[_clean_child_fds] pipe_data->in_fd: %d, pipe_data->out_fd: %d, pipe_data->is_piped: %d\n", pipe_data->in_fd, pipe_data->out_fd, pipe_data->is_piped);
	if (pipe_data->in_fd > 0)
	{
		// printf("[_clean_child_fds] Redirection de STDIN (fd 0) vers in_fd (%d).\n", pipe_data->in_fd);
		if (dup2(pipe_data->in_fd, STDIN_FILENO) == -1)
			perror("dup2 STDIN");
		if (close(pipe_data->in_fd) == -1)
			perror("close in_fd");
		// printf("[_clean_child_fds] in_fd (%d) fermé.\n", pipe_data->in_fd);
	}
	if (pipe_data->is_piped)
	{
		// printf("[_clean_child_fds] C'est un pipe. Fermeture de pipe_fd[0] (%d) et redirection de STDOUT (fd 1) vers pipe_fd[1] (%d).\n", pipe_data->pipe_fd[0], pipe_data->pipe_fd[1]);
		if (close(pipe_data->pipe_fd[0]) == -1)
			perror("close pipe_fd[0]");
		if (dup2(pipe_data->pipe_fd[1], STDOUT_FILENO) == -1)
			perror("dup2 STDOUT");
		if (close(pipe_data->pipe_fd[1]) == -1)
			perror("close pipe_fd[1]");
		// printf("[_clean_child_fds] pipe_fd[1] (%d) fermé.\n", pipe_data->pipe_fd[1]);
	}
	else if (pipe_data->out_fd > 1)
	{
		// printf("[_clean_child_fds] Redirection de STDOUT (fd 1) vers out_fd (%d).\n", pipe_data->out_fd);
		if (dup2(pipe_data->out_fd, STDOUT_FILENO) == -1)
			perror("dup2 STDOUT");
		if (close(pipe_data->out_fd) == -1)
			perror("close out_fd");
		// printf("[_clean_child_fds] out_fd (%d) fermé.\n", pipe_data->out_fd);
	}
	// printf("[_clean_child_fds] Nettoyage des FDs terminé.\n");
}

int	exec_child(t_env *env, t_command *command, t_pipe_data *pipe_data)
{
	int	check_result;

	// printf("[exec_child] Processus enfant lancé pour la commande: %s\n", command->cmd->string);
	if (!command || !command->cmd || !command->cmd->string)
	{
		// printf("[exec_child] Commande ou cmd est NULL, exit(127).\n");
		exit(127);
	}
	_clean_child_fds(pipe_data);
	// printf("[exec_child] FDs nettoyés et redirigés.\n");
	check_result = check_command(env, command);
	if (check_result < 0)
	{
		// printf("[exec_child] Commande introuvable ou permissions refusées, exit(127).\n");
		handle_command_error(command, check_result);
		exit(127);
	}
	// printf("[exec_child] Commande valide. Appel de exec_command.\n");
	exec_command(command, env);
	// printf("[exec_child] Échec d'exécution de la commande (exec_command a échoué), exit(127).\n");
	exit(127);
}
