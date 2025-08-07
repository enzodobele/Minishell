#include "minishell.h"

void	handle_sigint(int signum)
{
	(void)signum;
	write(1, "\n", 1);
    rl_replace_line("", 0); // del la lign de l'historique
	rl_on_new_line(); // dit a l'histo qu'on est sur une newline
	rl_redisplay(); // reaffiche minishell$
}
int	has_unclosed_quotes(char *str)
{
	int	i = 0;
	int	squote = 0;
	int	dquote = 0;

	while (str[i])
	{
		if (str[i] == '\'' && dquote % 2 == 0)
			squote++;
		else if (str[i] == '"' && squote % 2 == 0)
			dquote++;
		i++;
	}
	return (squote % 2 != 0 || dquote % 2 != 0);
}
int main(void)
{
    char    *input;
	t_token *token;

	token = NULL;
    signal(SIGINT, handle_sigint); // gère ctrl-c
    signal(SIGQUIT, SIG_IGN); // gère ctrl-/
    while (1)
{
	input = readline("minishell$ ");
	if (!input)
	{
		write(1, "exit\n", 5);
		break;
	}
	while (has_unclosed_quotes(input))
	{
		char *next_line = readline("> ");
		if (!next_line)
			break;
		char *tmp = input;
		input = ft_strjoin(tmp, "\n");
		free(tmp);
		tmp = input;
		input = ft_strjoin(tmp, next_line);
		free(tmp);
		free(next_line);
	}
	if (input[0])
	{
		add_history(input);
		tokenizer(input, &token, 0);
	}
		t_token *tmp_token = token;
		int i = 0;
		while (tmp_token)
		{
   			if (tmp_token->string)
			{
				printf("token n° %d : %s\n", i, tmp_token->string);
				printf("type: %d + %d\n",tmp_token->type, tmp_token->quote_type);
			}
			else
    			printf("token n° %d : (null string)\n", i);
   			i++;
  			tmp_token = tmp_token->next;
		}
		free(input);
		ft_tokenlstclear(&token);
	}
    return (0);
}
