/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julien <julien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 22:10:28 by julien            #+#    #+#             */
/*   Updated: 2025/04/25 00:35:39 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

/* Prototype de la fonction déplacée dans exec_builtin_helper.c */
int	exec_builtin_command(t_shell *sh, char *cmd, char **argv);

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
	ret = exec_builtin_command(sh, cmd, argv);
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
	path = get_path(sh, argv[0]);
	if (!path)
		return (handle_cmd_not_found(sh, argv));
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

int	handle_cmd_not_found(t_shell *sh, char **argv)
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
