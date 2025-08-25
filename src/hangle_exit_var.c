#include "minishell.h"

int	handle_exit_status(char *res, int *res_pos, t_env **env)
{
	char	status_str[12];

	ft_itoa_into(status_str, (*env)->last_exit_status);
	copy_env_value(res, res_pos, status_str);
	return (2);
}

int	handle_env_var(const char *str, int *i, char *res, int *res_pos, t_env **env)
{
	char	var_name[256];
	int		var_len;
	char	*env_value;

	var_len = extract_var_name(str, i, var_name);
	if (var_len > 0)
	{
		env_value = get_env_value(var_name, env);
		if (env_value)
			copy_env_value(res, res_pos, env_value);
	}
	else
		res[(*res_pos)++] = '$';
	return (0);
}