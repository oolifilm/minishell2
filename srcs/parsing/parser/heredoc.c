/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julien <julien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 21:00:00 by julien            #+#    #+#             */
/*   Updated: 2025/04/24 22:10:23 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	handle_heredocs(t_shell *sh, t_token *token)
{
	char	*delimiter;
	int		fd;

	while (token)
	{
		if (token->type == HEREDOC)
		{
			if (token->next)
				delimiter = token->next->input;
			else
				delimiter = NULL;
			if (!delimiter)
			{
				ft_putstr_fd("minishell: error: Missing delimiter\n", 2);
				return (1);
			}
			fd = process_heredoc_input(sh, delimiter);
			if (fd < 0)
				return (1);
			token->heredoc_fd = fd;
		}
		token = token->next;
	}
	return (0);
}
