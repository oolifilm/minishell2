/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_sort.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julien <julien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 22:55:00 by julien            #+#    #+#             */
/*   Updated: 2025/04/24 22:55:00 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static void	sort_env_array(char **sorted_env, size_t env_count)
{
	size_t	i;
	size_t	j;
	char	*temp;

	i = 0;
	while (i < env_count - 1)
	{
		j = 0;
		while (j < env_count - i - 1)
		{
			if (ft_strcmp(sorted_env[j], sorted_env[j + 1]) > 0)
			{
				temp = sorted_env[j];
				sorted_env[j] = sorted_env[j + 1];
				sorted_env[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}

static void	print_env_array(char **sorted_env)
{
	size_t	i;

	i = 0;
	while (sorted_env[i])
	{
		printf("export %s\n", sorted_env[i]);
		i++;
	}
}

int	print_sorted_env(t_shell *sh)
{
	char	**sorted_env;
	size_t	env_count;

	env_count = count_env(sh->env);
	sorted_env = malloc(sizeof(char *) * (env_count + 1));
	if (!sorted_env)
		return (1);
	if (copy_env(sorted_env, sh->env, env_count))
		return (1);
	sorted_env[env_count] = NULL;
	sort_env_array(sorted_env, env_count);
	print_env_array(sorted_env);
	free_env_arr(sorted_env);
	return (0);
}
