/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_minishell.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzimeris <mzimeris@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 13:12:43 by mzimeris          #+#    #+#             */
/*   Updated: 2025/08/19 11:36:48 by mzimeris         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef M_MINISHELL_H
# define M_MINISHELL_H

# include "minishell.h"
# include <linux/limits.h>
# include <unistd.h>

typedef struct s_env
{
	char			*key;
	char			*value;
	char			**path;
	struct s_env	*next;
}	t_env;

t_env	*extract_env(char **envp, t_env **env);
void	clear_env(t_env **env);
void	add_env_node(t_env **env, t_env *node);
void	remove_env_node(t_env **env, t_env *node);
void	create_env_node(t_env **env, char *key, char *value);

int		handle_export(t_env **env, t_command *command);
int		handle_unset(t_env **env, t_command *command);
int		handle_env(t_env *env);
int		print_export(t_env **env);

int		handle_echo(t_command *command);

void	debug_print_token(t_token *token);
void	debug_print_all_tokens(t_token *token);

int		exec(t_command *commands, t_env **env, t_token **token);
int		exec_builtins(t_command *commands, t_env **env, t_token **token);
// int		exec_system(t_command *command, t_env **env, t_token **token);

int		handle_pwd(void);
int		handle_cd(t_command *command);

// Command validation
int		check_command(t_env *env, t_command *cmd);

void	run_test_commands(char **envp);

void	free_splitted(char **splitted);
char	**ft_split(const char *s, char c);
size_t	ft_strlcat(char *dst, const char *src, size_t dsize);
size_t	ft_strlcpy(char *dst, const char *src, size_t dsize);
char	*ft_strchr(const char *s, int c);
int		ft_strncmp(const char *s1, const char *s2, size_t n);

#endif /* M_MINISHELL_H */