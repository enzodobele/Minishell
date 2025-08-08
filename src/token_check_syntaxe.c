#include "minishell.h"

int is_token_valid(t_token *token)
{

	while (token)
	{
		if (token -> type == PIPE || token -> type == LOGICAL_OR)
		{
			if (token -> type == LOGICAL_OR)
				return (printf("LOGICAL OR not handled\n"), 0);
		}
	}
	return (1);
}