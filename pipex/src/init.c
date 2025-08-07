/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoum <zoum@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 14:35:13 by mzimeris          #+#    #+#             */
/*   Updated: 2025/08/07 10:42:12 by zoum             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	get_paths_from_envp(t_pipex *pipex, char *envp[])
{
	int		i;

	pipex->path = NULL;
	if (!envp)
		return ;
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			pipex->path = ft_split_alloc(pipex->allocator, envp[i] + 5, ':');
			return ;
		}
		i++;
	}
}

t_pipex	*init_pipex(t_pipex *pipex, char *envp[])
{
	pipex = malloc(sizeof(t_pipex));
	if (!pipex)
		return (NULL);
	*pipex = (t_pipex){0};
	pipex->infile = NULL;
	pipex->outfile = NULL;
	pipex->infile_fd = -1;
	pipex->outfile_fd = -1;
	pipex->cmds = NULL;
	pipex->path = NULL;
	pipex->envp = envp;
	pipex->allocator = init_allocator();
	if (!pipex->allocator)
		return (NULL);
	get_paths_from_envp(pipex, envp);
	return (pipex);
}

char	***init_cmds(t_lalloc *allocator, int cmd_count)
{
	char	***cmds;
	int		i;

	cmds = ft_my_malloc(allocator, sizeof(char **) * (cmd_count + 1));
	if (!cmds)
		return (NULL);
	i = 0;
	while (i < cmd_count)
	{
		cmds[i] = NULL;
		i++;
	}
	cmds[cmd_count] = NULL;
	return (cmds);
}
