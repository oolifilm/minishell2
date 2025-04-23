/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_operators.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leaugust <leaugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 10:59:25 by julien            #+#    #+#             */
/*   Updated: 2025/04/23 15:52:13 by leaugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

/* Gère le caractère |. */

void	assign_pipe(char input, t_token_list *tokens)
{
	if (input == '|')
		add_token(tokens, "|", PIPE, NO_QUOTE);
}

/* Gère le caractère $. */

void	assign_dollar(char *input, int *i, t_token_list *tokens)
{
	char	token[1024];
	int		j;
	int		is_valid_var;

	(*i)++;
	if (input[*i] == '?')
	{
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
		return ;
	}
	if (input[*i] == '$')
	{
		token[0] = '$';
		(*i)++;
		add_token(tokens, ft_strdup("$"), STRING, NO_QUOTE);
		return ;
	}
	is_valid_var = (ft_isalpha(input[*i]) || input[*i] == '_');
	if (!is_valid_var)
	{
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
		return ;
	}
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
