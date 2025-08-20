/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_pipeline.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoum <zoum@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/08/20 22:03:52 by zoum             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"
#include "m_minishell.h"

int	exec(t_command *cmd, t_env *env, t_token **token)
{
	int	in_fd;

	(void)token;
	in_fd = -1;
	if (!cmd || !env)
		return (-1);
	while (cmd)
	{
		in_fd = fork_and_exec(env, cmd, in_fd, NULL);
		if (in_fd < 0)
			return (-1);
		cmd = cmd->next;
	}
	return (wait_for_children(env->last_pid));
}

static int	setup_input_output(char *outfile, char *infile, int *in_fd,
			int *outfile_error)
{
	int	outfile_test;

	*outfile_error = 0;
	if (outfile && outfile[0] != '\0')
	{
		outfile_test = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (outfile_test < 0)
		{
			perror(outfile);
			*outfile_error = 1;
		}
		else
			close(outfile_test);
	}
}

static int	setup_input_output(char *outfile, char *infile, int *in_fd,
			int *outfile_error)
{
	_setup_output(outfile, outfile_error);
	if (infile && infile[0] != '\0')
	{
		*in_fd = open(infile, O_RDONLY);
		if (*in_fd < 0)
		{
			perror(infile);
			*in_fd = -1;
		}
	}
	else
		*in_fd = 0;
	return (0);
}

static int	execute_pipeline(t_env *env, t_command *cmd, int in_fd, char *outfile)
{
	int	in_fd;
	int	outfile_error;

	setup_input_output(outfile, infile, &in_fd, &outfile_error);
	while (cmd)
	{
		in_fd = fork_and_exec(env, cmd, in_fd, outfile);
		if (in_fd < 0)
			return (-1);
		cmd = cmd->next;
	}
	return (wait_for_children(env->last_pid));
}

int	pipexecution(t_env *env, t_command *cmd, char *infile, char *outfile)
{
	int	in_fd;
	int	outfile_error;

	setup_input_output(outfile, infile, &in_fd, &outfile_error);
	env->last_exit_status = execute_pipeline(env, cmd, in_fd, outfile);
	if (env->last_exit_status < 0)
		return (-1);
	if (outfile_error)
		return (1);
	return (env->last_exit_status);
	return (env->last_exit_status);
}

int	wait_for_children(pid_t last_pid)
{
	int		status;
	int		last_exit_status;
	int		sig;
	pid_t	waited_pid;

	last_exit_status = 0;
	waited_pid = wait(&status);
	while (waited_pid > 0)
	{
		if (waited_pid == last_pid)
		{
			if (WIFEXITED(status))
				last_exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
			{
				sig = WTERMSIG(status);
				if (sig != SIGPIPE)
					last_exit_status = 128 + sig;
			}
		}
		waited_pid = wait(&status);
	}
	return (last_exit_status);
}
