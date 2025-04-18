/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbanchon <jbanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 14:29:56 by jbanchon          #+#    #+#             */
/*   Updated: 2025/04/18 16:55:07 by jbanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int	get_exec_err_code(void)
{
	if (errno == ENOENT)
		return (127);
	if (errno == EACCES)
		return (126);
	return (1);
}

static void	print_exec_err(char *cmd)
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
