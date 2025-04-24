/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julien <julien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 11:00:31 by julien            #+#    #+#             */
/*   Updated: 2025/04/24 23:43:24 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

/* Prototypes des fonctions déplacées dans token_quotes_utils.c */
char	*remove_quotes(const char *s);
void	handle_single_quote(char *input, int *i, t_token_list *tokens);

/* Prototype de la fonction déplacée dans token_quotes_dquote.c */
void	handle_double_quotes(char *input, int *i, t_token_list *tokens);

/* Gère le contenu entre guillemets et apostrophes dans l'entrée input. */
void	handle_quoted_content(char *input, int *i, t_token_list *tokens,
		char quote_type)
{
	if (quote_type == '"')
		handle_double_quotes(input, i, tokens);
	else
		handle_single_quote(input, i, tokens);
}
