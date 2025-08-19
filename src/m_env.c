/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_env.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzimeris <mzimeris@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 11:30:09 by mzimeris          #+#    #+#             */
/*   Updated: 2025/08/19 13:57:49 by mzimeris         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "m_minishell.h"

t_env_node	*create_env_node_only(char *key, char *value)
{
	t_env_node	*new_node;

	if (!key)
		return (NULL);
	new_node = malloc(sizeof(t_env_node));
	if (!new_node)
		return (NULL);
	new_node->key = ft_strdup(key, ft_strlen(key), 0);
	if (value)
		new_node->value = ft_strdup(value, ft_strlen(value), 0);
	else
		new_node->value = NULL;
	new_node->next = NULL;
	return (new_node);
}

void	create_env_node(t_env **env, char *key, char *value)
{
	t_env_node	*new_node;

	new_node = create_env_node_only(key, value);
	if (new_node)
		add_env_node(env, new_node);
}

void	add_env_node(t_env **env, t_env_node *new_node)
{
	t_env_node	*last;

	if (!env || !new_node || !*env)
		return ;
	if ((*env)->env_list == NULL)
	{
		(*env)->env_list = new_node;
		return ;
	}
	last = (*env)->env_list;
	while (last->next)
		last = last->next;
	last->next = new_node;
}

static t_env_node	*extract_env_node(char *line)
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
	node = create_env_node_only(kval[0], kval[1]);
	free_splitted(kval);
	return (node);
}

t_env	*extract_env(char **envp, t_env **env)
{
	char		*line;
	t_env		*new_env;
	t_env_node	*node;
	t_env_node	*last;

	if (!envp || !*envp)
		return (NULL);

	new_env = malloc(sizeof(t_env));
	if (!new_env)
		return (NULL);
	new_env->path = NULL;
	new_env->env_list = NULL;
	while (*envp)
	{
		line = ft_strdup(*envp, ft_strlen(*envp), 0);
		node = extract_env_node(line);
		if (node)
		{
			if (new_env->env_list == NULL)
				new_env->env_list = node;
			else
			{
				last = new_env->env_list;
				while (last->next)
					last = last->next;
				last->next = node;
			}
			if (ft_strcmp(node->key, "PATH") == 0 && node->value)
				new_env->path = ft_split(node->value, ':');
		}
		free(line);
		envp++;
	}
	*env = new_env;
	return (new_env);
}

int	print_export(t_env **env)
{
	t_env_node	*current;

	if (!env || !*env)
		return (0);
	current = (*env)->env_list;
	while (current)
	{
		if (current->value)
			printf("declare -x %s=\"%s\"\n", current->key, current->value);
		else
			printf("declare -x %s\n", current->key);
		current = current->next;
	}
	return (0);
}

static void	del_env_node(t_env_node *node)
{
	if (!node)
		return ;
	free(node->key);
	free(node->value);
	free(node);
}

void	remove_env_node(t_env **env, t_env_node *node)
{
	t_env_node	*current;
	t_env_node	*prev_node;

	if (!env || !*env || !node)
		return ;
	current = (*env)->env_list;
	if (current && ft_strcmp(current->key, node->key) == 0)
	{
		(*env)->env_list = current->next;
		del_env_node(current);
		return ;
	}
	prev_node = NULL;
	while (current && ft_strcmp(current->key, node->key) != 0)
	{
		prev_node = current;
		current = current->next;
	}
	if (current && ft_strcmp(current->key, node->key) == 0)
	{
		prev_node->next = current->next;
		del_env_node(current);
	}
}

void	clear_env(t_env **env)
{
	t_env_node	*current;
	t_env_node	*tmp;

	if (!env || !*env)
		return ;
	current = (*env)->env_list;
	while (current)
	{
		tmp = current;
		current = current->next;
		del_env_node(tmp);
	}
	if ((*env)->path)
		free_splitted((*env)->path);
	free(*env);
	*env = NULL;
}

t_env_node	*get_env(t_env *env, const char *key)
{
	t_env_node	*current;
// printf("get_env called with key: %s\n", key);
	if (!env || !key)
		return (NULL);
	current = env->env_list;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}
