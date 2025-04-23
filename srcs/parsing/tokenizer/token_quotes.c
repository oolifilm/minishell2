/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leaugust <leaugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 11:00:31 by julien            #+#    #+#             */
/*   Updated: 2025/04/23 15:58:00 by leaugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

// Fonction pour supprimer les quotes externes tout en
// préservant les quotes imbriquées

char	*remove_quotes(const char *s)
{
	char	*res;
	int		i;
	int		j;
	char	quote;

	i = 0;
	j = 0;
	quote = 0;
	res = malloc(ft_strlen(s) + 1);
	if (!res)
		return (NULL);
	while (s[i])
	{
		if ((s[i] == '"' || s[i] == '\'') && (quote == 0 || quote == s[i]))
		{
			if (quote == 0)
				quote = s[i];
			else if (quote == s[i])
				quote = 0;
			i++;
		}
		else if ((s[i] == '"' && quote == '\'') || (s[i] == '\''
				&& quote == '"'))
		{
			res[j++] = s[i++];
		}
		else
		{
			res[j++] = s[i++];
		}
	}
	res[j] = '\0';
	return (res);
}

/* Extrait le contenu entre apostrophes (' ') dans l'entrée input. */
static void	handle_single_quote(char *input, int *i, t_token_list *tokens)
{
	char	buffer[1024];
	int		len;
	int		start;
	char	*final_content;

	len = 0;
	(*i)++;
	start = *i;
	buffer[len++] = '\'';
	while (input[*i] && input[*i] != '\'')
		(*i)++;
	if (input[*i] == '\'')
	{
		while (start < *i && len < 1022)
			buffer[len++] = input[start++];
		(*i)++;
		buffer[len++] = '\'';
		while (input[*i] && !ft_isspace(input[*i])
			&& !is_token_breaker(input[*i]) && len < 1023)
			buffer[len++] = input[(*i)++];
		buffer[len] = '\0';
		final_content = ft_strdup(buffer);
		add_token(tokens, final_content, STRING, SINGLE_QUOTE);
	}
}

/* Extrait le contenu entre guillemets (" ) dans l'entrée input. */
static void	handle_double_quotes(char *input, int *i, t_token_list *tokens)
{
	char	buffer[1024];
	int		len;
	int		start;
	char	*content;
	char	*expanded;
	char	*final_content;

	len = 0;
	(*i)++;
	start = *i;
	while (input[*i] && input[*i] != '"')
		(*i)++;
	if (input[*i] == '"')
	{
		content = ft_substr(input, start, *i - start);
		if (content)
		{
			expanded = expand_var_in_dquotes(content);
			free(content);
			if (expanded)
			{
				ft_strlcpy(buffer, expanded, sizeof(buffer));
				len = ft_strlen(buffer);
				free(expanded);
				(*i)++;
				while (input[*i] && !ft_isspace(input[*i])
					&& !is_token_breaker(input[*i]) && len < 1023)
				{
					buffer[len] = input[*i];
					len++;
					(*i)++;
				}
				buffer[len] = '\0';
				final_content = remove_quotes(buffer);
				add_token(tokens,
					final_content ? final_content : ft_strdup(buffer), STRING,
					DOUBLE_QUOTE);
				free(final_content);
			}
		}
	}
}

/* Gère le contenu entre guillemets et apostrophes dans l'entrée input. */
void	handle_quoted_content(char *input, int *i, t_token_list *tokens,
		char quote_type)
{
	if (quote_type == '"')
		handle_double_quotes(input, i, tokens);
	else
		handle_single_quote(input, i, tokens);
}
