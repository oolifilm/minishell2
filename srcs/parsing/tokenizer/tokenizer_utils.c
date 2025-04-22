/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julien <julien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 23:31:34 by julien            #+#    #+#             */
/*   Updated: 2025/04/22 23:31:34 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

/*
** Vérifie si un caractère doit être ignoré
*/
int	is_ignored_char(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == ':' || c == '!');
}

/*
** Vérifie si une ligne d'entrée est entièrement composée de caractères ignorables
*/
int	is_ignorable_input(const char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (!is_ignored_char(line[i]))
			return (0);
		i++;
	}
	return (1);
}

/*
** Prépare les structures pour le tokenizing
*/
int	init_tokenizer(char *input, t_token_list **tokens, int *i)
{
	if (has_unclosed_quote(input))
	{
		printf("[ERROR] Lexer found an unclosed quote.\n");
		return (0);
	}
	*tokens = init_token_list();
	*i = 0;
	skip_spaces(input, i);
	return (1);
}
