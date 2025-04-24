/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julien <julien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 11:48:46 by jbanchon          #+#    #+#             */
/*   Updated: 2025/04/24 23:44:03 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

/* Prototypes des fonctions déplacées dans exec_pipe_utils.c */
t_token	*get_next_cmd(t_token *token);
pid_t	left_cmd(t_shell *sh, t_token *cmd, char *input, int fd[2]);
pid_t	right_cmd(t_shell *sh, t_token *cmd, char *input, int fd[2]);
int		contains_pipe(t_token *token);

static int	setup_pipe_commands(t_shell *sh, t_token *token, t_pipe_ctx *ctx)
{
	t_token	*tmp;

	ctx->left_cmd = token;
	ctx->pipe_token = token;
	while (ctx->pipe_token && ctx->pipe_token->type != PIPE)
		ctx->pipe_token = ctx->pipe_token->next;
	if (!ctx->pipe_token)
		return (set_exit_code(sh, ERR_GENERAL), 1);
	ctx->right_cmd = get_next_cmd(ctx->pipe_token->next);
	if (!ctx->right_cmd)
		return (perror("minishell"), set_exit_code(sh, ERR_GENERAL), 1);
	tmp = token;
	while (tmp && tmp->next != ctx->pipe_token)
		tmp = tmp->next;
	if (tmp)
		tmp->next = NULL;
	return (0);
}

static int	execute_pipe_commands(t_shell *sh, t_pipe_ctx *ctx, char *input)
{
	if (pipe(ctx->fd) < 0)
		return (perror("minishell"), set_exit_code(sh, ERR_GENERAL), 1);
	ctx->pid_left = left_cmd(sh, ctx->left_cmd, input, ctx->fd);
	if (ctx->pid_left < 0)
	{
		close(ctx->fd[0]);
		close(ctx->fd[1]);
		return (1);
	}
	ctx->pid_right = right_cmd(sh, ctx->right_cmd, input, ctx->fd);
	if (ctx->pid_right < 0)
	{
		close(ctx->fd[0]);
		close(ctx->fd[1]);
		return (1);
	}
	close(ctx->fd[0]);
	close(ctx->fd[1]);
	return (0);
}

static int	wait_for_pipe_commands(t_shell *sh, t_pipe_ctx *ctx)
{
	int	status_left;
	int	status_right;
	int	exit_code;

	waitpid(ctx->pid_left, &status_left, 0);
	waitpid(ctx->pid_right, &status_right, 0);
	if (WIFEXITED(status_right))
	{
		exit_code = WEXITSTATUS(status_right);
		if (exit_code == 2)
			exit_code = 1;
		set_exit_code(sh, exit_code);
		return (exit_code);
	}
	exit_code = handle_exit_status(sh, status_right, ctx->right_cmd->input);
	return (exit_code);
}

int	exec_pipe(t_shell *sh, t_token *token, char *input)
{
	t_pipe_ctx	ctx;
	int			ret;

	ret = setup_pipe_commands(sh, token, &ctx);
	if (ret != 0)
		return (ret);
	ret = execute_pipe_commands(sh, &ctx, input);
	if (ret != 0)
		return (ret);
	return (wait_for_pipe_commands(sh, &ctx));
}
