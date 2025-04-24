/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julien <julien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 21:00:00 by julien            #+#    #+#             */
/*   Updated: 2025/04/24 22:20:00 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	create_heredoc_file(void)
{
	int	fd;

	fd = open("/tmp/heredoc_file.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("minishell: heredoc");
		return (-1);
	}
	return (fd);
}

void	process_heredoc_line(t_shell *sh, char *line, int fd, int expand)
{
	char	*expanded_line;

	if (expand && sh)
	{
		expanded_line = expand_line(sh, line);
		free(line);
		line = expanded_line;
	}
	write(fd, line, strlen(line));
	write(fd, "\n", 1);
	free(line);
}

static int	read_heredoc_lines(t_shell *sh, char *delimiter, int fd,
	int should_expand)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line || strcmp(line, delimiter) == 0)
		{
			if (line)
				free(line);
			break ;
		}
		process_heredoc_line(sh, line, fd, should_expand);
	}
	return (fd);
}

int	process_heredoc_input(t_shell *sh, char *delimiter)
{
	int		fd;
	int		should_expand;
	char	*unquoted;

	unquoted = prepare_delimiter(delimiter, &should_expand);
	if (unquoted)
		delimiter = unquoted;
	fd = create_heredoc_file();
	if (fd < 0)
	{
		if (unquoted)
			free(unquoted);
		return (-1);
	}
	fd = read_heredoc_lines(sh, delimiter, fd, should_expand);
	if (unquoted)
		free(unquoted);
	return (fd);
}
