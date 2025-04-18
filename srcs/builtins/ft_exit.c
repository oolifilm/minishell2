/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbanchon <jbanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 14:16:11 by jbanchon          #+#    #+#             */
/*   Updated: 2025/04/18 15:27:16 by jbanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	handle_exit_error(char *s)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(s, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
	exit(2);
}

static long	parse_exit_arg(char *s)
{
	int		i;
	int		sign;
	long	num;

	i = 0;
	sign = 1;
	num = 0;
	if (s[0] == '+' || s[0] == '-')
	{
		if (s[0] == '-')
			sign = -1;
		i++;
	}
	if (s[i] == '\0')
		handle_exit_error(s);
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			handle_exit_error(s);
		num = num * 10 + (s[i] - '0');
		i++;
	}
	return (num * sign);
}

/*
Ce qui nous est démandé dans le sujet : exit with no options
Commande exit : quitter le shell
Utilisation : exit [status]
*/

int	ft_exit(t_shell *sh, char **argv)
{
	long	code;

	printf("exit\n");
	if (!argv[1])
		exit(sh->last_exit_status);
	code = parse_exit_arg(argv[1]);
	if (argv[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		sh->last_exit_status = 1;
		return (1);
	}
	exit((unsigned char)code);
}
