/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_fork.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoum <zoum@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 13:12:16 by mzimeris          #+#    #+#             */
/*   Updated: 2025/08/22 13:25:01 by zoum             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	_clean_parent(t_env *env, t_pipe_data *pipe_data, int pid)
{
	 //  printf("[_clean_parent] Nettoyage des FDs dans le processus parent.\n");
	 //  printf("[_clean_parent] PID de l'enfant: %d\n", pid);
	env->last_pid = pid;
	if (pipe_data->in_fd > 0)
	{
		 //  printf("[_clean_parent] Fermeture de in_fd (%d).\n", pipe_data->in_fd);
		if (close(pipe_data->in_fd) == -1)
			perror("close in_fd (parent)");
	}
	if (pipe_data->out_fd > 1)
	{
		 //  printf("[_clean_parent] Fermeture de out_fd (%d).\n", pipe_data->out_fd);
		if (close(pipe_data->out_fd) == -1)
			perror("close out_fd (parent)");
	}
	if (pipe_data->is_piped)
	{
		 //  printf("[_clean_parent] C'est un pipe. Fermeture de pipe_fd[1] (%d) dans le parent.\n", pipe_data->pipe_fd[1]);
		if (close(pipe_data->pipe_fd[1]) == -1)
			perror("close pipe_fd[1] (parent)");
	}
	 //  printf("[_clean_parent] Nettoyage du parent terminé.\n");
}

int	fork_and_exec(t_env *env, t_command *command, t_pipe_data *pipe_data)
{
	int	pid;

	 //  printf("[fork_and_exec] Entrée dans la fonction pour la commande: %s\n", command->cmd->string);
	pipe_data->is_piped = (command->next != NULL);
	 //  printf("[fork_and_exec] La commande est-elle pipée? %s (1 = oui, 0 = non)\n", pipe_data->is_piped ? "Oui" : "Non");
	if (ft_strcmp(command->cmd->string, "exit") == 0 && !pipe_data->is_piped)
		return (exec_builtins(command, env));
	if (pipe_data->is_piped && pipe(pipe_data->pipe_fd) < 0)
	{
		 //  printf("[fork_and_exec] Échec de la création du pipe.\n");
		return (handle_system_error("Pipe creation failed"), -1);
	}
	else if (pipe_data->is_piped)
	{
		 //  printf("[fork_and_exec] Pipe créé. FD de lecture: %d, FD d'écriture: %d\n", pipe_data->pipe_fd[0], pipe_data->pipe_fd[1]);
	}
	 //  printf("[fork_and_exec] Appel de fork().\n");
	pid = fork();
	if (pid < 0)
	{
		 //  printf("[fork_and_exec] Échec du fork.\n");
		return (handle_system_error("Fork failed"), -1);
	}
	if (pid == 0)
	{
		 //  printf("[fork_and_exec] PID == 0: Je suis l'enfant. Appel de exec_child.\n");
		exec_child(env, command, pipe_data);
	}
	else
	{
		 //  printf("[fork_and_exec] PID > 0: Je suis le parent. Mon enfant a le PID %d.\n", pid);
		_clean_parent(env, pipe_data, pid);
		 //  printf("[fork_and_exec] Le parent retourne le FD de lecture du pipe: %d.\n", pipe_data->pipe_fd[0]);
		return (pipe_data->pipe_fd[0]);
	}
	 //  printf("[fork_and_exec] Cette ligne ne devrait pas être atteinte en théorie.\n");
	return (-1);
}

int	wait_for_children(void)
{
	int		status;
	int		last_exit_status;
	int		sig;
	int		waited_pid;

	 //  printf("[wait_for_children] Entrée dans la fonction. Attente des processus enfants.\n");
	last_exit_status = 0;
	waited_pid = 1;
	while (waited_pid > 0)
	{
		waited_pid = wait(&status);
		if (waited_pid < 0)
		{
			 //  printf("[wait_for_children] wait() a échoué ou plus de processus enfants à attendre.\n");
			break;
		}
		 //  printf("[wait_for_children] Processus avec PID %d terminé.\n", waited_pid);
		if (WIFEXITED(status))
		{
			last_exit_status = WEXITSTATUS(status);
			 //  printf("[wait_for_children] Le processus %d s'est terminé normalement avec le statut: %d.\n", waited_pid, last_exit_status);
		}
		else if (WIFSIGNALED(status))
		{
			sig = WTERMSIG(status);
			 //  printf("[wait_for_children] Le processus %d a été terminé par le signal: %d.\n", waited_pid, sig);
			if (sig != SIGPIPE)
			{
				last_exit_status = 128 + sig;
			}
			else
			{
				 //  printf("[wait_for_children] Ignoré: SIGPIPE (Signal 13).\n");
			}
		}
	}
	 //  printf("[wait_for_children] Fin de l'attente. Le dernier statut de sortie est: %d.\n", last_exit_status);
	return (last_exit_status);
}
