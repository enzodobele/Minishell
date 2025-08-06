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
	PIPE, // |
	REDIR_IN, // <
	REDIR_OUT, // >
	REDIR_APPEND, // >>
	HEREDOC, // <<
	VAR, // $
}	t_token_type;

typedef	struct	s_token
{
	char			*string;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

void	handle_sigint(int signum);
int		tokenizer(char *input, t_token **token);
int		is_char_symbol(char *input, t_token **token, int i);
int		add_new_token_symbol_duo(t_token_type symbol, int i, t_token **token, char *input);
int		add_new_token_symbol_solo(t_token_type symbol, int i, t_token **token, char *input);
int		add_new_token_word(int start, int i, t_token **token, char *input);
int		is_special_char(char c);


// lib
char	*ft_strdup(const char *s, int len_s, int i);
int		ft_strlen(const char *s);
void	ft_lstadd_back(t_token **lst, t_token *new);
void	ft_tokenlstclear(t_token **lst);

#endif