/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_expand_dquote2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julien <julien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 23:22:53 by julien            #+#    #+#             */
/*   Updated: 2025/04/25 01:18:56 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

/*
** Expand les variables dans une chaîne entre guillemets doubles
** Fonction principale qui coordonne tout le processus d'expansion
*/

static char	*init_expansion_buffer(char *s, t_expand_ctx *ctx)
{
	char	*result;
	size_t	alloc_size;

	alloc_size = ft_strlen(s) * 2 + 1;
	result = malloc(alloc_size);
	if (!result)
		return (NULL);
	ctx->start = s;
	ctx->result = result;
	ctx->result_len = 0;
	return (result);
}

static char	*realloc_expansion_buffer(t_expand_ctx *ctx, size_t *alloc_size)
{
	char	*new_result;

	*alloc_size *= 2;
	new_result = malloc(*alloc_size);
	if (!new_result)
		free(ctx->result);
	ft_memcpy(new_result, ctx->result, ctx->result_len);
	free(ctx->result);
	ctx->result = new_result;
	return (new_result);
}

static int	process_expansion_loop(t_shell *sh, char *s, t_expand_ctx *ctx,
		size_t *alloc_size)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '$' && s[i + 1])
			i = process_var_segment(sh, s, i, ctx);
		else
			i++;
		if (ctx->result_len >= (int)*alloc_size - 10)
			if (!realloc_expansion_buffer(ctx, alloc_size))
				return (-1);
	}
	return (i);
}

void	expand_double_quoted_vars(t_shell *sh, t_token *token)
{
	char			*s;
	char			*result;
	t_expand_ctx	ctx;
	int				i;
	size_t			alloc_size;

	s = token->input;
	result = init_expansion_buffer(s, &ctx);
	if (!result)
		return ;
	alloc_size = ft_strlen(s) * 2 + 1;
	i = process_expansion_loop(sh, s, &ctx, &alloc_size);
	if (i == -1)
		return ;
	copy_text_segment(ctx.start, &s[i], result, &ctx.result_len);
	result[ctx.result_len] = '\0';
	free(token->input);
	token->input = result;
	return ;
}
