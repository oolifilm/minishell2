/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_operators.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbanchon <jbanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 10:59:25 by julien            #+#    #+#             */
/*   Updated: 2025/04/25 19:48:49 by jbanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

/* Gère le caractère |. */

void	assign_pipe(char input, t_token_list *tokens)
{
	t_token		*new;
	static char	pipe_token[] = "|";

	if (input == '|')
	{
		new = malloc(sizeof(t_token));
		if (!new)
			return ;
		new->input = pipe_token;
		new->type = PIPE;
		new->quote_state = NO_QUOTE;
		new->next = NULL;
		if (!tokens->head)
		{
			tokens->head = new;
			tokens->cur = new;
		}
		else
		{
			tokens->cur->next = new;
			tokens->cur = new;
		}
		tokens->size++;
	}
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
