/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_exec.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoum <zoum@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 13:13:37 by mzimeris          #+#    #+#             */
/*   Updated: 2025/08/10 00:14:48 by zoum             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "m_minishell.h"

// int	handle_echo(t_token *token)
// {

// }

int	handle_pwd(void)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		return (printf("%s\n", cwd));
	return (-1);
}



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

int	exec_builtins(char **commands)
{
	if (!commands || !commands[0])
		return (0);
	
	if (ft_strcmp(commands[0], "cd") == 0)
		return (handle_cd(commands));
	if (ft_strcmp(commands[0], "pwd") == 0)
		return (handle_pwd());
	// if (ft_strcmp(commands[0], "echo") == 0)
	// 	return (handle_echo(commands));
	// if (ft_strcmp(commands[0], "export") == 0)
	// 	return (handle_export(commands));
	// if (ft_strcmp(commands[0], "unset") == 0)
	// 	return (handle_unset(commands));
	// if (ft_strcmp(commands[0], "env") == 0)
	// 	return (handle_env(commands));
	// if (ft_strcmp(commands[0], "exit") == 0)
	// 	return (handle_exit(commands));
	// else
	// 	return (find_and_exec(commands));
	
	printf("Commande '%s' non reconnue comme builtin\n", commands[0]);
	return (0);
}

int	exec(char **commands)
{
	int	i;

	if (!commands || !commands[0])
	{
		printf("Aucune commande reçue, lancement des tests automatiques...\n");
		run_test_commands();
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
	return (exec_builtins(commands));
}

void	run_test_commands(void)
{
	char	*test1[] = {"pwd", NULL};
	char	*test2[] = {"cd", "..", NULL};
	char	*test3[] = {"cd", "/tmp", NULL};
	char	*test4[] = {"cd", "~", NULL};
	char	*test5[] = {"cd", NULL};
	char	*test6[] = {"cd", "/home/zoum/code/minishell/pipex", NULL};
	char	*test7[] = {"cd", "-", NULL};

	printf("=== TESTS DE COMMANDES SIMPLIFIES ===\n");
	printf("\n--- Test 1: pwd ---\n");
	exec(test1);
	printf("\n--- Test 2: cd .. ---\n");
	exec(test2);
	exec(test1);

	printf("\n--- Test 3: cd /tmp ---\n");
	exec(test3);
	exec(test1);

	printf("\n--- Test 4: cd ~ ---\n");
	exec(test4);
	exec(test1);

	printf("\n--- Test 5: cd (sans paramètre) ---\n");
	exec(test5);
	exec(test1);

	printf("\n--- Test 6: cd ---\n");
	exec(test6);
	exec(test1);

	exec(test7);
	exec(test1);
	printf("\n=== FIN DES TESTS ===\n");
}
