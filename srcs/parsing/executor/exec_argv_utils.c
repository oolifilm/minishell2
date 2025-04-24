/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_argv_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julien <julien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 00:10:00 by julien            #+#    #+#             */
/*   Updated: 2025/04/25 00:10:00 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

/* Détermine si un token doit être ignoré */
int	should_skip_token(t_token *cur, int *skip_next)
{
	if (*skip_next)
	{
		*skip_next = 0;
		return (1);
	}
	if (cur->type == HEREDOC)
	{
		*skip_next = 1;
		return (1);
	}
	return (0);
}

/* Vérifie si un token est un argument */
int	is_arg_token(t_token *cur)
{
	return (cur->type == STRING || cur->type == ARG || cur->type == ENV
		|| cur->type == EXIT);
}

/* Récupère la valeur d'un argument */
char	*get_arg_value(t_shell *sh, t_token *cur)
{
	if (cur->quote_state == SINGLE_QUOTE)
		return (ft_strdup(cur->input));
	else
		return (expand_token(sh, cur));
}

/* Compte le nombre d'arguments */
int	count_args(t_token *token)
{
	int		count;
	t_token	*cur;

	count = 1;
	cur = token->next;
	while (cur)
	{
		if (is_arg_token(cur))
			count++;
		cur = cur->next;
	}
	return (count);
}
