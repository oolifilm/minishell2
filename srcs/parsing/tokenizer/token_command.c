/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leaugust <leaugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 11:02:26 by julien            #+#    #+#             */
/*   Updated: 2025/04/23 15:51:44 by leaugust         ###   ########.fr       */
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
	char	full_token[1024];
	int		j;
	int		is_exit_status;

	j = 0;
	(*i)++;
	is_exit_status = 0;
	if (input[*i] == '?')
	{
		var_name[j++] = '?';
		(*i)++;
		is_exit_status = 1;
	}
	else
	{
		while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
		{
			var_name[j++] = input[*i];
			(*i)++;
		}
	}
	var_name[j] = '\0';
	// Si on a juste $? sans texte après, on crée un token simple
	if (is_exit_status && !input[*i])
	{
		add_token(tokens, ft_strdup("?"), EXIT, NO_QUOTE);
		return ;
	}
	// Si on a $? suivi de texte, on crée un token combiné
	if (is_exit_status)
	{
		// Capture le reste du mot jusqu'au prochain séparateur
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
		return ;
	}
	// Cas normal pour les variables d'environnement
	if (j > 0)
		add_token(tokens, ft_strdup(var_name), ENV, NO_QUOTE);
}
