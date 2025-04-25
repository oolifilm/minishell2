/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbanchon <jbanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 21:35:00 by julien            #+#    #+#             */
/*   Updated: 2025/04/25 18:38:15 by jbanchon         ###   ########.fr       */
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
	char	*new_pwd;

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
	new_pwd = ft_strjoin("PWD=", cwd);
	remove_env_var(sh, "PWD");
	add_env(sh, new_pwd);
	free(new_pwd);
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
	int		ret;
	int		flg;

	is_dash = 0;
	flg = 0;
	path = argv[1];
	if (!argv[1])
	{
		path = get_home_dir(sh);
		flg = (path != NULL);
	}
	else if (ft_strcmp(path, "-") == 0)
	{
		path = get_oldpwd(sh);
		is_dash = 1;
		flg = 1;
	}
	if (!path)
		return (1);
	ret = change_directory(sh, path, is_dash);
	if (flg)
		free(path);
	return (ret);
}
