/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_builtin_export.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzimeris <mzimeris@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 16:54:38 by zoum              #+#    #+#             */
/*   Updated: 2025/08/27 18:33:59 by mzimeris         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_export_error(char *arg)
{
	ft_putstr_fd("Minishell: export: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
}

int	handle_export(t_env *env, t_command *command)
{
	char		**kval;
	int			i;
	int			ret;

	ret = 0;
	if (!command->args || !command->args[0])
		return (print_export(env));
	i = 0;
	while (command->args[i])
	{
		kval = ft_split(command->args[i]->string, '=');
		if (!kval)
			return (handle_sys_error("Memory allocation failed"), 1);
		if (check_export(env, command->args[i]->string, kval))
			ret = 1;
		if (kval)
			free_splitted(kval);
		i++;
	}
	return (ret);
}
