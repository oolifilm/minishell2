/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbanchon <jbanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 12:03:45 by jbanchon          #+#    #+#             */
/*   Updated: 2025/04/17 16:15:03 by jbanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static char	*join_exit(const char *key, const char *val)
{
	char	*res;
	size_t	len;

	len = ft_strlen(key) + 1 + ft_strlen(val);
	res = malloc(len + 1);
	if (!res)
		return (NULL);
	ft_strlcpy(res, key, len + 1);
	ft_strlcat(res, "=", len + 1);
	ft_strlcat(res, val, len + 1);
	return (res);
}

char	*get_env_value(char **env, const char *key)
{
	int	i;
	int	key_len;

	if (!env || !key)
		return (NULL);
	key_len = ft_strlen(key);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], key, key_len) == 0 && env[i][key_len] == '=')
			return (ft_strdup(env[i] + key_len + 1));
		i++;
	}
	return (NULL);
}

int	get_exit_code(t_shell *sh)
{
	char	*exit_status;
	int		status;

	exit_status = get_env_value(sh->env, "?");
	if (!exit_status)
		return (0);
	status = ft_atoi(exit_status);
	free(exit_status);
	return (status);
}

int	set_exit_code(t_shell *sh, int status)
{
	char	*status_str;
	char	*new_var;
	int		ret;

	status_str = ft_itoa(status);
	if (!status_str)
		return (1);
	new_var = join_exit("?", status_str);
	free(status_str);
	if (!new_var)
		return (1);
	remove_env_var(sh, "?");
	ret = add_env(sh, new_var);
	free(new_var);
	sh->last_exit_status = status;
	return (ret < 0);
}
