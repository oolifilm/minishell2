/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_chars_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julien <julien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 21:40:00 by julien            #+#    #+#             */
/*   Updated: 2025/04/24 21:40:00 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	process_dollar_char(char *input, int *i, t_redir_ctx *ctx)
{
	int	dollar_pos;

	dollar_pos = *i;
	if (*ctx->buffer_len > 0)
		add_token_from_buffer(ctx->tokens, ctx->buffer, ctx->buffer_len);
	(*i)++;
	if (input[*i] && (input[*i] == '?' || input[*i] == ':' || input[*i] == '='
			|| !ft_isspace(input[*i])))
	{
		while (input[*i] && !ft_isspace(input[*i]) && input[*i] != '|'
			&& input[*i] != '<' && input[*i] != '>' && input[*i] != '$')
			(*i)++;
	}
	*i = dollar_pos;
	assign_dollar(input, i, ctx->tokens);
}

void	process_pipe_char(char *input, int *i, t_redir_ctx *ctx)
{
	if (*ctx->buffer_len > 0)
		add_token_from_buffer(ctx->tokens, ctx->buffer, ctx->buffer_len);
	assign_pipe(input[*i], ctx->tokens);
	(*i)++;
}

int	process_redir_char(char *input, int *i, t_redir_ctx *ctx)
{
	if (*ctx->buffer_len > 0)
		add_token_from_buffer(ctx->tokens, ctx->buffer, ctx->buffer_len);
	return (assign_redirection(input, i, ctx->tokens));
}

void	process_space_char(char *input, int *i, t_redir_ctx *ctx)
{
	if (*ctx->buffer_len > 0)
		add_token_from_buffer(ctx->tokens, ctx->buffer, ctx->buffer_len);
	skip_spaces(input, i);
}

void	process_normal_char(char *input, int *i, t_buffer_ctx *ctx)
{
	ctx->buffer[(*ctx->buffer_len)++] = input[*i];
	(*i)++;
}
