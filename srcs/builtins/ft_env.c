/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbanchon <jbanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 14:16:09 by jbanchon          #+#    #+#             */
/*   Updated: 2025/04/10 12:41:54 by jbanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
Sujet : env with no options
======
Commande env : afficher toutes les variables d'environnement
Utilisation : env
======
envp = tableau qui contient toutes les variables d'environnement
*/

int	ft_env(t_shell *sh, char **argv)
{
	int	i;

	i = 0;
	if (argv[1])
	{
		printf("[ERROR] env: no options\n");
		return (1);
	}
	while (sh->env[i])
	{
		printf("%s\n", sh->env[i]);
		i++;
	}
	return (0);
}
