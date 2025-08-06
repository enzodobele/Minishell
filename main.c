#include "minishell.h"

void	handle_sigint(int signum)
{
	(void)signum;
	write(1, "\n", 1);
    rl_replace_line("", 0); // del la lign de l'historique
	rl_on_new_line(); // dit a l'histo qu'on est sur une newline
	rl_redisplay(); // reaffiche minishell$
}

int main(void)
{
    char    *input;
	t_token *token;
	int 	i = 0;

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
		if (input[0])
		{
			add_history(input);
			tokenizer(input, &token);
		}
		t_token *tmp_token = token;
		int i = 0;
		while (tmp_token)
		{
   			if (tmp_token->string)
    			printf("token n° %d : %s\n", i, tmp_token->string);
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
