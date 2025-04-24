/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_operators_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julien <julien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 23:45:00 by julien            #+#    #+#             */
/*   Updated: 2025/04/24 23:45:00 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

/* Traite le cas où $? est rencontré */
void	process_exit_status_token(char *input, int *i, t_token_list *tokens)
{
	char	token[1024];
	int		j;

	j = 0;
	token[j++] = '?';
	(*i)++;
	while (input[*i] && !ft_isspace(input[*i]) && input[*i] != '|'
		&& input[*i] != '<' && input[*i] != '>' && input[*i] != '\''
		&& input[*i] != '"')
	{
		token[j++] = input[*i];
		(*i)++;
	}
	token[j] = '\0';
	add_token(tokens, ft_strdup(token), EXIT, NO_QUOTE);
}

/* Traite le cas où $$ est rencontré */
void	process_double_dollar(char *input, int *i, t_token_list *tokens)
{
	(void)input;
	(*i)++;
	add_token(tokens, ft_strdup("$"), STRING, NO_QUOTE);
}

/* Traite le cas où $ est suivi d'un caractère invalide */
void	process_invalid_dollar(char *input, int *i, t_token_list *tokens)
{
	char	token[1024];
	int		j;

	if (!input[*i] || ft_isspace(input[*i]) || input[*i] == '|'
		|| input[*i] == '<' || input[*i] == '>')
	{
		add_token(tokens, ft_strdup("$"), STRING, NO_QUOTE);
	}
	else
	{
		j = 0;
		token[j++] = '$';
		token[j++] = input[*i];
		token[j] = '\0';
		(*i)++;
		add_token(tokens, ft_strdup(token), STRING, NO_QUOTE);
	}
}

/* Traite le cas où $ est suivi d'un nom de variable valide */
void	process_env_variable(char *input, int *i, t_token_list *tokens)
{
	char	token[1024];
	int		j;

	j = 0;
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_') && j < 255)
	{
		token[j++] = input[*i];
		(*i)++;
	}
	token[j] = '\0';
	if (j > 0)
		add_token(tokens, ft_strdup(token), ENV, NO_QUOTE);
	else
		add_token(tokens, ft_strdup("$"), STRING, NO_QUOTE);
}

/* Extrait le nom d'une variable d'environnement à partir d'une chaîne input. */
int	get_env_var_name(char *input, int j, char *var_name)
{
	int	var_len;

	var_len = 0;
	j++;
	while (input[j] && (ft_isalnum(input[j]) || input[j] == '_'))
	{
		var_name[var_len] = input[j];
		var_len++;
		j++;
	}
	var_name[var_len] = '\0';
	return (j);
}
