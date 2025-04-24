/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julien <julien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 15:27:16 by leaugust          #+#    #+#             */
/*   Updated: 2025/04/24 23:55:00 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

/* Prototypes des fonctions déplacées dans les fichiers de validation */
int			is_invalid_first_token(t_token *head);
int			handle_pipes(t_token *tokens);
int			has_invalid_redirection(t_token *tokens);
int			has_unclosed_quote(char *input);

/* Vérifie la validité des tokens et exécute les heredocs si nécessaire */
static int	validate_tokens(t_shell *sh, t_token_list *tokens)
{
	if (is_invalid_first_token(tokens->head))
	{
		set_exit_code(sh, 2);
		return (0);
	}
	if (handle_pipes(tokens->head))
	{
		set_exit_code(sh, 2);
		return (0);
	}
	if (has_invalid_redirection(tokens->head))
	{
		set_exit_code(sh, 2);
		return (0);
	}
	if (handle_heredocs(sh, tokens->head))
	{
		set_exit_code(sh, 2);
		return (0);
	}
	return (1);
}

/* Analyse les tokens et vérifie leur validité */
int	parse_tokens(t_shell *sh, t_token_list *tokens)
{
	if (!tokens || !tokens->head)
	{
		set_exit_code(sh, 0);
		return (0);
	}
	return (validate_tokens(sh, tokens));
}
