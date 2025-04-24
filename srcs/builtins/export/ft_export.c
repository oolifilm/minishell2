/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julien <julien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 14:16:14 by jbanchon          #+#    #+#             */
/*   Updated: 2025/04/25 00:42:19 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

size_t		count_env(char **env);
void		free_env_arr(char **env);
int			copy_env(char **new_env, char **env, size_t env_count);
int			add_env(t_shell *sh, const char *var);
int			replace_var(t_shell *sh, const char *var, size_t var_len);
int			print_sorted_env(t_shell *sh);

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
	return (add_env(sh, var));
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
