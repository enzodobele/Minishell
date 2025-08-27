/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_error_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzimeris <mzimeris@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 17:15:00 by mzimeris          #+#    #+#             */
/*   Updated: 2025/08/27 14:52:14 by mzimeris         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_command_not_found_error(const char *command)
{
	ft_putstr_fd("Minishell: ", 2);
	ft_putstr_fd((char *)command, 2);
	ft_putstr_fd(": command not found\n", 2);
}

static void	print_no_such_file_error(const char *command)
{
	ft_putstr_fd("Minishell: ", 2);
	ft_putstr_fd((char *)command, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
}

static void	print_permission_denied_error(const char *command)
{
	ft_putstr_fd("Minishell: ", 2);
	ft_putstr_fd((char *)command, 2);
	ft_putstr_fd(": Permission denied\n", 2);
}

void	handle_export_error(char *arg)
{
	ft_putstr_fd("Minishell: export: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
}

void	handle_cd_error(t_command *command, int result)
{
	if (!command || !command->cmd || !command->cmd->string)
		return ;
	if (!result)
		return ;
	if (result == 1)
		ft_putstr_fd("Minihell: cd: No such file or directory: ", 2);
	else if (result == 2)
		ft_putstr_fd("Minihell: cd: Not a directory: ", 2);
	else if (result == 3)
		ft_putstr_fd("Minishell: cd: Permission denied: ", 2);
	ft_putstr_fd(command->args[0]->string, 2);
	ft_putstr_fd("\n", 2);
}

void	handle_command_error(t_command *command, int check_result)
{
	if (check_result == -2)
	{
		print_permission_denied_error(command->cmd->string);
		exit(126);
	}
	if (check_result < 0)
	{
		if (ft_strchr(command->cmd->string, '/') != NULL)
			print_no_such_file_error(command->cmd->string);
		else
			print_command_not_found_error(command->cmd->string);
		exit(127);
	}
}

void	handle_system_error(const char *error_msg)
{
	ft_putstr_fd("Error: ", 2);
	ft_putstr_fd((char *)error_msg, 2);
	ft_putstr_fd("\n", 2);
	exit(1);
}
