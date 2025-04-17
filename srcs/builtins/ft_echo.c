/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leaugust <leaugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 14:14:58 by jbanchon          #+#    #+#             */
/*   Updated: 2025/04/17 11:58:34 by leaugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


static int is_echo_option_n(char *str)
{
	int i;

	i = 1;
	if (!str || str[0] != '-' || str[1] != 'n')
		return (0);
	while (str[i] == 'n')
		i++;
	return (str[i] == '\0');
}

/*
Ce qui nous est démandé dans le sujet : echo with option -n
=================================================================================
Commande echo : Doit écrire les entrées sur la sortie standard et avec l'option
Option -n :
	-n n'affiche pas la nouvelle ligne à la fin. Sinon affiche un retour à la ligne.
=================================================================================
Utilisation UNIX de la commande echo : echo [OPTIONS] [TEXTE].
*/

int	ft_echo(char **argv)
{
	int	i;
	int	newline;

	if (argv[0] && ft_strcmp(argv[0], "echo") == 0)
		i = 1;
	else
		i = 0;
	newline = 1;
	while (argv[i] && is_echo_option_n(argv[i]))
	{
		newline = 0;
		i++;
	}
	while (argv[i])
	{
		printf("%s", argv[i]);
		if (argv[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	return (0);
}
