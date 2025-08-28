/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edobele <edobele@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 16:52:46 by edobele           #+#    #+#             */
/*   Updated: 2025/08/28 16:52:46 by edobele          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_lstadd_back(t_token **lst, t_token *new)
{
	t_token	*tmp_lst;

	if (!lst || !new)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	tmp_lst = *lst;
	while (tmp_lst -> next != NULL)
		tmp_lst = tmp_lst -> next;
	new -> prev = tmp_lst;
	tmp_lst -> next = new;
}
