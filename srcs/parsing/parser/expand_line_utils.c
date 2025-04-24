/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_line_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julien <julien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 23:55:00 by julien            #+#    #+#             */
/*   Updated: 2025/04/24 23:55:00 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

/* Réalloue la mémoire pour le résultat si nécessaire */
char	*realloc_result(char *result, int j, int expanded_len, char *line)
{
	char	*new_result;
	int		new_size;

	new_size = j + expanded_len + ft_strlen(line) + 1;
	new_result = (char *)malloc(sizeof(char) * new_size);
	if (!new_result)
		return (NULL);
	ft_strncpy(new_result, result, j);
	free(result);
	return (new_result);
}

/* Copie la variable expandée dans le résultat */
char	*copy_expanded(char *result, char *expanded, int *j)
{
	int	i;

	i = 0;
	while (expanded[i])
		result[(*j)++] = expanded[i++];
	free(expanded);
	return (result);
}

/* Vérifie si un caractère $ est suivi d'une variable valide */
int	check_dollar_var(char c, char next)
{
	return (c == '$' && (ft_isalnum(next) || next == '_' || next == '?'));
}

/* Initialise le résultat pour l'expansion */
char	*init_expand_result(char *line, int *result_size)
{
	char	*result;

	*result_size = ft_strlen(line) * 2;
	result = (char *)malloc(sizeof(char) * (*result_size));
	if (!result)
		return (NULL);
	return (result);
}

/* Gère le cas où l'entrée est NULL */
char	*handle_null_input(t_shell *sh, char *line)
{
	if (!sh)
		return (NULL);
	if (!line)
		return (ft_strdup(""));
	return (NULL);
}
