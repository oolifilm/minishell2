/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_quotes_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julien <julien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 23:45:00 by julien            #+#    #+#             */
/*   Updated: 2025/04/24 23:45:00 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

/* Traite les quotes imbriquées dans une chaîne */
static char	*process_nested_quotes(const char *s, int *i, int *j, char *res)
{
	char	quote;

	quote = 0;
	while (s[*i])
	{
		if ((s[*i] == '"' || s[*i] == '\'') && (quote == 0 || quote == s[*i]))
		{
			if (quote == 0)
				quote = s[*i];
			else if (quote == s[*i])
				quote = 0;
			(*i)++;
		}
		else if ((s[*i] == '"' && quote == '\'') || (s[*i] == '\''
				&& quote == '"'))
			res[(*j)++] = s[(*i)++];
		else
			res[(*j)++] = s[(*i)++];
	}
	return (res);
}

/* Supprime les quotes externes tout en préservant les quotes imbriquées */
char	*remove_quotes(const char *s)
{
	char	*res;
	int		i;
	int		j;

	i = 0;
	j = 0;
	res = malloc(ft_strlen(s) + 1);
	if (!res)
		return (NULL);
	res = process_nested_quotes(s, &i, &j, res);
	res[j] = '\0';
	return (res);
}

/* Extrait le contenu après une quote simple jusqu'au prochain séparateur */
static void	extract_after_squote(char *input, int *i, char *buffer, int *len)
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

/* Traite le contenu entre apostrophes */
void	handle_single_quote(char *input, int *i, t_token_list *tokens)
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
		extract_after_squote(input, i, buffer, &len);
		buffer[len] = '\0';
		final_content = ft_strdup(buffer);
		add_token(tokens, final_content, STRING, SINGLE_QUOTE);
	}
}
