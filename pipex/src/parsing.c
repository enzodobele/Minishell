/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoum <zoum@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 14:38:18 by mzimeris          #+#    #+#             */
/*   Updated: 2025/08/07 13:06:20 by zoum             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	check_fd(t_pipex *pipex)
{
	pipex->infile_fd = open(pipex->infile, O_RDONLY);
	if (pipex->infile_fd < 0)
	{
		perror(pipex->infile);
		pipex->infile_fd = -1;
		return (-1);
	}
	pipex->outfile_fd = open(pipex->outfile, O_WRONLY | O_CREAT
			| O_TRUNC, 0644);
	if (pipex->outfile_fd < 0)
	{
		perror(pipex->outfile);
		pipex->outfile_fd = -1;
		return (-1);
	}
	return (0);
}

char	*check_full_path(t_lalloc *allocator, char *cmd, char *path_dir)
{
	char	*full_path;
	char	*path_with_slash;

	if (!cmd || !path_dir)
		return (NULL);
	path_with_slash = ft_strjoin_alloc(allocator, path_dir, "/");
	if (!path_with_slash)
		return (NULL);
	full_path = ft_strjoin_alloc(allocator, path_with_slash, cmd);
	ft_my_free(allocator, path_with_slash);
	if (!full_path)
		return (NULL);
	if (access(full_path, F_OK | X_OK) == 0)
		return (full_path);
	ft_my_free(allocator, full_path);
	return (NULL);
}

int	check_command(t_pipex *pipex, char *cmd, int index)
{
	int		i;
	char	*full_path;

	if (!cmd || !pipex || !pipex->path)
		return (-1);
	if (ft_strchr(cmd, '/') != NULL && access(cmd, F_OK | X_OK) == 0)
		return (0);
	i = -1;
	while (pipex->path[++i])
	{
		full_path = check_full_path(pipex->allocator, cmd, pipex->path[i]);
		if (full_path)
		{
			ft_my_free(pipex->allocator, pipex->cmds[index][0]);
			pipex->cmds[index][0] = full_path;
			return (0);
		}
	}
	return (-1);
}

int	parse_one_command(t_pipex *pipex, char *cmd, char ***cmds, int index)
{
	char	**args;

	if (index < 0 || !cmds || !cmd || !*cmd)
		return (-1);
	args = parse_command_with_quotes(pipex->allocator, cmd);
	if (!args)
		return (-1);
	cmds[index] = args;
	return (0);
}

int	parse_args(t_pipex *pipex, int argc, char *argv[])
{
	int	i;

	i = 0;
	pipex->infile = ft_my_malloc(pipex->allocator, ft_strlen(argv[1]) + 1);
	if (!pipex->infile)
		return (-1);
	ft_strlcpy(pipex->infile, argv[1], ft_strlen(argv[1]) + 1);
	pipex->outfile = ft_my_malloc(pipex->allocator,
			ft_strlen(argv[argc - 1]) + 1);
	if (!pipex->outfile)
		return (-1);
	ft_strlcpy(pipex->outfile, argv[argc - 1], ft_strlen(argv[argc - 1]) + 1);
	pipex->cmds = init_cmds(pipex->allocator, argc - 3);
	if (!pipex->cmds)
		return (-1);
	while (i < argc - 3)
	{
		if (parse_one_command(pipex, argv[i + 2], pipex->cmds, i) < 0)
			return (-1);
		i++;
	}
	if (check_fd(pipex) < 0)
		return (-1);
	return (0);
}
