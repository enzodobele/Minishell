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

    signal(SIGINT, handle_sigint); // gère ctrl-c
    signal(SIGQUIT, SIG_IGN); // gère ctrl-\ 
    while(1)
    {
        input = readline("minishell$ ");
        if (input && input[0])
            add_history(input);
        else
        {
            write(1, "exit\n", 5);
            break;
        }
    }
    return (0);
}