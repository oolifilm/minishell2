/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirect.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julien <julien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 22:30:00 by julien            #+#    #+#             */
/*   Updated: 2025/04/25 00:42:36 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	has_redirect(t_token *token)
{
	t_token	*cur;

	cur = token;
	while (cur)
	{
		if (cur->type == REDIR_IN || cur->type == REDIR_OUT
			|| cur->type == APPEND || cur->type == HEREDOC)
			return (1);
		cur = cur->next;
	}
	return (0);
}

char	*clean_filename(char *input)
{
	char	*clean;
	int		len;
	int		i;
	int		j;

	if (!input)
		return (NULL);
	len = ft_strlen(input);
	clean = malloc(len + 1);
	if (!clean)
		return (NULL);
	if ((len >= 2) && ((input[0] == '"' && input[len - 1] == '"')
			|| (input[0] == '\'' && input[len - 1] == '\'')))
	{
		i = 1;
		j = 0;
		while (i < len - 1)
			clean[j++] = input[i++];
		clean[j] = '\0';
	}
	else
		ft_strlcpy(clean, input, len + 1);
	return (clean);
}

static int	handle_append_redirect(t_token *cur)
{
	int		fd;
	char	*clean_file;

	if (!cur->next || !cur->next->input)
		return (ft_putstr_fd("minishell: syntax error near unexpected token\n",
				2), ERR_GENERAL);
	clean_file = clean_filename(cur->next->input);
	if (!clean_file)
		return (perror("minishell"), ERR_GENERAL);
	fd = open(clean_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		free(clean_file);
		return (print_exec_err(cur->next->input), ERR_GENERAL);
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

int	handle_redirect(t_token *token, char *input)
{
	t_token	*cur;
	int		ret;

	ret = 0;
	(void)input;
	cur = token;
	while (cur && !ret)
	{
		if (cur->type == REDIR_IN)
			ret = handle_input_redirect(cur);
		else if (cur->type == HEREDOC)
			ret = handle_heredoc_redirect(cur);
		else if (cur->type == REDIR_OUT)
			ret = handle_output_redirect(cur);
		else if (cur->type == APPEND)
			ret = handle_append_redirect(cur);
		cur = cur->next;
	}
	return (ret);
}
