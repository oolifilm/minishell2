/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_null_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julien <julien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 00:05:00 by julien            #+#    #+#             */
/*   Updated: 2025/04/25 00:05:00 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

/* Gère le cas où l'entrée est NULL */
char	*handle_null_input(t_shell *sh, char *line)
{
	if (!sh)
		return (NULL);
	if (!line)
		return (ft_strdup(""));
	return (NULL);
}
