/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_var.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edobele <edobele@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 16:53:32 by edobele           #+#    #+#             */
/*   Updated: 2025/08/28 16:53:33 by edobele          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process_string(char **str_ptr, t_env **env)
{
	char	*expanded;

	if (!str_ptr || !*str_ptr)
		return ;
	if (contains_env_vars(*str_ptr))
	{
		expanded = expand_env_vars(*str_ptr, env);
		if (expanded)
		{
			free(*str_ptr);
			*str_ptr = expanded;
		}
	}
}

void	process_redirects(t_redirect *redirect, t_env **env)
{
	while (redirect)
	{
		if (redirect->quote_type != SINGLE_QUOTE)
			process_string(&redirect->filename, env);
		redirect = redirect->next;
	}
}

void	process_token(t_token *token, t_env **env)
{
	if (token && token->quote_type != SINGLE_QUOTE)
		process_string(&token->string, env);
}
