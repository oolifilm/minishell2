/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julien <julien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 22:25:24 by julien            #+#    #+#             */
/*   Updated: 2025/04/25 00:38:28 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static char	*check_direct_path(char *cmd)
{
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK | X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	return (NULL);
}

static char	*get_path_var(t_shell *sh)
{
	int		i;
	char	*path_var;

	path_var = NULL;
	i = 0;
	while (sh->env[i])
	{
		if (ft_strncmp(sh->env[i], "PATH=", 5) == 0)
		{
			path_var = sh->env[i] + 5;
			break ;
		}
		i++;
	}
	return (path_var);
}

static char	*try_path(char *dir, char *cmd)
{
	char	*tmp;
	char	*path_cmd;

	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	path_cmd = ft_strjoin(tmp, cmd);
	free(tmp);
	if (!path_cmd)
		return (NULL);
	if (access(path_cmd, F_OK | X_OK) == 0)
		return (path_cmd);
	free(path_cmd);
	return (NULL);
}

static char	*search_in_path_dirs(char **path_dirs, char *cmd)
{
	int		i;
	char	*path_cmd;

	i = 0;
	while (path_dirs[i])
	{
		path_cmd = try_path(path_dirs[i], cmd);
		if (path_cmd)
		{
			ft_free_split(path_dirs);
			return (path_cmd);
		}
		i++;
	}
	ft_free_split(path_dirs);
	return (NULL);
}

char	*get_path(t_shell *sh, char *cmd)
{
	char	*direct_path;
	char	*path_var;
	char	**path_dirs;

	direct_path = check_direct_path(cmd);
	if (direct_path)
		return (direct_path);
	path_var = get_path_var(sh);
	if (!path_var || !*path_var)
		return (NULL);
	path_dirs = ft_split(path_var, ':');
	if (!path_dirs)
		return (NULL);
	return (search_in_path_dirs(path_dirs, cmd));
}
