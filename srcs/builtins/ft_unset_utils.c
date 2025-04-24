/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julien <julien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 22:50:00 by julien            #+#    #+#             */
/*   Updated: 2025/04/24 22:50:00 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	should_remove_var(char *env_var, char *var)
{
	return (ft_strncmp(env_var, var, ft_strlen(var)) == 0
		&& env_var[ft_strlen(var)] == '=');
}

static int	count_env_vars(t_shell *sh, char *var, int *remove_count)
{
	int	env_count;

	env_count = 0;
	*remove_count = 0;
	while (sh->env[env_count])
	{
		if (ft_strncmp(sh->env[env_count], var, ft_strlen(var)) == 0
			&& sh->env[env_count][ft_strlen(var)] == '=')
			(*remove_count)++;
		env_count++;
	}
	return (env_count);
}

static char	**create_new_env(t_shell *sh, char *var, int env_count,
	int remove_count)
{
	char	**new_env;
	int		i;
	int		j;

	new_env = malloc(sizeof(char *) * (env_count - remove_count + 1));
	if (!new_env)
		return (NULL);
	i = 0;
	j = 0;
	while (sh->env[i])
	{
		if (should_remove_var(sh->env[i], var))
		{
			i++;
			continue ;
		}
		new_env[j++] = ft_strdup(sh->env[i++]);
	}
	new_env[j] = NULL;
	return (new_env);
}

void	remove_env_var(t_shell *sh, char *var)
{
	int		env_count;
	int		remove_count;
	char	**new_env;
	int		i;

	env_count = count_env_vars(sh, var, &remove_count);
	new_env = create_new_env(sh, var, env_count, remove_count);
	if (!new_env)
		return ;
	i = 0;
	while (sh->env[i])
		free(sh->env[i++]);
	free(sh->env);
	sh->env = new_env;
}

int	is_valid_identifier(const char *str)
{
	size_t	i;

	i = 1;
	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
