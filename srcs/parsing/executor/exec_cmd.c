/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julien <julien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 21:30:28 by julien            #+#    #+#             */
/*   Updated: 2025/04/24 23:42:14 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int	exec_cmd_pipe(t_shell *sh, t_token *token, char *input,
	t_token_list *tokens_list)
{
	int	ret;

	ret = exec_pipe(sh, token, input);
	sh->last_exit_status = ret;
	set_exit_code(sh, ret);
	sh->current_tokens = NULL;
	free(tokens_list);
	return (ret);
}

static int	exec_cmd_builtin(t_shell *sh, t_token *token, char *input,
	t_token_list *tokens_list)
{
	int	ret;

	if (has_redirect(token))
		ret = exec_builtin_redirect(sh, token, input);
	else
		ret = exec_builtin_cmd(sh, token, input);
	sh->last_exit_status = ret;
	set_exit_code(sh, ret);
	sh->current_tokens = NULL;
	free(tokens_list);
	return (ret);
}

static int	exec_cmd_external(t_shell *sh, t_token *token, char *input,
	t_token_list *tokens_list)
{
	char	**argv;
	int		ret;

	(void)input;
	if (handle_file_access_error(sh, token, tokens_list))
		return (0);
	argv = build_argv(sh, token);
	if (!argv)
	{
		sh->current_tokens = NULL;
		free(tokens_list);
		return (0);
	}
	ret = exec_ext_cmd(sh, token, argv);
	sh->last_exit_status = ret;
	set_exit_code(sh, ret);
	sh->current_tokens = NULL;
	free(tokens_list);
	return (ret);
}

static int	prepare_cmd_exec(t_shell *sh, t_token *token,
	t_token_list **tokens_list)
{
	int	heredoc_fd;

	if (!check_heredoc(sh, token, &heredoc_fd))
	{
		sh->last_exit_status = ERR_GENERAL;
		set_exit_code(sh, ERR_GENERAL);
		return (0);
	}
	*tokens_list = init_token_list();
	if (!*tokens_list)
		return (0);
	(*tokens_list)->head = token;
	sh->current_tokens = *tokens_list;
	return (1);
}

void	exec_cmd(t_shell *sh, t_token *token, char *input)
{
	t_token_list	*tokens_list;

	if (token->type != CMD)
		return ;
	tokens_list = NULL;
	if (!prepare_cmd_exec(sh, token, &tokens_list))
		return ;
	if (contains_pipe(token))
	{
		exec_cmd_pipe(sh, token, input, tokens_list);
		return ;
	}
	expand_token_list(sh, token);
	if (handle_directory_error(sh, token, tokens_list))
		return ;
	if (is_builtin(token->input))
		exec_cmd_builtin(sh, token, input, tokens_list);
	else
		exec_cmd_external(sh, token, input, tokens_list);
}
