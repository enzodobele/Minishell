/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzimeris <mzimeris@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 11:30:09 by mzimeris          #+#    #+#             */
/*   Updated: 2025/08/27 20:16:15 by mzimeris         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_env_node(t_env *env, t_env_node *new_node)
{
	t_env_node	*last;

	if (!env || !new_node)
		return ;
	if (env->env_list == NULL)
	{
		env->env_list = new_node;
		return ;
	}
	last = env->env_list;
	while (last->next)
		last = last->next;
	last->next = new_node;
}

t_env_node	*create_env_node(t_env *env, char *key, char *value)
{
	t_env_node	*new_node;

	if (!key)
		return (NULL);
	new_node = malloc(sizeof(t_env_node));
	if (!new_node)
		return (NULL);
	new_node->key = ft_strdup(key, ft_strlen(key), 0);
	if (!new_node->key)
		return (free(new_node), NULL);
	if (value)
	{
		new_node->value = ft_strdup(value, ft_strlen(value), 0);
		if (!new_node->value)
		{
			free(new_node->key);
			return (free(new_node), NULL);
		}
	}
	else
		new_node->value = NULL;
	new_node->next = NULL;
	if (new_node)
		add_env_node(env, new_node);
	return (new_node);
}

static t_env_node	*_extract_env_node(t_env *env, char *line)
{
	char		**kval;
	t_env_node	*node;

	kval = ft_split(line, '=');
	if (!kval || !kval[0])
	{
		if (kval)
			free_splitted(kval);
		return (NULL);
	}
	node = create_env_node(env, kval[0], kval[1]);
	if (!node)
	{
		free_splitted(kval);
		return (NULL);
	}
	free_splitted(kval);
	return (node);
}

t_env_node	*env_loop(t_env *new_env, char *line)
{
	t_env_node	*node;

	node = _extract_env_node(new_env, line);
	if (!node && ft_strchr(line, '='))
		return (NULL);
	if (node && ft_strcmp(node->key, "PATH") == 0 && node->value)
	{
		new_env->path = ft_split(node->value, ':');
		if (!new_env->path)
			return (handle_sys_error("Memory allocation failed"), NULL);
	}
	return (node);
}

t_env	*extract_env(char **envp)
{
	char		*line;
	t_env		*new_env;

	if (!envp || !*envp)
		return (NULL);
	new_env = malloc(sizeof(t_env));
	if (!new_env)
		return (NULL);
	*new_env = (t_env){0};
	while (*envp)
	{
		line = ft_strdup(*envp, ft_strlen(*envp), 0);
		if (!line)
		{
			free_splitted(new_env->path);
			free(new_env);
			return (NULL);
		}
		env_loop(new_env, line);
		free(line);
		envp++;
	}
	return (new_env);
}
