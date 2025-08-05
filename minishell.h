#ifndef MINISHELL_H
# define MINISHELL_H

#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

typedef	enum	e_token_type
{
	WORD,
	PIPE,
	REDIR_IN, // <
	REDIR_OUT, // >
	REDIR_APPEND, // >>
	HEREDOC, // <<
	VAR, // $
	SEPARATOR // ;
}	t_token_type;

typedef	struct	s_token
{
	char			*string;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

void	handle_sigint(int signum);
void	input_in_token(char *input);
char	*input_in_string(int i, char *input);

#endif