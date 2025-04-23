/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leaugust <leaugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 21:30:28 by julien            #+#    #+#             */
/*   Updated: 2025/04/23 15:50:34 by leaugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	exec_builtin_cmd(t_shell *sh, t_token *token, char *input)
{
	char	**argv;
	char	*cmd;
	int		ret;

	(void)input;
	ret = 0;
	cmd = ft_strdup(token->input);
	if (!cmd)
		return (set_exit_code(sh, 1), perror("minishell"), 1);
	argv = build_argv(sh, token);
	if (!argv)
	{
		free(cmd);
		return (set_exit_code(sh, 1), perror("minishell"), 1);
	}
	if (ft_strcmp(cmd, "echo") == 0)
		ret = ft_echo(sh, argv);
	else if (ft_strcmp(cmd, "exit") == 0)
		ret = ft_exit(sh, argv);
	else if (ft_strcmp(cmd, "cd") == 0)
		ret = ft_cd(sh, argv);
	else if (ft_strcmp(cmd, "export") == 0)
		ret = ft_export(sh, argv);
	else if (ft_strcmp(cmd, "env") == 0)
		ret = ft_env(sh, argv);
	else if (ft_strcmp(cmd, "pwd") == 0)
		ret = ft_pwd(sh, argv);
	else if (ft_strcmp(cmd, "unset") == 0)
		ret = ft_unset(sh, argv);
	free(cmd);
	ft_free_split(argv);
	return (ret);
}

int	exec_builtin_redirect(t_shell *sh, t_token *token, char *input)
{
	pid_t	pid;
	int		status;
	int		redir_code;

	status = 0;
	pid = fork();
	if (pid < 0)
		return (set_exit_code(sh, 1), perror("minishell"), 1);
	if (pid == 0)
	{
		redir_code = handle_redirect(token, input);
		if (redir_code != 0)
			exit(redir_code);
		exit(exec_builtin_cmd(sh, token, input));
	}
	waitpid(pid, &status, 0);
	return (handle_exit_status(sh, status, token->input));
}

int	run_ext_child(t_shell *sh, t_token *token, char *path, char **argv)
{
	int	redir_code;

	redir_code = handle_redirect(token, NULL);
	if (redir_code != 0)
		exit(redir_code);
	execve(path, argv, sh->env);
	exit(handle_execve_err(path, argv));
}

int	exec_ext_cmd(t_shell *sh, t_token *token, char **argv)
{
	char	*path;
	pid_t	pid;
	int		status;
	int		exit_code;

	status = 0;
	if (!argv || !argv[0])
		return (set_exit_code(sh, ERR_GENERAL), ERR_GENERAL);
	path = get_path(argv[0]);
	if (!path)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(argv[0], STDERR_FILENO);
		if (errno == EACCES)
		{
			ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
			ft_free_split(argv);
			return (set_exit_code(sh, ERR_NOT_EXEC), ERR_NOT_EXEC);
		}
		else
		{
			ft_putstr_fd(": command not found\n", STDERR_FILENO);
			ft_free_split(argv);
			return (set_exit_code(sh, ERR_CMD_NOT_FOUND), ERR_CMD_NOT_FOUND);
		}
	}
	pid = fork();
	if (pid < 0)
		return (perror("minishell"), free(path), ft_free_split(argv),
			ERR_GENERAL);
	if (pid == 0)
		run_ext_child(sh, token, path, argv);
	else
		waitpid(pid, &status, 0);
	free(path);
	ft_free_split(argv);
	exit_code = handle_exit_status(sh, status, token->input);
	return (exit_code);
}

void	exec_cmd(t_shell *sh, t_token *token, char *input)
{
	char			**argv;
	int				ret;
	t_token_list	*tokens_list;

	if (token->type != CMD)
		return ;
	tokens_list = init_token_list();
	if (!tokens_list)
		return ;
	tokens_list->head = token;
	sh->current_tokens = tokens_list;
	if (token->input[0] == '$')
	{
		if (!is_builtin(token->input))
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(token->input, STDERR_FILENO);
			ft_putstr_fd(": command not found\n", STDERR_FILENO);
			sh->last_exit_status = ERR_CMD_NOT_FOUND;
			set_exit_code(sh, ERR_CMD_NOT_FOUND);
			sh->current_tokens = NULL;
			free(tokens_list);
			return ;
		}
	}
	if (contains_pipe(token))
		ret = exec_pipe(sh, token, input);
	else
	{
		expand_token_list(sh, token);
		if (is_builtin(token->input))
		{
			if (has_redirect(token))
				ret = exec_builtin_redirect(sh, token, input);
			else
				ret = exec_builtin_cmd(sh, token, input);
		}
		else
		{
			if (access(token->input, F_OK) == 0 && !ft_strchr(token->input,
					'/'))
			{
				ft_putstr_fd("minishell: ", STDERR_FILENO);
				ft_putstr_fd(token->input, STDERR_FILENO);
				ft_putstr_fd(": command not found\n", STDERR_FILENO);
				sh->last_exit_status = ERR_CMD_NOT_FOUND;
				set_exit_code(sh, ERR_CMD_NOT_FOUND);
				sh->current_tokens = NULL;
				free(tokens_list);
				return ;
			}
			argv = build_argv(sh, token);
			if (!argv)
			{
				sh->current_tokens = NULL;
				free(tokens_list);
				return ;
			}
			ret = exec_ext_cmd(sh, token, argv);
		}
	}
	sh->last_exit_status = ret;
	set_exit_code(sh, ret);
	sh->current_tokens = NULL;
	free(tokens_list);
}
