/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julien <julien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 11:02:26 by julien            #+#    #+#             */
/*   Updated: 2025/04/24 23:31:28 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

/* Prototypes des fonctions dans token_command_utils.c */
bool	is_special_char(char c);
void	process_exit_status_with_text(char *input, int *i,
			t_token_list *tokens);
void	process_variable_name(char *input, int *i, char *var_name, int *j);
void	process_exit_status(char *input, int *i, t_token_list *tokens);

/* Extrait un mot de l'entrée `input` et le
classe en fonction de son rôle dans la commande. */

void	token_is_command(char *input, int *i, t_token_list *tokens,
		int *is_first_word)
{
	char	temp[256];
	int		j;

	j = 0;
	while (input[*i] && !is_special_char(input[*i]) && j < 255)
		temp[j++] = input[*(i)++];
	temp[j] = '\0';
	if (j > 0)
	{
		if (tokens->cur && tokens->cur->type == PIPE)
			*is_first_word = 1;
		if (*is_first_word)
		{
			add_token(tokens, temp, CMD, NO_QUOTE);
			*is_first_word = 0;
		}
		else if (temp[0] == '-')
			add_token(tokens, temp, ARG, NO_QUOTE);
		else
			add_token(tokens, temp, STRING, NO_QUOTE);
	}
}

/* Extrait une variable d'environnement à partir
de l'entrée input lorsqu'un $ est rencontré. */

void	assign_env_var(char *input, int *i, t_token_list *tokens)
{
	char	var_name[256];
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
		process_variable_name(input, i, var_name, &j);
	if (is_exit_status)
	{
		process_exit_status(input, i, tokens);
		return ;
	}
	if (j > 0)
		add_token(tokens, var_name, ENV, NO_QUOTE);
}
