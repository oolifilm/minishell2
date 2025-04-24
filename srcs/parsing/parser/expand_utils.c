/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julien <julien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 21:10:00 by julien            #+#    #+#             */
/*   Updated: 2025/04/24 21:10:00 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

char	*copy_buffer(char *dest, char *src, int len)
{
	int	i;

	i = 0;
	while (i < len)
	{
		dest[i] = src[i];
		i++;
	}
	return (dest);
}

char	*realloc_result(char *result, int j, int expanded_len, char *line)
{
	char	*new_result;
	int		result_size;

	result_size = (j + expanded_len) * 2 + 1;
	new_result = malloc(sizeof(char) * result_size);
	if (!new_result)
	{
		free(result);
		return (ft_strdup(line));
	}
	copy_buffer(new_result, result, j);
	free(result);
	return (new_result);
}

char	*copy_expanded(char *result, char *expanded, int *j)
{
	int	k;

	k = 0;
	while (expanded[k])
		result[(*j)++] = expanded[k++];
	free(expanded);
	return (result);
}

int	check_dollar_var(char c, char next)
{
	if (c == '$' && next && (ft_isalnum(next) || next == '_' || next == '?'))
		return (1);
	return (0);
}

char	*init_expand_result(char *line, int *result_size)
{
	char	*result;

	*result_size = ft_strlen(line) * 4 + 1;
	result = malloc(sizeof(char) * (*result_size));
	if (!result)
		return (NULL);
	return (result);
}
