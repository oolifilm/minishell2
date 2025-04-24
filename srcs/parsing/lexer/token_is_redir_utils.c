/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_is_redir_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julien <julien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 23:02:13 by julien            #+#    #+#             */
/*   Updated: 2025/04/24 23:02:13 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

/* Extrait le nom d'un fichier après un opérateur de redirection (>>, <<). */

void	get_redir_target(char *input, int *i, t_token_list *tokens,
		t_token_type redir_type)
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
			if (redir_type == HEREDOC)
				add_token(tokens, target, STRING, NO_QUOTE);
			else
				add_token(tokens, target, REDIR_FILE, NO_QUOTE);
			free(target);
		}
	}
}
