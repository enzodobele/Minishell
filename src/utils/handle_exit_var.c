/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_exit_var.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzimeris <mzimeris@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 16:52:18 by edobele           #+#    #+#             */
/*   Updated: 2025/08/28 20:01:23 by mzimeris         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_exit_status(char *res, int *res_pos, t_env **env)
{
	char	status_str[12];

	ft_itoa_into(status_str, (*env)->last_exit_status);
	copy_env_value(res, res_pos, status_str);
	return (2);
}

char	*get_env_value(const char *var_name, t_env **env)
{
	t_env_node	*current;

	if (!var_name || !env || !*env || !(*env)->env_list)
		return (NULL);
	current = (*env)->env_list;
	while (current)
	{
		if (current->key && ft_strcmp(current->key, var_name) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

char	*copy_env_value(char *result, int *result_pos, char *env_value)
{
	int	env_len;

	env_len = ft_strlen(env_value);
	if (*result_pos + env_len < 2047)
	{
		ft_strcpy(&result[*result_pos], env_value);
		*result_pos += env_len;
	}
	return (result);
}
