/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbanchon <jbanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 11:02:26 by julien            #+#    #+#             */
/*   Updated: 2025/04/17 18:26:34 by jbanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

/* Détermine si un caractère est un caractère
spécial dans l'analyse de la commande. */

static bool	is_special_char(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '$' || c == '>'
		|| c == '<' || c == '\'' || c == '\"');
}

/* Extrait un mot de l'entrée `input` et le
classe en fonction de son rôle dans la commande. */

void	token_is_command(char *input, int *i, t_token_list *tokens,
		int *is_first_word)
{
	char	temp[256];
	int		j;
	char	*value;

	j = 0;
	while (input[*i] && !is_special_char(input[*i]) && j < 255)
		temp[j++] = input[*(i)++];
	temp[j] = '\0';
	if (j > 0)
	{
		value = ft_strdup(temp);
		if (!value)
			return ;
		if (tokens->cur && tokens->cur->type == PIPE)
			*is_first_word = 1;
		if (*is_first_word)
		{
			add_token(tokens, value, CMD, NO_QUOTE);
			*is_first_word = 0;
		}
		else if (temp[0] == '-')
			add_token(tokens, value, ARG, NO_QUOTE);
		else
			add_token(tokens, value, STRING, NO_QUOTE);
	}
}

/* Extrait une variable d'environnement à partir
de l'entrée input lorsqu'un $ est rencontré. */

void	assign_env_var(char *input, int *i, t_token_list *tokens)
{
	char	var_name[256];
	int		j;

	j = 0;
	(*i)++;
	if (input[*i] == '?')
	{
		add_token(tokens, ft_strdup("?"), EXIT, NO_QUOTE);
		(*i)++;
		return ;
	}
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
	{
		var_name[j++] = input[*i];
		(*i)++;
	}
	var_name[j] = '\0';
	if (j > 0)
		add_token(tokens, ft_strdup(var_name), ENV, NO_QUOTE);
}
