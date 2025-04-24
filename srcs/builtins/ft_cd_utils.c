/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julien <julien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 21:35:00 by julien            #+#    #+#             */
/*   Updated: 2025/04/24 21:35:00 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*get_home_dir(t_shell *sh)
{
	char	*home;

	home = get_env_value(sh->env, "HOME");
	if (!home)
		ft_putstr_fd("cd: HOME not set\n", 2);
	return (home);
}

char	*get_oldpwd(t_shell *sh)
{
	char	*oldpwd;

	oldpwd = get_env_value(sh->env, "OLDPWD");
	if (!oldpwd)
		ft_putstr_fd("cd: OLDPWD not set\n", 2);
	return (oldpwd);
}

int	update_pwd(t_shell *sh)
{
	char	buffer[4096];
	char	*old_pwd;
	char	*cwd;

	old_pwd = get_env_value(sh->env, "PWD");
	if (!old_pwd)
		old_pwd = ft_strdup("");
	cwd = getcwd(buffer, sizeof(buffer));
	if (!cwd)
	{
		ft_putstr_fd("cd: error getting current directory: ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		free(old_pwd);
		return (1);
	}
	remove_env_var(sh, "OLDPWD");
	add_env(sh, old_pwd);
	remove_env_var(sh, "PWD");
	add_env(sh, ft_strjoin("PWD=", cwd));
	free(old_pwd);
	return (0);
}

static int	change_directory(t_shell *sh, char *path, int is_dash)
{
	if (chdir(path) == -1)
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		return (1);
	}
	if (is_dash)
		printf("%s\n", path);
	return (update_pwd(sh));
}

int	ft_cd(t_shell *sh, char **argv)
{
	char	*path;
	int		is_dash;

	is_dash = 0;
	if (!argv[1])
		path = get_home_dir(sh);
	else if (ft_strcmp(argv[1], "-") == 0)
	{
		path = get_oldpwd(sh);
		if (path)
			is_dash = 1;
	}
	else
		path = argv[1];
	if (!path)
		return (1);
	return (change_directory(sh, path, is_dash));
}
