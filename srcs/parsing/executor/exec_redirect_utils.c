/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirect_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leaugust <leaugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 22:50:00 by julien            #+#    #+#             */
/*   Updated: 2025/04/24 22:50:00 by leaugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	handle_input_redirect(t_token *cur)
{
	int		fd;
	char	*clean_file;

	if (!cur->next || !cur->next->input)
		return (ft_putstr_fd("minishell: syntax error near unexpected token\n",
				2), ERR_GENERAL);
	clean_file = clean_filename(cur->next->input);
	if (!clean_file)
		return (perror("minishell"), ERR_GENERAL);
	fd = open(clean_file, O_RDONLY);
	if (fd < 0)
	{
		free(clean_file);
		return (print_exec_err(cur->next->input), ERR_GENERAL);
	}
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		close(fd);
		free(clean_file);
		return (perror("minishell"), ERR_GENERAL);
	}
	close(fd);
	free(clean_file);
	return (0);
}

int	handle_heredoc_redirect(t_token *cur)
{
	int	fd;

	if (cur->heredoc_fd >= 0)
	{
		fd = open("/tmp/heredoc_file.txt", O_RDONLY);
		if (fd < 0)
			return (perror("minishell: heredoc"), ERR_GENERAL);
		if (dup2(fd, STDIN_FILENO) < 0)
		{
			close(fd);
			return (perror("minishell"), ERR_GENERAL);
		}
		close(fd);
	}
	return (0);
}

int	handle_output_redirect(t_token *cur)
{
	int		fd;
	char	*clean_file;

	if (!cur->next || !cur->next->input)
		return (ft_putstr_fd("minishell: syntax error near unexpected token\n",
				2), ERR_GENERAL);
	clean_file = clean_filename(cur->next->input);
	if (!clean_file)
		return (perror("minishell"), ERR_GENERAL);
	fd = open(clean_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		free(clean_file);
		print_exec_err(cur->next->input);
		return (ERR_GENERAL);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		close(fd);
		free(clean_file);
		return (perror("minishell"), ERR_GENERAL);
	}
	close(fd);
	free(clean_file);
	return (0);
}
