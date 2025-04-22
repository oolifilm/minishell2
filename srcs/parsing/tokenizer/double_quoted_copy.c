/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   double_quoted_copy.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julien <julien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 23:42:15 by julien            #+#    #+#             */
/*   Updated: 2025/04/22 23:42:15 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

/*
** Traite un caractère normal (non-spécial) dans l'entrée
*/
static void	process_dquoted_normal_char(char *input, char *result, int *j, int *len)
{
	result[(*len)++] = input[(*j)++];
}

/*
** Traite l'expansion des variables dans une chaîne entre guillemets doubles
*/
static void	process_expansion(char *input, char *result, int *j, int *len)
{
	(*j)++;
	if (input[*j] == '?')
		process_question_mark(j, len, result);
	else if (ft_isalpha(input[*j]) || input[*j] == '_')
		process_env_var(input, j, len, result);
	else
		process_dquoted_dollar(input, j, len, result);
}

/*
** Remplit result avec le contenu de input en gérant les expansions de variables
*/
void	copy_quoted_and_expand(char *input, char *result, int *j, int *len)
{
	while (input[*j] && input[*j] != '"')
	{
		if (input[*j] == '$' && input[*j + 1])
			process_expansion(input, result, j, len);
		else
			process_dquoted_normal_char(input, result, j, len);
	}
}

/*
** Remplit le buffer result avec le contenu traité entre guillemets
*/
void	fill_quoted_content(char *input, char *result, int *i)
{
	int		j;
	int		len;

	j = *i + 1;
	len = 0;
	copy_quoted_and_expand(input, result, &j, &len);
	result[len] = '\0';
	*i = j;
}
