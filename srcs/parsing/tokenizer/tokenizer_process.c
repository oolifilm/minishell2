/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_process.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leaugust <leaugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 23:32:15 by julien            #+#    #+#             */
/*   Updated: 2025/04/24 12:43:10 by leaugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

/*
** Initialise les variables nécessaires pour le traitement des tokens
*/
static void	init_process_vars(char *input, size_t *len, int *buffer_len,
		t_quotes_ctx *quotes_ctx, char *buffer)
{
	*len = ft_strlen(input);
	*buffer_len = 0;
	quotes_ctx->in_quotes = 0;
	quotes_ctx->quote_type = 0;
	quotes_ctx->buffer = buffer;
	quotes_ctx->buffer_len = buffer_len;
	buffer[0] = '\0';
}

/*
** Traite un seul caractère dans le processus de tokenization
*/
static int	process_single_char(char *input, int *i, t_token_list *tokens,
		char *buffer, int *buffer_len, t_quotes_ctx *quotes_ctx)
{
	if (handle_quotes_in_tokenizer(input, i, quotes_ctx))
		return (0);
	if (input[*i] == '$')
		process_dollar_char(input, i, tokens, buffer, buffer_len);
	else if (input[*i] == '|')
		process_pipe_char(input, i, tokens, buffer, buffer_len);
	else if (input[*i] == '<' || input[*i] == '>')
	{
		if (process_redir_char(input, i, tokens, buffer, buffer_len))
			return (1); // erreur
	}
	else if (ft_isspace(input[*i]))
		process_space_char(input, i, tokens, buffer, buffer_len);
	else
		process_normal_char(input, i, buffer, buffer_len);
	return (0);
}

/*
** Traite tous les caractères d'une chaîne d'entrée pour les convertir en tokens
*/
int	process_tokens(char *input, t_token_list *tokens, int *i,
		int *is_first_word)
{
	size_t			len;
	char			buffer[1024];
	int				buffer_len;
	t_quotes_ctx	quotes_ctx;

	(void)is_first_word;
	init_process_vars(input, &len, &buffer_len, &quotes_ctx, buffer);
	while (*i < (int)len)
	{
		if (process_single_char(input, i, tokens, buffer, &buffer_len,
				&quotes_ctx))
			return (1);
	}
	if (buffer_len > 0)
		add_token_from_buffer(tokens, buffer, &buffer_len);
	return (0);
}
