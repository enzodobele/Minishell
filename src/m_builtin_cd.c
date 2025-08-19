/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_builtin_cd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzimeris <mzimeris@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 13:13:37 by mzimeris          #+#    #+#             */
/*   Updated: 2025/08/19 13:00:00 by mzimeris         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "m_minishell.h"

int	handle_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("pwd");
		return (1);
	}
	printf("%s\n", cwd);
	free(cwd);
	return (0);
}

int	handle_cd(t_command *command, t_env **env)
{
	char	*path;
	char	*home;

	if (!command->args[1])
	{
		home = get_env_value(*env, "HOME");
		if (!home)
		{
			printf("cd: HOME not set\n");
			return (1);
		}
		path = home;
	}
	else
		path = command->args[1];
	if (chdir(path) == -1)
	{
		perror("cd");
		return (1);
	}
	return (0);
}
