/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbanchon <jbanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 14:29:56 by jbanchon          #+#    #+#             */
/*   Updated: 2025/04/18 17:46:26 by jbanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	get_exec_err_code(void)
{
	if (errno == ENOENT)
		return (127);
	if (errno == EACCES)
		return (126);
	return (1);
}

void	print_exec_err(char *cmd)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(strerror(errno), STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}

int	handle_execve_err(char *path, char **argv)
{
	int	code;

	code = get_exec_err_code();
	print_exec_err(path);
	free(path);
	ft_free_split(argv);
	exit(code);
}

int	handle_path_error(char *shell, char *cmd)
{
	ft_putstr_fd(shell, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(": command not found\n", STDERR_FILENO);
	return (127);
}

int	handle_exit_status(int status, char *cmd)
{
	int	code;
	int	sig;

	(void)cmd;
	if (WIFEXITED(status))
	{
		code = WEXITSTATUS(status);
		return (code);
	}
	else if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)
			ft_putstr_fd("\n", STDERR_FILENO);
		else if (sig == SIGQUIT)
			ft_putstr_fd("Quit: 3\n", STDERR_FILENO);
		return (128 + sig);
	}
	return (1);
}
