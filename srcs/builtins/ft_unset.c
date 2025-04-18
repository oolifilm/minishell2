/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbanchon <jbanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 14:16:16 by jbanchon          #+#    #+#             */
/*   Updated: 2025/04/18 13:08:53 by jbanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
Ce qui nous est démandé dans le sujet : unset with no options
=================================================================================
Commande unset : supprimer une variable d'environnement
Utilisation : unset [variable]
*/

void	remove_env_var(t_shell *sh, char *var)
{
	int		i;
	int		j;
	char	**new_env;
	int		env_count;
	int		remove_count;

	i = 0;
	j = 0;
	env_count = 0;
	remove_count = 0;
	while (sh->env[env_count])
	{
		if (ft_strncmp(sh->env[env_count], var, ft_strlen(var)) == 0
			&& sh->env[env_count][ft_strlen(var)] == '=')
			remove_count++;
		env_count++;
	}
	new_env = malloc(sizeof(char *) * (env_count - remove_count + 1));
	if (!new_env)
		return ;
	i = 0;
	j = 0;
	while (sh->env[i])
	{
		if (ft_strncmp(sh->env[i], var, ft_strlen(var)) == 0
			&& sh->env[i][ft_strlen(var)] == '=')
		{
			i++;
			continue ;
		}
		new_env[j++] = ft_strdup(sh->env[i]);
		i++;
	}
	new_env[j] = NULL;
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

int	ft_unset(t_shell *sh, char **argv)
{
	int	i;
	int	ret;

	i = 1;
	ret = 0;
	while (argv[i])
	{
		if (!is_valid_identifier(argv[i]))
		{
			ft_putstr_fd("unset: `", STDERR_FILENO);
			ft_putstr_fd(argv[i], STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			ret = 1;
		}
		else
			remove_env_var(sh, argv[i]);
		i++;
	}
	return (ret);
}
