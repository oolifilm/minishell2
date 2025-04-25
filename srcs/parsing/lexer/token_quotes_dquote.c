/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_quotes_dquote.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julien <julien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 23:50:00 by julien            #+#    #+#             */
/*   Updated: 2025/04/25 16:54:04 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

/* Extrait le contenu après une quote double jusqu'au prochain séparateur */
static void	extract_after_dquote(char *input, int *i, char *buffer, int *len)
{
	(*i)++;
	while (input[*i] && !ft_isspace(input[*i])
		&& !is_token_breaker(input[*i]) && *len < 1023)
	{
		buffer[*len] = input[*i];
		(*len)++;
		(*i)++;
	}
}

/* Traite le contenu extrait des guillemets doubles */
static void	process_dquote_content(char *buffer, int len,
	t_token_list *tokens)
{
	char	*final_content;

	buffer[len] = '\0';
	final_content = remove_quotes(buffer);
	if (final_content)
	{
		add_token(tokens, final_content, STRING, DOUBLE_QUOTE);
		free(final_content);
	}
	else
	{
		add_token(tokens, buffer, STRING, DOUBLE_QUOTE);
	}
}

/* Extrait et traite le contenu entre guillemets doubles */
static char	*extract_dquote_content(char *input, int *i)
{
	int		start;
	char	*content;

	(*i)++;
	start = *i;
	while (input[*i] && input[*i] != '"')
		(*i)++;
	if (input[*i] == '"')
	{
		content = ft_substr(input, start, *i - start);
		return (content);
	}
	return (NULL);
}

/* Extrait le contenu entre guillemets doubles et l'ajoute comme token */
void	handle_double_quotes(char *input, int *i, t_token_list *tokens)
{
	char	buffer[1024];
	int		len;
	char	*content;
	char	*expanded;

	content = extract_dquote_content(input, i);
	if (!content)
		return ;
	expanded = expand_var_in_dquotes(content);
	free(content);
	if (!expanded)
		return ;
	ft_strlcpy(buffer, expanded, sizeof(buffer));
	len = ft_strlen(buffer);
	free(expanded);
	extract_after_dquote(input, i, buffer, &len);
	process_dquote_content(buffer, len, tokens);
}
