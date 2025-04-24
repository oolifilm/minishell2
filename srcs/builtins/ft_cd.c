/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbanchon <jbanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 14:16:05 by jbanchon          #+#    #+#             */
/*   Updated: 2025/04/24 21:35:00 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*get_home_dir(t_shell *sh);
char	*get_oldpwd(t_shell *sh);
int		update_pwd(t_shell *sh);
int		ft_cd(t_shell *sh, char **argv);

/*
PWD ne marche pas ?
CD marche
*/

int	handle_command(t_shell *sh, t_token_list *tokens)
{
	t_token	*cmd;
	char	*argv[3];
	char	*argv_pwd[2];

	cmd = tokens->head;
	if (!cmd || cmd->type != CMD)
		return (0);
	if (ft_strcmp(cmd->input, "cd") == 0)
	{
		argv[0] = "cd";
		if (cmd->next && cmd->next->type == STRING)
			argv[1] = cmd->next->input;
		else
			argv[1] = NULL;
		argv[2] = NULL;
		return (ft_cd(sh, argv));
	}
	if (ft_strcmp(cmd->input, "pwd") == 0)
	{
		argv_pwd[0] = "pwd";
		argv_pwd[1] = NULL;
		return (ft_pwd(sh, argv));
	}
	return (0);
}
