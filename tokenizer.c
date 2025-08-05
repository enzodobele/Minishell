#include "minishell.h"

void input_in_token(char *input)
{
	int		i;
	t_token	token;
	char	*string;

	i = 0;
	while (input[i])
	{
		while (input[i] == ' ')
			i++;
		if (input[i])
		{
			string = input_in_string(i, input);
		}
		while (input[i] && input[i] == ' ')
			i++;
	}
}
char *input_in_string(int i, char *input)
{
	int		j;
	int		nbr_letters;
	char	*string;

	nbr_letters = 0;
	while (input[i] != ' ' && input[i])
	{
		nbr_letters++;
		i++;
	}
	string = malloc(sizeof (char) * (nbr_letters + 1));
	if (!string)
		return (NULL);
	i -= nbr_letters;
	j = 0;
	while (j < nbr_letters)
		string[j++] = input[i++];
	string[j] = '\0';
	return (string);
}
