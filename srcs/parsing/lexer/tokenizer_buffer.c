/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_buffer.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julien <julien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 23:27:32 by julien            #+#    #+#             */
/*   Updated: 2025/04/25 19:36:57 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

/*
** Détermine l'état des guillemets pour un token basé sur son contenu
*/
static t_quote_state	determine_quote_state(char *buffer)
{
	t_quote_state	state;

	state = NO_QUOTE;
	if (ft_strchr(buffer, '\'') == buffer && ft_strrchr(buffer, '\'') == buffer
		+ ft_strlen(buffer) - 1)
		state = SINGLE_QUOTE;
	else if (ft_strchr(buffer, '"') == buffer && ft_strrchr(buffer,
			'"') == buffer + ft_strlen(buffer) - 1)
		state = DOUBLE_QUOTE;
	return (state);
}

/*
** Crée un token à partir d'une chaîne traitée
*/
static t_token	*create_token_from_processed(char *processed, t_quote_state state)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->input = processed;
	token->type = STRING;
	token->quote_state = state;
	token->next = NULL;
	return (token);
}

/*
** Ajoute un token à la liste de tokens
*/
static void	add_token_to_list(t_token_list *tokens, t_token *token)
{
	if (!tokens->head)
	{
		tokens->head = token;
		tokens->cur = token;
	}
	else
	{
		tokens->cur->next = token;
		tokens->cur = token;
	}
	tokens->size++;
}

/*
** Ajoute un token à partir du contenu du buffer
*/
void	add_token_from_buffer(t_token_list *tokens, char *buffer,
		int *buffer_len)
{
	char			*processed;
	t_quote_state	current_quote_state;
	t_token			*token;

	if (*buffer_len <= 0)
		return ;
	buffer[*buffer_len] = '\0';
	processed = remove_quotes(buffer);
	if (!processed)
		return ;
	current_quote_state = determine_quote_state(buffer);
	token = create_token_from_processed(processed, current_quote_state);
	if (!token)
	{
		free(processed);
		return ;
	}
	add_token_to_list(tokens, token);
	*buffer_len = 0;
}

/*
** Traite les caractères guillemets dans le processus de tokenization
** Retourne 1 si le caractère a été traité, 0 sinon
*/
int	handle_quotes_in_tokenizer(char *input, int *i, t_quotes_ctx *ctx)
{
	if (ctx->in_quotes)
	{
		if (input[*i] == ctx->quote_type)
			ctx->in_quotes = 0;
		ctx->buffer[(*ctx->buffer_len)++] = input[*i];
		(*i)++;
		return (1);
	}
	if (input[*i] == '\'' || input[*i] == '"')
	{
		ctx->in_quotes = 1;
		ctx->quote_type = input[*i];
		ctx->buffer[(*ctx->buffer_len)++] = input[*i];
		(*i)++;
		return (1);
	}
	return (0);
}
