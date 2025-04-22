/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_expand_dquote2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julien <julien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 23:22:53 by julien            #+#    #+#             */
/*   Updated: 2025/04/22 23:22:53 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

/*
** Expand les variables dans une chaîne entre guillemets doubles
** Fonction principale qui coordonne tout le processus d'expansion
*/
void	expand_double_quoted_vars(t_shell *sh, t_token *token)
{
	char		*s;
	char		*result;
	t_expand_ctx	ctx;
	int			i;

	s = token->input;
	result = malloc(ft_strlen(s) * 4);
	if (!result)
		return ;
	i = 0;
	ctx.start = s;
	ctx.result = result;
	ctx.result_len = 0;
	while (s[i])
	{
		if (s[i] == '$' && s[i + 1])
			i = process_var_segment(sh, s, i, &ctx);
		else
			i++;
	}
	copy_text_segment(ctx.start, &s[i], result, &ctx.result_len);
	result[ctx.result_len] = '\0';
	free(token->input);
	token->input = result;
}
