/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_pipeline.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoum <zoum@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 13:13:37 by mzimeris          #+#    #+#             */
/*   Updated: 2025/08/22 13:25:01 by zoum             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	_handle_out_redir(t_redirect *redir)
{
	int	fd;

	 //  printf("[_handle_out_redir] Tente d'ouvrir le fichier de sortie: %s\n", redir->filename);
	fd = -1;
	if (redir->type == REDIR_OUT)
	{
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		 //  printf("[_handle_out_redir] Type: REDIR_OUT (>). FD obtenu: %d\n", fd);
	}
	else if (redir->type == REDIR_APPEND)
	{
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
		 //  printf("[_handle_out_redir] Type: REDIR_APPEND (>>). FD obtenu: %d\n", fd);
	}
	return (fd);
}

static int	_get_last_output_fd(t_command *cmd, t_pipe_data *pipe_data)
{
	t_redirect *redir;
	int fd;
	int prev_fd;

	 //  printf("[_get_last_output_fd] Recherche de la dernière redirection de sortie.\n");
	fd = -1;
	prev_fd = -1;
	pipe_data->outfile_error = 0;
	redir = cmd->redirects;
	while (redir)
	{
		if (redir->type == REDIR_OUT || redir->type == REDIR_APPEND)
		{
			if (prev_fd != -1)
			{
				 //  printf("[_get_last_output_fd] Fermeture du FD de la redirection de sortie précédente: %d\n", prev_fd);
				close(prev_fd);
			}
			fd = _handle_out_redir(redir);
			if (fd < 0)
			{
				 //  printf("[_get_last_output_fd] Erreur lors de la redirection de sortie. Fichier: %s\n", redir->filename);
				perror(redir->filename);
				pipe_data->outfile_error = 1;
				return (fd);
			}
			prev_fd = fd;
		}
		redir = redir->next;
	}
	if (prev_fd != -1)
	{
		 //  printf("[_get_last_output_fd] Le dernier FD de sortie est %d.\n", prev_fd);
		return (prev_fd);
	}
	 //  printf("[_get_last_output_fd] Aucune redirection de sortie trouvée.\n");
	return (-1);
}

static int _get_last_input_fd(t_command *cmd)
{
	t_redirect *redir;
	int fd;

	 //  printf("[_get_last_input_fd] Recherche de la dernière redirection d'entrée.\n");
	fd = -1;
	redir = cmd->redirects;
	while (redir)
	{
		if (redir->type == REDIR_IN)
		{
			if (fd > 0)
			{
				 //  printf("[_get_last_input_fd] Fermeture du FD de la redirection d'entrée précédente: %d\n", fd);
				close(fd);
			}
			fd = open(redir->filename, O_RDONLY);
				 //  printf("[_get_last_input_fd] Type: REDIR_IN (<). FD obtenu: %d\n", fd);
			if (fd < 0)
			{
				 //  printf("[_get_last_input_fd] Erreur lors de la redirection d'entrée. Fichier: %s\n", redir->filename);
				perror(redir->filename);
				return (-1);
			}
		}
		redir = redir->next;
	}
	return (fd);
}

static void	_pipexecution_clean_fds(t_pipe_data *pipe_data)
{
	 //  printf("[_pipexecution_clean_fds] Nettoyage des FDs pour la prochaine exécution.\n");
	 //  printf("[_pipexecution_clean_fds] État initial: redir_in_fd=%d, in_fd=%d, outfile_error=%d\n", pipe_data->redir_in_fd, pipe_data->in_fd, pipe_data->outfile_error);
	if (pipe_data->outfile_error)
	{
		 //  printf("[_pipexecution_clean_fds] Erreur de fichier de sortie détectée. Fermeture des FDs d'entrée.\n");
		if (pipe_data->redir_in_fd > 0)
		{
			 //  printf("[_pipexecution_clean_fds] Fermeture de redir_in_fd (%d).\n", pipe_data->redir_in_fd);
			close(pipe_data->redir_in_fd);
		}
		if (pipe_data->in_fd > 0)
		{
			 //  printf("[_pipexecution_clean_fds] Fermeture de in_fd (%d).\n", pipe_data->in_fd);
			close(pipe_data->in_fd);
		}
		pipe_data->in_fd = -1;
		 //  printf("[_pipexecution_clean_fds] in_fd mis à -1.\n");
		return;
	}
	if (pipe_data->redir_in_fd != -1)
	{
		 //  printf("[_pipexecution_clean_fds] Redirection d'entrée trouvée. Mise à jour de in_fd.\n");
		if (pipe_data->in_fd > 0)
		{
			 //  printf("[_pipexecution_clean_fds] Fermeture de l'ancien in_fd du pipe: %d.\n", pipe_data->in_fd);
			close(pipe_data->in_fd);
		}
		pipe_data->in_fd = pipe_data->redir_in_fd;
		 //  printf("[_pipexecution_clean_fds] in_fd mis à %d (redir_in_fd).\n", pipe_data->in_fd);
	}
	else if (pipe_data->in_fd == -1)
	{
		 //  printf("[_pipexecution_clean_fds] Aucune redirection d'entrée. in_fd reste l'entrée du pipe précédent ou STDIN.\n");
			pipe_data->in_fd = STDIN_FILENO;
	}
	 //  printf("[_pipexecution_clean_fds] État final: redir_in_fd=%d, in_fd=%d\n", pipe_data->redir_in_fd, pipe_data->in_fd);
}

int pipexecution(t_env *env, t_command *cmd)
{
	t_pipe_data *pipe_data;

	 //  printf("[pipexecution] Entrée dans la fonction. Démarrage de l'exécution des commandes en chaîne.\n");
	pipe_data = malloc(sizeof(t_pipe_data));
	if (!pipe_data)
	{
		 //  printf("[pipexecution] Échec de l'allocation mémoire pour pipe_data.\n");
		return (-1);
	}
	*pipe_data = (t_pipe_data){0};
	while (cmd)
	{
		pipe_data->redir_in_fd = -1;
        pipe_data->out_fd = -1;
		 //  printf("[pipexecution] Traitement de la commande: %s\n", cmd->cmd->string);
		if (cmd && cmd->redirects && cmd->redirects->type && cmd->redirects->type == REDIR_IN)
		{
			 //  printf("[pipexecution] Des redirections sont présentes.\n");
			pipe_data->redir_in_fd = _get_last_input_fd(cmd);
			if (pipe_data->redir_in_fd < 0)
			{
				 //  printf("[pipexecution] Échec de la redirection d'entrée. Sortie.\n");
				return (free(pipe_data), -1);
			}
		}
		if (cmd && cmd->redirects && cmd->redirects->type && (cmd->redirects->type == REDIR_OUT || cmd->redirects->type == REDIR_APPEND))
		{
			pipe_data->out_fd = _get_last_output_fd(cmd, pipe_data);
			if (pipe_data->outfile_error)
			{
				 //  printf("[pipexecution] Échec de la redirection de sortie. Sortie.\n");
				return (free(pipe_data), -1);
			}
		}
		_pipexecution_clean_fds(pipe_data);
		 //  printf("[pipexecution] Appel de fork_and_exec pour la commande.\n");
		pipe_data->in_fd = fork_and_exec(env, cmd, pipe_data);
		if (pipe_data->in_fd < 0 || pipe_data->outfile_error)
		{
			 //  printf("[pipexecution] Erreur dans fork_and_exec ou outfile_error est vrai. Sortie.\n");
			free(pipe_data);
			return (wait_for_children(), -1);
		}
		cmd = cmd->next;
			 //  printf("[pipexecution] Passe à la commande suivante dans le pipe.\n");
	}
	 //  printf("[pipexecution] Toutes les commandes ont été lancées. Attente des processus enfants.\n");
	free(pipe_data);
	return (wait_for_children());
}
