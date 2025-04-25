/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbanchon <jbanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 22:40:00 by julien            #+#    #+#             */
/*   Updated: 2025/04/25 18:52:59 by jbanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	free_env_and_shell(t_shell *sh, int i)
{
	while (i > 0)
		free(sh->env[--i]);
	free(sh->env);
	free(sh);
}

void	free_shell(t_shell *sh)
{
	int	i;

	if (!sh)
		return ;
	if (sh->env)
	{
		i = 0;
		while (sh->env[i])
		{
			free(sh->env[i]);
			i++;
		}
		free(sh->env);
	}
	free(sh);
}

static int	allocate_env(t_shell *sh, int count)
{
	sh->env = malloc(sizeof(char *) * (count + 1));
	if (!sh->env)
	{
		free(sh);
		return (0);
	}
	return (1);
}

static int	copy_env_vars(t_shell *sh, char **envp, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		sh->env[i] = ft_strdup(envp[i]);
		if (!sh->env[i])
		{
			free_env_and_shell(sh, i);
			return (0);
		}
		i++;
	}
	sh->env[count] = NULL;
	return (1);
}

t_shell	*init_shell(char **envp)
{
	t_shell	*sh;
	int		count;

	sh = malloc(sizeof(t_shell));
	if (!sh)
		return (NULL);
	count = 0;
	while (envp[count])
		count++;
	if (!allocate_env(sh, count))
		return (NULL);
	if (!copy_env_vars(sh, envp, count))
		return (NULL);
	sh->last_exit_status = 0;
	return (sh);
}
