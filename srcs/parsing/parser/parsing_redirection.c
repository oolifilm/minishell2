/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_redirection.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julien <julien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 23:55:00 by julien            #+#    #+#             */
/*   Updated: 2025/04/24 23:55:00 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

/* Vérifie si une redirection est suivie d'une cible valide */
static int	check_redirection_target(t_token *token)
{
	if (!token->next)
	{
		printf("[ERROR] Redirection must be followed by a target.\n");
		return (1);
	}
	if (token->next->type == REDIR_IN || token->next->type == REDIR_OUT
		|| token->next->type == APPEND || token->next->type == HEREDOC)
	{
		printf("[ERROR] Invalid redirection: cannot chain symbols.\n");
		return (1);
	}
	if (token->next->type != REDIR_FILE && token->next->type != STRING
		&& token->next->type != ENV)
	{
		printf("[ERROR] Unexpected token after redirection.\n");
		return (1);
	}
	return (0);
}

/* Vérifie si un heredoc est suivi d'un délimiteur valide */
static int	check_heredoc_delimiter(t_token *token)
{
	if (token->type == HEREDOC && (!token->next || token->next->type != STRING))
	{
		printf("[ERROR] Heredoc must be followed by a delimiter.\n");
		return (1);
	}
	return (0);
}

/* Vérifie la validité des redirections dans la ligne de commande */
int	has_invalid_redirection(t_token *tokens)
{
	while (tokens)
	{
		if (tokens->type == REDIR_IN || tokens->type == REDIR_OUT
			|| tokens->type == APPEND || tokens->type == HEREDOC)
		{
			if (check_redirection_target(tokens))
				return (1);
			if (check_heredoc_delimiter(tokens))
				return (1);
		}
		tokens = tokens->next;
	}
	return (0);
}

/* Vérifie si une chaîne contient des guillemets non fermés */
int	has_unclosed_quote(char *input)
{
	int	in_single_quote;
	int	in_double_quote;

	in_single_quote = 0;
	in_double_quote = 0;
	while (*input)
	{
		if (*input == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (*input == '"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		input++;
	}
	if (in_single_quote || in_double_quote)
		return (1);
	return (0);
}
