/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_operators.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julien <julien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 10:59:25 by julien            #+#    #+#             */
/*   Updated: 2025/04/24 23:50:00 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

/* Gère le caractère |. */

void	assign_pipe(char input, t_token_list *tokens)
{
	if (input == '|')
		add_token(tokens, "|", PIPE, NO_QUOTE);
}

/* Prototypes des fonctions dans token_operators_utils.c */
void	process_exit_status_token(char *input, int *i, t_token_list *tokens);
void	process_double_dollar(char *input, int *i, t_token_list *tokens);
void	process_invalid_dollar(char *input, int *i, t_token_list *tokens);
void	process_env_variable(char *input, int *i, t_token_list *tokens);

/* Gère le caractère $. */
void	assign_dollar(char *input, int *i, t_token_list *tokens)
{
	int	is_valid_var;

	(*i)++;
	if (input[*i] == '?')
	{
		process_exit_status_token(input, i, tokens);
		return ;
	}
	if (input[*i] == '$')
	{
		process_double_dollar(input, i, tokens);
		return ;
	}
	is_valid_var = (ft_isalpha(input[*i]) || input[*i] == '_');
	if (!is_valid_var)
	{
		process_invalid_dollar(input, i, tokens);
		return ;
	}
	process_env_variable(input, i, tokens);
}

/* Extrait le nom d'une variable d'environnement à partir d'une chaîne input. */
int		get_env_var_name(char *input, int j, char *var_name);
