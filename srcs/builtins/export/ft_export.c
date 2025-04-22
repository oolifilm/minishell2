/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julien <julien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 14:16:14 by jbanchon          #+#    #+#             */
/*   Updated: 2025/04/22 22:13:13 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static size_t	count_env(char **env)
{
	size_t	i;

	i = 0;
	while (env[i])
		i++;
	return (i);
}

static void	free_env_arr(char **env)
{
	size_t	i;

	i = 0;
	if (!env)
		return ;
	while (env[i])
		free(env[i++]);
	free(env);
}

static int	copy_env(char **new_env, char **env, size_t env_count)
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

static int	replace_var(t_shell *sh, const char *var, size_t var_len)
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

static int	process_var(t_shell *sh, const char *var)
{
	char	*equal_sign;
	size_t	var_len;

	if (!is_valid_env_var(var))
	{
		write(2, "export: '", 9);
		write(2, var, ft_strlen(var));
		write(2, "': not a valid identifier\n", 26);
		return (1);
	}
	equal_sign = ft_strchr(var, '=');
	if (!equal_sign)
		return (0);
	var_len = equal_sign - var;
	if (replace_var(sh, var, var_len) != -1)
		return (0);
	ft_putstr_fd("—> add_env va insérer : ", 2);
	ft_putstr_fd((char *)var, 2);
	ft_putstr_fd("\n", 2);
	return (add_env(sh, var));
}

int	print_sorted_env(t_shell *sh)
{
	char	**sorted_env;
	size_t	env_count;
	size_t	i;
	size_t	j;
	char	*temp;

	env_count = count_env(sh->env);
	sorted_env = malloc(sizeof(char *) * (env_count + 1));
	if (!sorted_env)
		return (1);
	if (copy_env(sorted_env, sh->env, env_count))
		return (1);
	sorted_env[env_count] = NULL;
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
	i = 0;
	while (sorted_env[i])
	{
		printf("export %s\n", sorted_env[i]);
		i++;
	}
	free_env_arr(sorted_env);
	return (0);
}

int	is_valid_key(const char *str)
{
	size_t	i;

	i = 0;
	if (!str[0] || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	ft_export(t_shell *sh, char **argv)
{
	int	i;
	int	ret;

	ret = 0;
	if (!argv || !argv[0])
		return (1);
	if (!argv[1])
		return (print_sorted_env(sh));
	i = 1;
	while (argv[i])
	{
		if (!is_valid_key(argv[i]))
		{
			ft_putstr_fd("export: `", STDERR_FILENO);
			ft_putstr_fd(argv[i], STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			ret = 1;
		}
		else
			process_var(sh, argv[i]);
		i++;
	}
	sh->last_exit_status = ret;
	return (ret);
}
