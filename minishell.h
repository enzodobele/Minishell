#ifndef MINISHELL_H
# define MINISHELL_H

#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void	handle_sigint(int signum);

#endif