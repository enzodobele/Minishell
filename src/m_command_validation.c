/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_command_validation.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzimeris <mzimeris@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 13:13:37 by mzimeris          #+#    #+#             */
/*   Updated: 2025/08/19 12:56:48 by mzimeris         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "m_minishell.h"

static char	*build_command_path(char *dir, char *cmd)
{
	char	*path;
	char	*temp;

	temp = ft_strjoin(dir, "/");
	if (!temp)
		return (NULL);
	path = ft_strjoin(temp, cmd);
	free(temp);
	return (path);
}

int	check_command(t_env *env, t_command *cmd)
{
	char	*full_path;
	int		i;

	if (!env || !cmd || !cmd->cmd)
		return (-1);
	if (ft_strchr(cmd->cmd->string, '/'))
	{
		if (access(cmd->cmd->string, F_OK | X_OK) == 0)
			return (0);
		return (-1);
	}
	if (!env->path)
		return (-1);
	i = 0;
	while (env->path[i])
	{
		full_path = build_command_path(env->path[i], cmd->cmd->string);
		if (full_path)
		{
			if (access(full_path, F_OK | X_OK) == 0)
			{
				free(cmd->cmd->string);
				cmd->cmd->string = full_path;
				return (0);
			}
			free(full_path);
		}
		i++;
	}
	return (-1);
}
