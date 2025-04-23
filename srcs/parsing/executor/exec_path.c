/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leaugust <leaugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 22:25:24 by julien            #+#    #+#             */
/*   Updated: 2025/04/23 15:50:54 by leaugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

char	*get_path(char *cmd)
{
	char	*path;
	char	*path_cmd;
	char	**path_split;
	int		i;
	char	*tmp;

	if (!cmd || cmd[0] == '\0')
	{
		errno = ENOENT;
		return (NULL);
	}
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK) == 0)
		{
			if (access(cmd, X_OK) == 0)
				return (ft_strdup(cmd));
			errno = EACCES;
			return (NULL);
		}
		errno = ENOENT;
		return (NULL);
	}
	path = getenv("PATH");
	if (!path)
	{
		errno = ENOENT;
		return (NULL);
	}
	path_split = ft_split(path, ':');
	if (!path_split)
	{
		errno = ENOENT;
		return (NULL);
	}
	i = 0;
	while (path_split[i])
	{
		tmp = ft_strjoin(path_split[i], "/");
		if (!tmp)
			break ;
		path_cmd = ft_strjoin(tmp, cmd);
		free(tmp);
		if (!path_cmd)
			break ;
		if (access(path_cmd, X_OK) == 0)
			return (ft_free_split(path_split), path_cmd);
		free(path_cmd);
		i++;
	}
	ft_free_split(path_split);
	errno = ENOENT;
	return (NULL);
}

int	ft_free_split(char **tab)
{
	int	i;

	if (!tab)
		return (0);
	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
	return (0);
}
