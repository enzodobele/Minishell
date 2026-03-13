/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzimeris <mzimeris@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 17:54:44 by mzimeris          #+#    #+#             */
/*   Updated: 2025/08/28 20:28:24 by mzimeris         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	print_export(t_env *env)
{
	t_env_node	*current;

	if (!env)
		return (0);
	current = env->env_list;
	while (current)
	{
		if (current->value)
			printf("export %s=\"%s\"\n", current->key, current->value);
		else
			printf("export %s\n", current->key);
		current = current->next;
	}
	return (0);
}

static void	_update_env_node(t_env_node *node, char *value)
{
	if (node->value)
		free(node->value);
	if (value)
	{
		node->value = ft_strdup(value, ft_strlen(value), 0);
		if (!node->value)
			return ;
	}
	else
	{
		node->value = ft_strdup("", 0, 0);
		if (!node->value)
			return ;
	}
}

static void	_create_env_node(t_env *env, char **kval)
{
	if (kval[1])
		create_env_node(env, kval[0], kval[1]);
	else
		create_env_node(env, kval[0], "");
	if (ft_strcmp(kval[0], "PATH") == 0 && kval[1])
	{
		if (env->path)
			free_splitted(env->path);
		env->path = ft_split(kval[1], ':');
	}
}

static int	_check_key_validity(char *key)
{
	int	i;

	if (!key || !key[0])
		return (1);
	if ((key[0] >= '0' && key[0] <= '9') || key[0] == '=')
		return (1);
	i = 0;
	while (key[i] && key[i] != '=')
	{
		if (!(ft_isalnum(key[i]) || key[i] == '_'))
			return (1);
		i++;
	}
	return (0);
}

int	check_export(t_env *env, char *string, char **kval)
{
	t_env_node	*node;
	int			ret;

	ret = 0;
	if (_check_key_validity(kval[0]))
	{
		handle_export_error(string);
		ret = 1;
	}
	else
	{
		node = get_env(env, kval[0]);
		if (node)
			_update_env_node(node, kval[1]);
		else
			_create_env_node(env, kval);
	}
	return (ret);
}
