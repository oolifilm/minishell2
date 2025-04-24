/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julien <julien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 21:12:00 by julien            #+#    #+#             */
/*   Updated: 2025/04/24 22:20:00 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	is_quoted_delimiter(char *delimiter)
{
	int	len;

	len = ft_strlen(delimiter);
	if (len >= 2 && delimiter[0] == '\'' && delimiter[len - 1] == '\'')
		return (1);
	return (0);
}

char	*prepare_delimiter(char *delimiter, int *should_expand)
{
	char	*unquoted_delimiter;

	*should_expand = !is_quoted_delimiter(delimiter);
	if (!*should_expand)
	{
		unquoted_delimiter = ft_substr(delimiter, 1, ft_strlen(delimiter) - 2);
		if (unquoted_delimiter)
			return (unquoted_delimiter);
	}
	return (NULL);
}
