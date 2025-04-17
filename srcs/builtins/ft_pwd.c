/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbanchon <jbanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 14:16:15 by jbanchon          #+#    #+#             */
/*   Updated: 2025/04/17 16:12:25 by jbanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <errno.h>

/*
Sujet : pwd with no options
======
Commande pwd : afficher le répertoire courant
Utilisation : pwd
======
getcwd = obtenir le répertoire courant
strerror = Retourner une string d'erreur spécifique à la fonction getcwd
*/

int	ft_pwd(t_shell *sh, char **argv)
{
	char	buffer[4096];

	(void)sh;
	(void)argv;
	if (!getcwd(buffer, sizeof(buffer)))
	{
		ft_putstr_fd("pwd: ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		return (1);
	}
	printf("%s\n", buffer);
	return (0);
}
