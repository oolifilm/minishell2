/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   double_quoted.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julien <julien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 10:39:41 by julien            #+#    #+#             */
/*   Updated: 2025/04/22 23:46:24 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

/* Extrait et traite le contenu d'une chaîne entre guillemets. */

static char	*handle_double_quoted(char *input, int *i)
{
	char	*result;
	int		len;

	len = cal_quoted_len(input, *i + 1);
	result = (char *)malloc(sizeof(char) * (len + 1));
	if (!result)
		return (NULL);
	(*i)++;
	fill_quoted_content(input, result, i);
	(*i)++;
	return (result);
}

/* Wrappe handle_double_quoted. */

char	*expand_var_in_dquotes(char *str)
{
	int		i;
	char	*result;

	i = 0;
	result = handle_double_quoted(str, &i);
	return (result);
}
