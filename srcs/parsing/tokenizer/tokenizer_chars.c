/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_chars.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julien <julien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 23:28:15 by julien            #+#    #+#             */
/*   Updated: 2025/04/23 01:10:17 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

/*
** Traite le caractère $ dans le processus de tokenization
*/
void	process_dollar_char(char *input, int *i, t_token_list *tokens,
		char *buffer, int *buffer_len)
{
	int		dollar_pos;
	
	dollar_pos = *i;
	if (*buffer_len > 0)
		add_token_from_buffer(tokens, buffer, buffer_len);
	(*i)++;
	if (input[*i] && (input[*i] == '?' || input[*i] == ':' || input[*i] == '='
		|| !ft_isspace(input[*i])))
	{
		while (input[*i] && !ft_isspace(input[*i]) && input[*i] != '|' 
			&& input[*i] != '<' && input[*i] != '>' && input[*i] != '$')
			(*i)++;
	}
	*i = dollar_pos;
	assign_dollar(input, i, tokens);
}

/*
** Traite le caractère | dans le processus de tokenization
*/
void	process_pipe_char(char *input, int *i, t_token_list *tokens,
		char *buffer, int *buffer_len)
{
	if (*buffer_len > 0)
		add_token_from_buffer(tokens, buffer, buffer_len);
	assign_pipe(input[*i], tokens);
	(*i)++;
}

/*
** Traite les caractères < et > dans le processus de tokenization
*/
void	process_redir_char(char *input, int *i, t_token_list *tokens,
		char *buffer, int *buffer_len)
{
	if (*buffer_len > 0)
		add_token_from_buffer(tokens, buffer, buffer_len);
	assign_redirection(input, i, tokens);
}

/*
** Traite les espaces dans le processus de tokenization
*/
void	process_space_char(char *input, int *i, t_token_list *tokens,
		char *buffer, int *buffer_len)
{
	if (*buffer_len > 0)
		add_token_from_buffer(tokens, buffer, buffer_len);
	skip_spaces(input, i);
}

/*
** Traite les caractères normaux dans le processus de tokenization
*/
void	process_normal_char(char *input, int *i, char *buffer, int *buffer_len)
{
	buffer[(*buffer_len)++] = input[*i];
	(*i)++;
}
