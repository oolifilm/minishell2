/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_validation.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julien <julien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 23:50:00 by julien            #+#    #+#             */
/*   Updated: 2025/04/24 23:50:00 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

/* Vérifie si le premier token est valide */
int	is_invalid_first_token(t_token *head)
{
	if (head->input && (head->input[0] == ';' || head->input[0] == '&'
			|| head->input[0] == '(' || head->input[0] == ')'))
	{
		printf("[ERROR] ;, & and () can't be the first argument.\n");
		return (1);
	}
	if (head->type == PIPE)
	{
		printf("[ERROR] Pipe cannot be the first token.\n");
		return (1);
	}
	if (head->type == REDIR_IN || head->type == REDIR_OUT
		|| head->type == APPEND || head->type == HEREDOC)
	{
		if (!head->next || (head->next->type != STRING
				&& head->next->type != REDIR_FILE && head->next->type != ENV))
		{
			printf("[ERROR] Redirection must be followed by a target.\n");
			return (1);
		}
	}
	return (0);
}

/* Vérifie si une chaîne contient un pipe non quoté */
static int	check_unquoted_pipe(t_token *tokens)
{
	while (tokens)
	{
		if (tokens->type == STRING)
		{
			if (!tokens->quote_state && ft_strchr(tokens->input, '|'))
			{
				printf("[ERROR] Pipe found in unquoted string.\n");
				return (1);
			}
		}
		tokens = tokens->next;
	}
	return (0);
}

/* Vérifie si un pipe est suivi d'une commande valide */
static int	check_pipe_followed_by_cmd(t_token *tokens)
{
	while (tokens)
	{
		if (tokens->type == PIPE)
		{
			if (!tokens->next)
			{
				printf("[ERROR] Pipe must be followed by a command.\n");
				return (1);
			}
			else if (tokens->next->type == PIPE)
			{
				printf("[ERROR] Found consecutive pipes.\n");
				return (1);
			}
			if (tokens->next->type != CMD)
			{
				printf("[ERROR] Pipe must be followed by a command.\n");
				return (1);
			}
		}
		tokens = tokens->next;
	}
	return (0);
}

/* Vérifie la validité des pipes dans la ligne de commande */
int	handle_pipes(t_token *tokens)
{
	if (check_unquoted_pipe(tokens))
		return (1);
	if (check_pipe_followed_by_cmd(tokens))
		return (1);
	return (0);
}
