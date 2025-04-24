/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_env.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julien <julien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 21:30:00 by julien            #+#    #+#             */
/*   Updated: 2025/04/24 21:30:00 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

size_t	count_env(char **env)
{
	size_t	i;

	i = 0;
	while (env[i])
		i++;
	return (i);
}

void	free_env_arr(char **env)
{
	size_t	i;

	i = 0;
	if (!env)
		return ;
	while (env[i])
		free(env[i++]);
	free(env);
}

int	copy_env(char **new_env, char **env, size_t env_count)
{
	size_t	i;

	i = 0;
	while (i < env_count)
	{
		new_env[i] = ft_strdup(env[i]);
		if (!new_env[i])
		{
			free_env_arr(new_env);
			return (1);
		}
		i++;
	}
	return (0);
}

int	add_env(t_shell *sh, const char *var)
{
	char	**new_env;
	size_t	env_count;

	env_count = count_env(sh->env);
	new_env = malloc(sizeof(char *) * (env_count + 2));
	if (!new_env)
		return (1);
	if (copy_env(new_env, sh->env, env_count))
		return (1);
	new_env[env_count] = ft_strdup(var);
	if (!new_env[env_count])
	{
		free_env_arr(new_env);
		return (1);
	}
	new_env[env_count + 1] = NULL;
	free_env_arr(sh->env);
	sh->env = new_env;
	return (0);
}

int	replace_var(t_shell *sh, const char *var, size_t var_len)
{
	size_t	i;

	i = 0;
	while (sh->env[i])
	{
		if (ft_strncmp(sh->env[i], var, var_len) == 0
			&& sh->env[i][var_len] == '=')
		{
			free(sh->env[i]);
			sh->env[i] = ft_strdup(var);
			if (!sh->env[i])
				return (1);
			return (0);
		}
		i++;
	}
	return (-1);
}
