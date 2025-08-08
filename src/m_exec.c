/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_exec.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzimeris <mzimeris@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 13:13:37 by mzimeris          #+#    #+#             */
/*   Updated: 2025/08/08 15:11:30 by mzimeris         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "m_minishell.h"

// int	handle_echo(t_token *token)
// {

// }

int	handle_cd(t_token *token)
{
	if (!token || !token->string)
		return (-1);
	if (token->string[0] == '\0')
		return (chdir(getenv("HOME")));
	else if (token->string[0] == '/' || token->string[0] == '~')
		return (chdir(token->string));
	return (-1);
}

int	handle_pwd(void)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		return (printf("%s\n", cwd));
	return (-1);
}
// pwd return -1 on failure positive on success

// int	handle_export(t_token *token)
// {
	
// }

// int	handle_unset(t_token *token)
// {

// }

// int	handle_env(t_token *token)
// {
	
// }

// int	handle_exit(t_token *token)
// {
	
// }

// int	exec_builtins(t_token *token)
// {
// 	if (!token || !token->string)
// 		return (0);
// 	if (ft_strcmp(token->string, "echo") == 0) // besoin du param
// 		return (handle_echo(token));
// 	if (ft_strcmp(token->string, "cd") == 0) // besoin du chemin
// 		return (handle_cd(token));
// 	if (ft_strcmp(token->string, "pwd") == 0)
// 		return (handle_pwd(token));
// 	if (ft_strcmp(token->string, "export") == 0)
// 		return (handle_export(token));
// 	if (ft_strcmp(token->string, "unset") == 0)
// 		return (handle_unset(token));
// 	if (ft_strcmp(token->string, "env") == 0)
// 		return (handle_env(token));
// 	if (ft_strcmp(token->string, "exit") == 0)
// 		return (handle_exit(token));
// 	else
// 		return (find_and_exec(token));
// 	return (0);
// }

// int	exec(t_token *token)
// {
// 	(void)token;
// 	return (0);
// }
