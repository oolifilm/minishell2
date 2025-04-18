/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leaugust <leaugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 14:14:58 by jbanchon          #+#    #+#             */
/*   Updated: 2025/04/18 19:13:39 by leaugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	ft_strequ(const char *s1, const char *s2)
{
	if (!s1 || !s2)
		return (0);
	while (*s1 && *s2)
	{
		if (*s1 != *s2)
			return (0);
		s1++;
		s2++;
	}
	return (*s1 == *s2);
}

static int	is_echo_option_n(char *str)
{
	int	i;

	i = 0;
	if (str[i++] != '-')
		return (0);
	if (str[i] == '\0')
		return (0);
	while (str[i])
	{
		if (str[i++] != 'n')
			return (0);
	}
	return (1);
}

int	ft_echo(t_shell *sh, char **argv)
{
	int		i;
	int		newline;
	char	*expanded_value;

	i = 1;
	newline = 1;
	(void)sh;
	while (argv[i] && is_echo_option_n(argv[i]))
	{
		newline = 0;
		i++;
	}
	while (argv[i])
	{
		if (ft_strcmp(argv[i], "$") == 0)
		{
			ft_putstr_fd("$", STDOUT_FILENO);
		}
		else if (argv[i][0] == '$')
		{
			if (ft_isalpha(argv[i][1]) || argv[i][1] == '_')
			{
				expanded_value = get_env_value(sh->env, argv[i] + 1);
				if (expanded_value)
				{
					ft_putstr_fd(expanded_value, STDOUT_FILENO);
					free(expanded_value);
				}
				else
				{
					ft_putstr_fd("$", STDOUT_FILENO);
				}
			}
			else
			{
				ft_putstr_fd("$", STDOUT_FILENO);
			}
		}
		else
		{
			ft_putstr_fd(argv[i], STDOUT_FILENO);
		}
		if (argv[i + 1] && !(ft_strequ(argv[i], "[") || ft_strequ(argv[i + 1],
					"]")) && !ft_strequ(argv[i + 1], "%"))
		{
			ft_putstr_fd(" ", STDOUT_FILENO);
		}
		i++;
	}
	if (newline)
		ft_putstr_fd("\n", STDOUT_FILENO);
	sh->last_exit_status = 0;
	return (0);
}