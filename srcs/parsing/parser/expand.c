/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julien <julien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 21:00:00 by julien            #+#    #+#             */
/*   Updated: 2025/04/24 23:53:24 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static char	*handle_expanded_var(t_shell *sh, char *line, int *i,
		t_expand_ctx *ctx)
{
	char	*expanded;
	size_t	expanded_len;

	expanded = expand_var(sh, line, i);
	if (!expanded)
		return (ctx->result);
	expanded_len = ft_strlen(expanded);
	if ((size_t)ctx->result_len + expanded_len >= (size_t)ctx->result_size - 1)
		ctx->result = realloc_result(ctx->result, ctx->result_len, expanded_len,
				line);
	ft_strlcpy(ctx->result + ctx->result_len, expanded, ft_strlen(expanded)
		+ 1);
	ctx->result_len += ft_strlen(expanded);
	free(expanded);
	return (ctx->result);
}

static void	init_expansion(char *line, t_expand_ctx *ctx)
{
	ctx->result_size = ft_strlen(line) * 2;
	ctx->result = (char *)malloc(sizeof(char) * (ctx->result_size));
	ctx->result_len = 0;
	ctx->start = line;
}

char	*expand_line(t_shell *sh, char *line)
{
	t_expand_ctx	ctx;
	int				i;
	char			*null_result;

	null_result = handle_null_input(sh, line);
	if (null_result)
		return (null_result);
	init_expansion(line, &ctx);
	if (!ctx.result)
		return (ft_strdup(line));
	i = 0;
	while (line[i])
	{
		if (check_dollar_var(line[i], line[i + 1]))
			ctx.result = handle_expanded_var(sh, line, &i, &ctx);
		else
			ctx.result[ctx.result_len++] = line[i++];
	}
	ctx.result[ctx.result_len] = '\0';
	return (ctx.result);
}
