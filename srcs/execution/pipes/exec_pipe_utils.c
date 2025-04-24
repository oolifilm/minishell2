/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julien <julien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 00:15:00 by julien            #+#    #+#             */
/*   Updated: 2025/04/25 00:15:00 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

/* Trouve la prochaine commande dans la liste de tokens */
t_token	*get_next_cmd(t_token *token)
{
	while (token && token->type != CMD)
		token = token->next;
	return (token);
}

/* Exécute la commande de gauche du pipe */
pid_t	left_cmd(t_shell *sh, t_token *cmd, char *input, int fd[2])
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (perror("minishell"), set_exit_code(sh, 1), -1);
	if (pid == 0)
	{
		close(fd[0]);
		if (dup2(fd[1], STDOUT_FILENO) < 0)
		{
			perror("minishell");
			exit(1);
		}
		close(fd[1]);
		exec_cmd(sh, cmd, input);
		exit(sh->last_exit_status);
	}
	return (pid);
}

/* Exécute la commande de droite du pipe */
pid_t	right_cmd(t_shell *sh, t_token *cmd, char *input, int fd[2])
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (set_exit_code(sh, 1), -1);
	if (pid == 0)
	{
		close(fd[1]);
		if (dup2(fd[0], STDIN_FILENO) < 0)
		{
			perror("minishell");
			exit(1);
		}
		close(fd[0]);
		exec_cmd(sh, cmd, input);
		exit(sh->last_exit_status);
	}
	return (pid);
}

/* Vérifie si la liste de tokens contient un pipe */
int	contains_pipe(t_token *token)
{
	while (token)
	{
		if (token->type == PIPE)
			return (1);
		token = token->next;
	}
	return (0);
}
