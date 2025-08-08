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
	LOGICAL_OR, // ||
	REDIR_IN, // <
	REDIR_OUT, // >
	REDIR_APPEND, // >>
	HEREDOC, // <<
	VAR, // $
}	t_token_type;

typedef enum	e_quote_type {
	NO_QUOTE,
	SINGLE_QUOTE,
	DOUBLE_QUOTE,
	NO_WORD
}	t_quote_type;

typedef	struct	s_token
{
	char			*string;
	t_token_type	type;
	t_quote_type	quote_type;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

typedef struct s_token_data
{
	t_token_type	type;
	t_quote_type	quote;
	int				start;
	int				len;
}	t_token_data;

void	handle_sigint(int signum);
int		tokenizer(char *input, t_token **token, int i);
int		is_char_symbol(char *input, t_token **token, int i);
int		is_single_quote(t_token **token, char *input, int i);
int		is_double_quote(t_token **token, char *input, int i);
int		add_new_token_word(int start, int i, t_token **token, char *input);
int		is_special_char(char c);
int		ft_isdigit(int c);
int		is_dollard(t_token **token, char *input, int i);
int		has_unclosed_quotes(char *input);
int		create_token(t_token_data data, char *input, t_token **token);
int		is_greater_than_symbol(t_token **token, char *input, int i);
int		is_less_than_symbol(t_token **token, char *input, int i);
int		is_pipe(t_token **token, char *input, int i);
int		is_token_valid(t_token *token);
int		has_leading_pipe(char *input);

// lib
char	*ft_strdup(const char *s, int len_s, int i);
int		ft_strlen(const char *s);
void	ft_lstadd_back(t_token **lst, t_token *new);
void	ft_tokenlstclear(t_token **lst);
int		ft_isalpha(int c);
int		ft_isalnum(int c);
int		ft_isdigit(int c);
char	*ft_strjoin(const char *str1, const char *str2);
int		ft_strcmp(const char *s1, const char *s2);

#endif