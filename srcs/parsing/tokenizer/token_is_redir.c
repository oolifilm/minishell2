/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_is_redir.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leaugust <leaugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 23:09:13 by julien            #+#    #+#             */
/*   Updated: 2025/04/24 13:30:16 by leaugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

/* Extrait le nom d’un fichier après un opérateur de redirection (>>, <<). */

static void	get_redir_target(char *input, int *i, t_token_list *tokens, t_token_type redir_type)
{
	int		start;
	char	*target;

	(*i)++;
	while (input[*i] && input[*i] == ' ')
		(*i)++;
	start = *i;
	while (input[*i] && input[*i] != ' ' && input[*i] != '<' && input[*i] != '>'
		&& input[*i] != '|')
		(*i)++;
	if (*i > start)
	{
		target = ft_substr(input, start, *i - start);
		if (target)
		{
			// On distingue heredoc du reste
			if (redir_type == HEREDOC)
				add_token(tokens, target, STRING, NO_QUOTE);  // ou DELIMITER si tu veux être plus explicite
			else
				add_token(tokens, target, REDIR_FILE, NO_QUOTE);
			free(target);
		}
	}
}


/* Gère les redirections de sortie (> et >>). */

int	handle_output_redir(char *input, int *i, t_token_list *tokens)
{
	char	temp[3] = {'\0', '\0', '\0'};

	if (input[*i + 1] && input[*i + 2] && input[*i + 1] == '>' && input[*i + 2] == '>')
	{
		printf("[ERROR] Redirection must be followed by a valid target.\n");
		*i = ft_strlen(input);
		return (1);
	}
	if (input[*i + 1] == '>')
	{
		temp[0] = input[*i];
		temp[1] = input[*i + 1];
		add_token(tokens, temp, APPEND, NO_QUOTE);
		(*i)++;
		get_redir_target(input, i, tokens, APPEND);
	}
	else
	{
		temp[0] = input[*i];
		add_token(tokens, temp, REDIR_OUT, NO_QUOTE);
		get_redir_target(input, i, tokens, REDIR_OUT);
	}
	return (0);
}



/* Gère les redirections d'entrée (< et <<). */

int handle_input_redir(char *input, int *i, t_token_list *tokens)
{
	char	temp[3];

	temp[0] = '\0';
	temp[1] = '\0';
	temp[2] = '\0';

	if (input[*i + 1] == '<')
	{
		if (input[*i + 2] == '<') // triple chevrons ?
		{
			printf("[ERROR] Redirection must be followed by a valid target.\n");
			*i = ft_strlen(input);
			return (1);
		}
		temp[0] = input[*i];
		temp[1] = input[*i + 1];
		add_token(tokens, temp, HEREDOC, NO_QUOTE);
		(*i)++;
		get_redir_target(input, i, tokens, HEREDOC); // ← changement ici
	}
	else
	{
		temp[0] = input[*i];
		add_token(tokens, temp, REDIR_IN, NO_QUOTE);
		get_redir_target(input, i, tokens, REDIR_IN); // ← changement ici
	}
	return (0);
}

/* Détecte si le caractère actuel dans input est un symbole de redirection. */

int	assign_redirection(char *input, int *i, t_token_list *tokens)
{
	if (input[*i] == '>')
		return handle_output_redir(input, i, tokens);
	else if (input[*i] == '<')
		return handle_input_redir(input, i, tokens);
	return (0);
}

