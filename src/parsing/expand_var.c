/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzimeris <mzimeris@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 16:53:23 by edobele           #+#    #+#             */
/*   Updated: 2025/08/28 20:04:16 by mzimeris         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	handle_env_var(t_expand *ctx)
{
	char	var_name[256];
	int		var_len;
	char	*env_value;

	var_len = extract_var_name(ctx->str, ctx->i, var_name);
	if (var_len > 0)
	{
		env_value = get_env_value(var_name, ctx->env);
		if (env_value)
			copy_env_value(ctx->res, ctx->res_pos, env_value);
	}
	else
		ctx->res[(*ctx->res_pos)++] = '$';
	return (0);
}

char	*process_expand(const char *str, t_env **env, char *res, int res_pos)
{
	int			i;
	t_expand	ctx;

	i = 0;
	ctx.str = str;
	ctx.i = &i;
	ctx.res = res;
	ctx.res_pos = &res_pos;
	ctx.env = env;
	while (str[i] && res_pos < 2047)
	{
		if (str[i] == '$')
		{
			if (str[i + 1] == '?')
				i += handle_exit_status(res, &res_pos, env);
			else
				handle_env_var(&ctx);
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
	process_expand(str, env, result, 0);
	final = malloc(ft_strlen(result) + 1);
	if (final)
		ft_strcpy(final, result);
	free(result);
	return (final);
}
