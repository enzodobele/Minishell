/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_command_validation.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoum <zoum@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 18:45:00 by mzimeris          #+#    #+#             */
/*   Updated: 2025/08/13 21:21:45 by zoum             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "m_minishell.h"

static char	*check_full_path(t_command *cmd, char *path)
{
	char	*full_path;
	char	*temp;

	temp = malloc(ft_strlen(path) + 2);
	if (!temp)
		return (NULL);
	ft_strlcpy(temp, path, ft_strlen(path) + 1);
	ft_strlcat(temp, "/", ft_strlen(path) + 2);
	full_path = ft_strjoin(temp, cmd->cmd->string);
	if (!full_path)
		return (NULL);
	if (access(full_path, F_OK) == 0 && access(full_path, X_OK) == 0)
		return (full_path);
	free(full_path);
	return (NULL);
}

static int	check_absolute_path(char *cmd)
{
	if (access(cmd, F_OK) != 0)
		return (-1);
	if (access(cmd, X_OK) != 0)
		return (-2);
	return (0);
}

static int	search_in_path(char **path, t_command *cmd)
{
	int		i;
	char	*full_path;

	i = -1;
	while (path[++i])
	{
		if (ft_strchr(cmd->cmd->string, '/') != NULL)
			return (check_absolute_path(cmd->cmd->string));
		if (ft_strchr(path[i], ' ') != NULL || ft_strchr(path[i], '\t') != NULL)
			continue ;
		if (ft_strlen(path[i]) == 0)
			continue ;
		if (ft_strcmp(path[i], ".") == 0 || ft_strcmp(path[i], "..") == 0)
			continue ;
		full_path = check_full_path(cmd, path[i]);
		if (full_path)
		{
			free(cmd->cmd->string);
			cmd->cmd->string = full_path;
			return (0);
		}
	}
	return (-1);
}

int	check_command(t_env *env, t_command *cmd)
{
	char	**path;
	t_env	*node;

	node = env;
	if (!cmd || !env)
		return (-1);
	if (!cmd)
		return (-1);
	if (ft_strchr(cmd->cmd->string, '/') != NULL)
		return (check_absolute_path(cmd->cmd->string));
	path = NULL;
	while (node)
	{
		if (ft_strncmp(node->key, "PATH=", 5) == 0)
		{
			path = ft_split(node->value, ':');
			break ;
		}
		node = node->next;
	}
	search_in_path(path, cmd);
	if (path)
		free_splitted(path);
	return (0);
}
