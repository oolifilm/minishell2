/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_process.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julien <julien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 23:32:15 by julien            #+#    #+#             */
/*   Updated: 2025/04/24 12:43:10 by leaugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static void	init_process_vars(t_process_ctx *ctx)
{
	*(ctx->len) = ft_strlen(ctx->input);
	*(ctx->buffer_len) = 0;
	ctx->quotes_ctx->in_quotes = 0;
	ctx->quotes_ctx->quote_type = 0;
	ctx->quotes_ctx->buffer = ctx->buffer;
	ctx->quotes_ctx->buffer_len = ctx->buffer_len;
	ctx->buffer[0] = '\0';
}

static int	process_single_char(char *input, int *i, t_token_list *tokens,
		t_process_ctx *ctx)
{
	t_token_ctx	token_ctx;

	token_ctx.input = input;
	token_ctx.i = i;
	token_ctx.tokens = tokens;
	token_ctx.buffer = ctx->buffer;
	token_ctx.buffer_len = ctx->buffer_len;
	if (handle_quotes_in_tokenizer(input, i, ctx->quotes_ctx))
		return (0);
	if (input[*i] == '$')
		process_dollar_char_legacy(&token_ctx);
	else if (input[*i] == '|')
		process_pipe_char_legacy(&token_ctx);
	else if (input[*i] == '<' || input[*i] == '>')
	{
		if (process_redir_char_legacy(&token_ctx))
			return (1);
	}
	else if (ft_isspace(input[*i]))
		process_space_char_legacy(&token_ctx);
	else
		process_normal_char_legacy(&token_ctx);
	return (0);
}

int	process_tokens(char *input, t_token_list *tokens, int *i,
		int *is_first_word)
{
	size_t			len;
	char			buffer[1024];
	int				buffer_len;
	t_quotes_ctx	quotes_ctx;
	t_process_ctx	ctx;

	(void)is_first_word;
	ctx.input = input;
	ctx.len = &len;
	ctx.buffer_len = &buffer_len;
	ctx.quotes_ctx = &quotes_ctx;
	ctx.buffer = buffer;
	init_process_vars(&ctx);
	while (*i < (int)len)
	{
		if (process_single_char(input, i, tokens, &ctx))
			return (1);
	}
	if (buffer_len > 0)
		add_token_from_buffer(tokens, buffer, &buffer_len);
	return (0);
}
