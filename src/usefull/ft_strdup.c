/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edobele <edobele@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 16:53:02 by edobele           #+#    #+#             */
/*   Updated: 2025/08/28 16:53:02 by edobele          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_strdup(const char *s, int len_s, int i)
{
	int		j;
	char	*tab;

	tab = malloc(sizeof(char) * (len_s + 1));
	if (!tab)
		return (0);
	j = 0;
	while (j < len_s && s[i])
	{
		tab[j] = s[i];
		i++;
		j++;
	}
	tab[j] = '\0';
	return (tab);
}
