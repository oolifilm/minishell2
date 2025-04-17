/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbanchon <jbanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 21:30:28 by julien            #+#    #+#             */
/*   Updated: 2025/04/17 16:33:11 by jbanchon         ###   ########.fr       */
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
		return (1);
	argv = build_argv(sh, token);
	if (!argv)
	{
		free(cmd);
		return (1);
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

	status = 0;
	pid = fork();
	if (pid < 0)
		return (perror("minishell"), 1);
	if (pid == 0)
	{
		if (handle_redirect(token, NULL) == -1)
			exit(1);
		exit(exec_builtin_cmd(sh, token, input));
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

int	run_ext_child(t_shell *sh, t_token *token, char *path, char **argv)
{
	if (handle_redirect(token, NULL) == -1)
		exit(1);
	execve(path, argv, sh->env);
	handle_execve_err(path, argv);
	return (0);
}

int	exec_ext_cmd(t_shell *sh, t_token *token, char **argv)
{
	char	*path;
	pid_t	pid;
	int		status;

	status = 0;
	path = get_path(argv[0]);
	if (!path)
	{
		perror("minishell");
		ft_free_split(argv);
		return (127);
	}
	pid = fork();
	if (pid < 0)
		return (perror("minishell"), free(path), ft_free_split(argv), 1);
	if (pid == 0)
		run_ext_child(sh, token, path, argv);
	else
		waitpid(pid, &status, 0);
	free(path);
	ft_free_split(argv);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

void	exec_cmd(t_shell *sh, t_token *token, char *input)
{
	char	**argv;
	int		ret;

	if (token->type != CMD)
		return ;
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
			argv = build_argv(sh, token);
			if (!argv)
				return ;
			ret = exec_ext_cmd(sh, token, argv);
		}
	}
	sh->last_exit_status = ret;
	set_exit_code(sh, ret);
}
