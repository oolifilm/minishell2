/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbanchon <jbanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 22:25:24 by julien            #+#    #+#             */
/*   Updated: 2025/04/17 15:22:05 by jbanchon         ###   ########.fr       */
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

	path = getenv("PATH");
	if (!path)
		return (NULL);
	i = 0;
	path_split = ft_split(path, ':');
	while (path_split[i])
	{
		tmp = ft_strjoin(path_split[i], "/");
		path_cmd = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(path_cmd, F_OK) == 0)
			return (ft_free_split(path_split), path_cmd);
		free(path_cmd);
		i++;
	}
	ft_free_split(path_split);
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

int	handle_execve_err(char *path, char **argv)
{
	if (errno == ENOENT)
	{
		printf("%s: %s\n", path, strerror(errno));
		free((void *)path);
		ft_free_split(argv);
		exit(127);
	}
	if (errno == EACCES)
	{
		printf("%s: %s\n", path, strerror(errno));
		free((void *)path);
		ft_free_split(argv);
		exit(126);
	}
	printf("%s: %s\n", path, strerror(errno));
	perror("minishell");
	free(path);
	ft_free_split(argv);
	exit(1);
}
