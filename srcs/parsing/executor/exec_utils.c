/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leaugust <leaugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 21:30:30 by julien            #+#    #+#             */
/*   Updated: 2025/04/18 19:16:21 by leaugust         ###   ########.fr       */
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

char	**fill_argv(t_shell *sh, t_token *token, char **argv)
{
	t_token	*cur;
	int		i;

	i = 1;
	cur = token->next;
	while (cur)
	{
		if (cur->type == STRING || cur->type == ARG || cur->type == ENV || cur->type == EXIT)
		{
			argv[i] = expand_token(sh, cur);
			// printf("argv[%d] = '%s'\n", i, argv[i]);
			i++;
		}
		cur = cur->next;
	}
	argv[i] = NULL;
	return (argv);
}

char	**build_argv(t_shell *sh, t_token *token)
{
	char	**argv;
	int		count;
	t_token	*cur;

	count = 1;
	cur = token->next;
	if (token->type == CMD && token->input && token->input[0] == '\0')
	{
		sh->last_exit_status = 127;
		return (NULL);
	}
	while (cur)
	{
		if (cur->type == STRING || cur->type == ARG || cur->type == ENV
			|| cur->type == EXIT)
			count++;
		cur = cur->next;
	}
	argv = malloc(sizeof(char *) * (count + 1));
	if (!argv)
		return (NULL);
	argv[0] = ft_strdup(token->input);
	return (fill_argv(sh, token, argv));
}

int	handle_redirect(t_token *token, char *input)
{
	t_token	*cur;
	int		fd;

	(void)input;
	cur = token;
	while (cur)
	{
		if (cur->type == REDIR_IN)
		{
			fd = open(cur->next->input, O_RDONLY);
			if (fd < 0)
			{
				print_exec_err(cur->next->input);
				return (1);
			}
			if (dup2(fd, STDIN_FILENO) < 0)
				return (perror("minishell"), close(fd), 1);
			close(fd);
		}
		else if (cur->type == REDIR_OUT)
		{
			fd = open(cur->next->input, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd < 0)
			{
				print_exec_err(cur->next->input);
				return (get_exec_err_code());
			}
			if (dup2(fd, STDOUT_FILENO) < 0)
				return (perror("minishell"), close(fd), 1);
			close(fd);
		}
		else if (cur->type == APPEND)
		{
			fd = open(cur->next->input, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd < 0)
			{
				print_exec_err(cur->next->input);
				return (1);
			}
			if (dup2(fd, STDOUT_FILENO) < 0)
				return (perror("minishell"), close(fd), 1);
			close(fd);
		}
		cur = cur->next;
	}
	return (0);
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	return (0);
}
