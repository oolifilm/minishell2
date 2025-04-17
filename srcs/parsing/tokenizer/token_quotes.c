/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbanchon <jbanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 11:00:31 by julien            #+#    #+#             */
/*   Updated: 2025/04/17 18:22:58 by jbanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

// Fonction pour supprimer les quotes internes
char	*remove_quotes(const char *s)
{
	char	*res;
	int		i;
	int		j;
	char	quote;
	int		in_quote;

	i = 0;
	j = 0;
	quote = 0;
	in_quote = 0;
	res = malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (!res)
		return (NULL);
	while (s[i])
	{
		if ((s[i] == '"' || s[i] == '\''))
		{
			if (!in_quote)
			{
				in_quote = 1;
				quote = s[i];
			}
			else if (quote == s[i])
			{
				in_quote = 0;
				quote = 0;
			}
			else
				res[j++] = s[i];
			i++;
		}
		else
			res[j++] = s[i++];
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
	char	*content;

	len = 0;
	(*i)++;
	start = *i;
	while (input[*i] && input[*i] != '\'')
		(*i)++;
	if (input[*i] == '\'')
	{
		while (start < *i && len < 1023)
			buffer[len++] = input[start++];
		buffer[len] = '\0';
		(*i)++;
		content = ft_strdup(buffer);
		add_token(tokens, content, STRING, SINGLE_QUOTE);
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
					&& !is_token_breaker(input[*i]))
					buffer[len++] = input[(*i)++];
				buffer[len] = '\0';
				final_content = remove_quotes(buffer);
				add_token(tokens, final_content, STRING, DOUBLE_QUOTE);
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
