/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_builtin_cd_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzimeris <mzimeris@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 17:34:12 by mzimeris          #+#    #+#             */
/*   Updated: 2025/08/27 18:03:52 by mzimeris         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_cd_error(t_command *command, int result)
{
	if (!command || !command->cmd || !command->cmd->string)
		return ;
	if (!result)
		return ;
	if (result == 1)
		ft_putstr_fd("Minihell: cd: No such file or directory: ", 2);
	else if (result == 2)
		ft_putstr_fd("Minihell: cd: Not a directory: ", 2);
	else if (result == 3)
		ft_putstr_fd("Minishell: cd: Permission denied: ", 2);
	ft_putstr_fd(command->args[0]->string, 2);
	ft_putstr_fd("\n", 2);
}

void	update_oldpwd(t_env *env, char *old_pwd)
{
	t_env_node	*oldpwd_node;
	char		*new_value;

	if (!env || !old_pwd)
		return ;
	oldpwd_node = get_env(env, "OLDPWD");
	if (oldpwd_node)
	{
		new_value = ft_strdup(old_pwd, ft_strlen(old_pwd), 0);
		if (!new_value)
			return ;
		if (oldpwd_node->value)
			free(oldpwd_node->value);
		oldpwd_node->value = new_value;
	}
	else
		create_env_node(env, "OLDPWD", old_pwd);
}
