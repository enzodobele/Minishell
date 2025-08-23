#include "minishell.h"

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

int	extract_var_name(const char *str, int *i, char *var_name)
{
	int	var_start;
	int	var_len;

	(*i)++;
	var_start = *i;
	var_len = 0;
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
	{
		(*i)++;
		var_len++;
	}
	if (var_len > 0)
	{
		ft_strncpy(var_name, &str[var_start], var_len);
		var_name[var_len] = '\0';
	}
	return (var_len);
}

char	*process_expansion(const char *str, t_env **env, char *res, int res_pos)
{
	char	var_name[256];
	int		i;
	int		var_len;
	char	*env_value;

	i = 0;
	while (str[i] && res_pos < 2047)
	{
		if (str[i] == '$')
		{
			var_len = extract_var_name(str, &i, var_name);
			if (var_len > 0)
			{
				env_value = get_env_value(var_name, env);
				if (env_value)
					copy_env_value(res, &res_pos, env_value);
			}
			else
				res[res_pos++] = '$';
		}
		else
			res[res_pos++] = str[i++];
	}
	res[res_pos] = '\0';
	return (res);
}

char	*expand_env_vars(const char *str, t_env **env)
{
	char	*result;
	char	*final;

	if (!str || !contains_env_vars(str))
		return (ft_strdup2(str));
	result = malloc(2048);
	if (!result)
		return (NULL);
	process_expansion(str, env, result, 0);
	final = malloc(ft_strlen(result) + 1);
	if (final)
		ft_strcpy(final, result);
	free(result);
	return (final);
}
