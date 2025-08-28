/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entry_point.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzimeris <mzimeris@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 19:30:32 by mzimeris          #+#    #+#             */
/*   Updated: 2025/08/28 19:30:43 by mzimeris         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pipexecution(t_env *env, t_command *cmd)
{
	t_pipe_data	*pipe_data;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	pipe_data = malloc(sizeof(t_pipe_data));
	if (!pipe_data)
		return (-1);
	*pipe_data = (t_pipe_data){0};
	while (cmd)
	{
		if (pipexecution_loop(env, cmd, pipe_data) < 0)
			return (-1);
		cmd = cmd->next;
	}
	free(pipe_data);
	wait_for_children();
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	return (env->last_exit_status);
}
