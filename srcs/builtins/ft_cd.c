/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbanchon <jbanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 14:16:05 by jbanchon          #+#    #+#             */
/*   Updated: 2025/04/18 17:08:50 by jbanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
** get_home_dir : Récupère le chemin du répertoire HOME de l'utilisateur
** @return : Le chemin du HOME ou NULL si la variable HOME n'est pas définie
** Description :
** - Utilise getenv pour obtenir la valeur de la variable d'environnement HOME
** - Affiche une erreur si HOME n'est pas défini
*/

static char	*get_home_dir(t_shell *sh)
{
	char	*home;

	home = get_env_value(sh->env, "HOME");
	if (!home)
		ft_putstr_fd("cd: HOME not set\n", 2);
	return (home);
}

/*
** get_oldpdw : Récupère le chemin du dernier répertoire visité (OLDPWD)
** @return : Le chemin du OLDPWD ou NULL si la variable OLDPWD n'est pas définie
** Description :
** - Utilise getenv pour obtenir la valeur de la variable d'environnement OLDPWD
** - Affiche une erreur si OLDPWD n'est pas défini
** - Utilisé pour la commande 'cd -'
*/

static char	*get_oldpwd(t_shell *sh)
{
	char	*oldpwd;

	oldpwd = get_env_value(sh->env, "OLDPWD");
	if (!oldpwd)
		ft_putstr_fd("cd: OLDPWD not set\n", 2);
	return (oldpwd);
}

/*
** update_pdw : Met à jour les variables d'environnement PWD et OLDPWD
** @return : 0 en cas de succès, 1 en cas d'erreur
** Description :
** - Obtient le répertoire courant avec getcwd
** - Met à jour OLDPWD avec l'ancienne valeur de PWD
** - Met à jour PWD avec le nouveau répertoire courant
** - Gère les erreurs potentielles de getcwd et setenv
*/

static int	update_pwd(t_shell *sh)
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

/*
** ft_cd : Change le répertoire courant
** @param argv : Tableau de chaînes contenant la commande et ses arguments
** @return : 0 en cas de succès, 1 en cas d'erreur
** Description :
** - Sans argument : change vers le répertoire HOME
** - Avec '-' : change vers le dernier répertoire visité (OLDPWD)
** - Avec un chemin : change vers le chemin spécifié (relatif ou absolu)
** - Met à jour PWD et OLDPWD après chaque changement réussi
*/

int	ft_cd(t_shell *sh, char **argv)
{
	char	*path;
	int		is_dash;

	is_dash = 0;
	if (!argv[1])
		path = get_home_dir(sh);
	else if (argv[2])
		return (ft_putstr_fd("cd: too many arguments\n", 2), 1);
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
