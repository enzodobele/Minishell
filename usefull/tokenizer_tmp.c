/*
#include "minishell.h"

t_token **input_in_token(char *input)
{
	int		i;
	t_token	**token;
	t_token *new_token;
	char	*string;

	i = 0;
	token = malloc(sizeof (t_token *));
	if (!token)
		return (perror("Malloc failed"), NULL);
	*token = NULL;
	while (input[i])
	{
		while (input[i] == ' ')
			i++;
		if (input[i])
		{
			string = input_in_string(i, input);
			new_token = tokenizer(string);
			free(string);
			if (!new_token)
				return(ft_tokenlstclear(token), NULL);
			ft_lstadd_back(token, new_token);
		}
		while (input[i] != ' ' && input[i])
			i++;
	}
	return (token);
}

t_token	*tokenizer(char *string)
{
	t_token *new_token;

	new_token = malloc (sizeof (t_token));
	if (!new_token)
		return (perror("Malloc failed"), NULL);
	new_token->string = ft_strdup(string);
	if (!new_token->string)
		return (perror("Malloc failed"), free(new_token), NULL);
	new_token->type = 1;
	new_token->next = NULL;
	return (new_token);
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
		return (perror("Malloc failed"), NULL);
	i -= nbr_letters;
	j = 0;
	while (j < nbr_letters)
		string[j++] = input[i++];
	string[j] = '\0';
	return (string);
}*/
