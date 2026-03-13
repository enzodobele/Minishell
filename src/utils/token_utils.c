/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzimeris <mzimeris@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 15:12:53 by mzimeris          #+#    #+#             */
/*   Updated: 2025/08/28 15:13:25 by mzimeris         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*join_two_tokens(t_token *first, t_token *second)
{
	char	*new_string;
	t_token	*new_token;

	if (!first || !second)
		return (NULL);
	new_string = ft_strjoin(first->string, second->string);
	if (!new_string)
		return (NULL);
	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (free(new_string), NULL);
	new_token->string = new_string;
	new_token->type = WORD;
	if (first->quote_type == DOUBLE_QUOTE || second->quote_type == DOUBLE_QUOTE)
		new_token->quote_type = DOUBLE_QUOTE;
	else if (first->quote_type == SINGLE_QUOTE
		|| second->quote_type == SINGLE_QUOTE)
		new_token->quote_type = SINGLE_QUOTE;
	else
		new_token->quote_type = NO_QUOTE;
	new_token->next = NULL;
	new_token->prev = NULL;
	new_token->have_space = second->have_space;
	return (new_token);
}

t_token	*duplicate_token(t_token *token)
{
	t_token	*new_token;

	if (!token)
		return (NULL);
	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->string = ft_strdup(token->string, ft_strlen(token->string), 0);
	if (!new_token->string)
		return (free(new_token), NULL);
	new_token->type = token->type;
	new_token->quote_type = token->quote_type;
	new_token->next = NULL;
	new_token->prev = NULL;
	new_token->have_space = token->have_space;
	return (new_token);
}
