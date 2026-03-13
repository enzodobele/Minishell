/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_syntaxe_error.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edobele <edobele@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 16:52:24 by edobele           #+#    #+#             */
/*   Updated: 2025/08/28 16:52:25 by edobele          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_syntax_error_doubles(char c)
{
	printf("minishell: syntax error near unexpected token `%c%c'\n", c, c);
}

void	print_syntax_error_singles(char c)
{
	printf("minishell: syntax error near unexpected token `%c'\n", c);
}

void	print_syntax_error_newlines(void)
{
	printf("minishell: syntax error near unexpected token `newline'\n");
}

void	print_syntax_error_pipe(void)
{
	printf("Minishell: syntax error near unexpected token `|'\n");
}
