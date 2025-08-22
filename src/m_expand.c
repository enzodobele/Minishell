/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_expand.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoum <zoum@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 15:48:44 by zoum              #+#    #+#             */
/*   Updated: 2025/08/22 16:27:21 by zoum             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_last_exit_status(t_env *env, t_command *command)
{
	int		i;

	if (!env || !command || !command->args)
		return ;
	i = 0;
	while (command->args[i])
	{
		if (ft_strcmp(command->args[i]->string, "$?") == 0
			&& command->args[i]->quote_type != SINGLE_QUOTE)
		{
			free(command->args[i]->string);
			command->args[i]->string = ft_itoa(env->last_exit_status);
			if (!command->args[i]->string)
				perror("expand last exit status failed");
		}
		i++;
	}
}
