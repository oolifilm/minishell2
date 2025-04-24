/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin_helper.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julien <julien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 00:10:00 by julien            #+#    #+#             */
/*   Updated: 2025/04/25 00:10:00 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

/* Exécute la commande builtin appropriée */
int	exec_builtin_command(t_shell *sh, char *cmd, char **argv)
{
	int	ret;

	ret = 0;
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
	return (ret);
}
