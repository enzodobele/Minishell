/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_minishell.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoum <zoum@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 13:12:43 by mzimeris          #+#    #+#             */
/*   Updated: 2025/08/10 00:22:54 by zoum             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef M_MINISHELL_H
# define M_MINISHELL_H

# include "minishell.h"
# include <linux/limits.h>

void	debug_print_token(t_token *token);
void	debug_print_all_tokens(t_token *token);

int		exec(char **commands);
int		exec_builtins(char **commands);

int		handle_pwd(void);
int		handle_cd(char **commands);

void	run_test_commands(void);

#endif /* M_MINISHELL_H */