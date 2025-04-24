/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leaugust <leaugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 22:12:28 by julien            #+#    #+#             */
/*   Updated: 2025/04/24 22:12:50 by leaugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	open_heredoc_file(t_shell *sh, t_token *token)
{
	char	*delimiter;

	if (token->next)
		delimiter = token->next->input;
	else
		delimiter = NULL;
	if (!delimiter)
	{
		ft_putstr_fd("minishell: error: Missing heredoc delimiter\n",
			STDERR_FILENO);
		return (-1);
	}
	return (process_heredoc_input(sh, delimiter));
}

int	check_heredoc(t_shell *sh, t_token *token, int *heredoc_fd)
{
	t_token	*cur;

	cur = token;
	*heredoc_fd = -1;
	while (cur)
	{
		if (cur->type == HEREDOC)
		{
			if (cur->heredoc_fd >= 0)
				*heredoc_fd = cur->heredoc_fd;
			else
			{
				*heredoc_fd = open_heredoc_file(sh, cur);
				if (*heredoc_fd < 0)
					return (0);
			}
			break ;
		}
		cur = cur->next;
	}
	return (1);
}

int	handle_directory_error(t_shell *sh, t_token *token,
	t_token_list *tokens_list)
{
	struct stat	st;

	if (ft_strchr(token->input, '/') && access(token->input, F_OK) == 0)
	{
		stat(token->input, &st);
		if (S_ISDIR(st.st_mode))
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(token->input, STDERR_FILENO);
			ft_putstr_fd(": is a directory\n", STDERR_FILENO);
			sh->last_exit_status = ERR_GENERAL;
			set_exit_code(sh, ERR_GENERAL);
			sh->current_tokens = NULL;
			free(tokens_list);
			return (1);
		}
	}
	return (0);
}

int	handle_file_access_error(t_shell *sh, t_token *token,
	t_token_list *tokens_list)
{
	if (access(token->input, F_OK) == 0 && !ft_strchr(token->input, '/'))
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(token->input, STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		sh->last_exit_status = ERR_CMD_NOT_FOUND;
		set_exit_code(sh, ERR_CMD_NOT_FOUND);
		sh->current_tokens = NULL;
		free(tokens_list);
		return (1);
	}
	return (0);
}
