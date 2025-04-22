/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_expand_dquote.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julien <julien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 23:18:02 by julien            #+#    #+#             */
/*   Updated: 2025/04/22 23:49:53 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int	expand_exit_status(t_shell *sh, char *result, int *result_len,
		int i)
{
	char	*exit_code;

	exit_code = ft_itoa(sh->last_exit_status);
	if (exit_code)
	{
		ft_strlcpy(result + *result_len, exit_code, ft_strlen(exit_code) + 1);
		*result_len += ft_strlen(exit_code);
		free(exit_code);
	}
	return (i + 1);
}

static int	expand_env_var(t_shell *sh, char *s, char *result, int *result_len)
{
	char	var_name[256];
	int		var_idx;
	int		i;
	char	*var_value;

	var_idx = 0;
	i = 0;
	while (s[i] && (ft_isalnum(s[i]) || s[i] == '_') && var_idx < 255)
		var_name[var_idx++] = s[i++];
	var_name[var_idx] = '\0';
	var_value = get_env_value(sh->env, var_name);
	if (var_value)
	{
		ft_strlcpy(result + *result_len, var_value, ft_strlen(var_value) + 1);
		*result_len += ft_strlen(var_value);
		free(var_value);
	}
	return (i);
}

static int	expand_invalid_dollar(char *s, char *result, int *result_len, int i)
{
	result[(*result_len)++] = '$';
	if (s[i] && s[i] != '"')
	{
		result[(*result_len)++] = s[i];
		i++;
	}
	return (i);
}

void	copy_text_segment(char *start, char *end, char *result,
		int *result_len)
{
	int	len_before;

	len_before = end - start;
	if (len_before > 0)
	{
		ft_memcpy(result + *result_len, start, len_before);
		*result_len += len_before;
	}
}

int	process_var_segment(t_shell *sh, char *s, int i, t_expand_ctx *ctx)
{
	copy_text_segment(ctx->start, &s[i], ctx->result, &ctx->result_len);
	i++;
	if (s[i] == '?')
		i = expand_exit_status(sh, ctx->result, &ctx->result_len, i);
	else if (ft_isalpha(s[i]) || s[i] == '_')
		i += expand_env_var(sh, &s[i], ctx->result, &ctx->result_len);
	else
		i = expand_invalid_dollar(s, ctx->result, &ctx->result_len, i);
	ctx->start = &s[i];
	return (i);
}
