/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_chars.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leaugust <leaugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 23:28:15 by julien            #+#    #+#             */
/*   Updated: 2025/04/24 21:45:00 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	process_dollar_char_legacy(t_token_ctx *ctx)
{
	t_redir_ctx	redir_ctx;

	redir_ctx.buffer = ctx->buffer;
	redir_ctx.buffer_len = ctx->buffer_len;
	redir_ctx.tokens = ctx->tokens;
	process_dollar_char(ctx->input, ctx->i, &redir_ctx);
}

void	process_pipe_char_legacy(t_token_ctx *ctx)
{
	t_redir_ctx	redir_ctx;

	redir_ctx.buffer = ctx->buffer;
	redir_ctx.buffer_len = ctx->buffer_len;
	redir_ctx.tokens = ctx->tokens;
	process_pipe_char(ctx->input, ctx->i, &redir_ctx);
}

int	process_redir_char_legacy(t_token_ctx *ctx)
{
	t_redir_ctx	redir_ctx;

	redir_ctx.buffer = ctx->buffer;
	redir_ctx.buffer_len = ctx->buffer_len;
	redir_ctx.tokens = ctx->tokens;
	return (process_redir_char(ctx->input, ctx->i, &redir_ctx));
}

void	process_space_char_legacy(t_token_ctx *ctx)
{
	t_redir_ctx	redir_ctx;

	redir_ctx.buffer = ctx->buffer;
	redir_ctx.buffer_len = ctx->buffer_len;
	redir_ctx.tokens = ctx->tokens;
	process_space_char(ctx->input, ctx->i, &redir_ctx);
}

void	process_normal_char_legacy(t_token_ctx *ctx)
{
	t_buffer_ctx	buffer_ctx;

	buffer_ctx.buffer = ctx->buffer;
	buffer_ctx.buffer_len = ctx->buffer_len;
	process_normal_char(ctx->input, ctx->i, &buffer_ctx);
}
