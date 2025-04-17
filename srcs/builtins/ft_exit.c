/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbanchon <jbanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 14:16:11 by jbanchon          #+#    #+#             */
/*   Updated: 2025/04/17 15:29:41 by jbanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static long	parse_exit_arg(char *s)
{
	int		i;
	int		sign;
	long	num;

	i = 0;
	sign = 1;
	if (s[0] == '+' || s[0] == '-')
	{
		if (s[0] == '-')
			sign = -1;
		i++;
	}
	if (s[i] == '\0')
	{
		printf("exit: %s: numeric argument required\n", s);
		exit(255);
	}
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
		{
			printf("exit: %s: numeric argument required\n", s);
			exit(255);
		}
		num = num * 10 + (s[i++] - '0');
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
		printf("exit : too many arguments\n");
		sh->last_exit_status = 1;
		return (1);
	}
	exit((int)(code & 0xFF));
}
