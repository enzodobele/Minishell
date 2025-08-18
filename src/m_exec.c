/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_exec.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzimeris <mzimeris@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 13:13:37 by mzimeris          #+#    #+#             */
/*   Updated: 2025/08/13 18:15:28 by mzimeris         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "m_minishell.h"

// int	exec_system(t_command *command, t_env **env, t_token **token)
// {
// 	// This function should handle the execution of system commands.
// 	// should probably totally lead to pipex
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
