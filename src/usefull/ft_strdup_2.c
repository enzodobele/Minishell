/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edobele <edobele@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 16:52:59 by edobele           #+#    #+#             */
/*   Updated: 2025/08/28 16:53:00 by edobele          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strdup2(const char *s)
{
	int		i;
	char	*tab;
	size_t	len_s;

	i = 0;
	len_s = ft_strlen(s);
	tab = malloc(sizeof(char) * (len_s + 1));
	if (!tab)
		return (0);
	while ((char)s[i])
	{
		tab[i] = (char)s[i];
		i++;
	}
	tab[i] = '\0';
	return (tab);
}
