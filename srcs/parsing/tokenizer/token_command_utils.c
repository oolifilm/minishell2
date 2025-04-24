/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_command_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julien <julien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 23:35:00 by julien            #+#    #+#             */
/*   Updated: 2025/04/24 23:35:00 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

/* Détermine si un caractère est un caractère
spécial dans l'analyse de la commande. */

bool	is_special_char(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '$' || c == '>'
		|| c == '<' || c == '\'' || c == '\"');
}

/* Traite une variable d'environnement de type $?
avec du texte après (ex: $?abc) */

void	process_exit_status_with_text(char *input, int *i, t_token_list *tokens)
{
	char	full_token[1024];
	int		j;

	j = 0;
	ft_strlcpy(full_token, "?", sizeof(full_token));
	while (input[*i] && !is_special_char(input[*i]))
	{
		full_token[j + 1] = input[*i];
		j++;
		(*i)++;
	}
	full_token[j + 1] = '\0';
	add_token(tokens, ft_strdup(full_token), EXIT, NO_QUOTE);
}

/* Extrait le nom d'une variable d'environnement */

void	process_variable_name(char *input, int *i, char *var_name, int *j)
{
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
	{
		var_name[(*j)++] = input[*i];
		(*i)++;
	}
	var_name[*j] = '\0';
}

/* Traite une variable d'environnement de type $? */

void	process_exit_status(char *input, int *i, t_token_list *tokens)
{
	if (!input[*i])
	{
		add_token(tokens, ft_strdup("?"), EXIT, NO_QUOTE);
		return ;
	}
	process_exit_status_with_text(input, i, tokens);
}
