/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julien <julien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 14:16:16 by jbanchon          #+#    #+#             */
/*   Updated: 2025/04/24 22:50:00 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
Ce qui nous est démandé dans le sujet : unset with no options
=================================================================================
Commande unset : supprimer une variable d'environnement
Utilisation : unset [variable]
*/

int	ft_unset(t_shell *sh, char **argv)
{
	int	i;
	int	ret;

	i = 1;
	ret = 0;
	while (argv[i])
	{
		if (!is_valid_identifier(argv[i]))
		{
			ft_putstr_fd("unset: `", STDERR_FILENO);
			ft_putstr_fd(argv[i], STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			ret = 1;
		}
		else
			remove_env_var(sh, argv[i]);
		i++;
	}
	return (ret);
}
