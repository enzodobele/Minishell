#include "minishell.h"

void	add_history_perso(char *input)
{
	int		i;
	int		len;
	char	*tmp_input;

	i = 0;
	len = 0;
	if (!input)
		return ;
	while (input[i])
	{
		if (input[i] == '\n')
		{
			i++;
			while (input[i] && input[i] == '\n')
				i++;
			if (input[i])
				len++;
			continue ;
		}
		i++;
		len++;
	}
	tmp_input = create_input_history(len, input, 0);
	add_history(tmp_input);
	free(tmp_input);
}

char	*create_input_history(int len, char *input, int j)
{
	char	*tmp_input;
	int		i;

	i = 0;
	tmp_input = malloc(sizeof(char) * len + 1);
	if (!tmp_input)
		return (free(input), printf("malloc failed"), NULL);
	while (input[i])
	{
		if (input[i] != '\n')
			tmp_input[j] = input[i];
		else
		{
			while (input[i] && input[i] == '\n')
				i++;
			if (input[i])
				tmp_input[j++] = ' ';
			continue ;
		}
		i++;
		j++;
	}
	tmp_input[j] = '\0';
	return (tmp_input);
}
