/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_exec.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoum <zoum@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 13:13:37 by mzimeris          #+#    #+#             */
/*   Updated: 2025/08/12 11:49:10 by zoum             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "m_minishell.h"

// un seul builtin pour echo
// echo -n pas de saut de ligne
int	handle_echo(char **commands)
{
	int	i;

	i = 0;
	if (ft_strcmp(commands[1], "-n") == 0)
	{
		while (commands[2][i] && commands[2][i] != '\n')
		{
			write(1, &commands[2][i], 1);
			i++;
		}
	}
	return (0);
}

int	handle_pwd(void)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		return (printf("%s\n", cwd));
	return (-1);
}



int	handle_exit(void)
{
	exit(0);
}

int	exec_builtins(char **commands, char **envp)
{
	if (!commands || !commands[0])
		return (0);
	if (ft_strcmp(commands[0], "cd") == 0)
		return (handle_cd(commands));
	if (ft_strcmp(commands[0], "pwd") == 0)
		return (handle_pwd());
	if (ft_strcmp(commands[0], "echo") == 0)
		return (handle_echo(commands));
	// if (ft_strcmp(commands[0], "export") == 0)
	// 	return (handle_export(commands));
	// if (ft_strcmp(commands[0], "unset") == 0)
	// 	return (handle_unset(commands));
	if (ft_strcmp(commands[0], "env") == 0)
		return (handle_env(envp));
	if (ft_strcmp(commands[0], "exit") == 0)
		return (handle_exit());
	// else
	// 	return (find_and_exec(commands));
	
	printf("Commande '%s' non reconnue comme builtin\n", commands[0]);
	return (0);
}

int	exec(char **commands, char **envp)
{
	int	i;

	if (!commands || !commands[0])
	{
		printf("Aucune commande reçue, lancement des tests automatiques...\n");
		run_test_commands(envp);
		return (0);
	}
	printf("Exécution de: ");
	i = 0;
	while (commands[i])
	{
		printf("%s ", commands[i]);
		i++;
	}
	printf("\n");
	return (exec_builtins(commands, envp));
}

void	run_test_commands(char **envp)
{
	char	*test1[] = {"env", NULL};
	char	*test2[] = {"exit", NULL};


	printf("=== TESTS DE COMMANDES SIMPLIFIES ===\n");
	printf("\n--- Test 1: echo -n ---\n");
	exec(test1, envp);
	// printf("--- Test 2: exit ---\n");
	exec(test2, envp);
	
	printf("\n=== FIN DES TESTS ===\n");
}
