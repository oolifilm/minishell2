/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julien <julien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 23:55:00 by julien            #+#    #+#             */
/*   Updated: 2025/04/24 23:55:00 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

/* Expande le statut de sortie ($?) */
char	*expand_exit_status(t_shell *sh, int *i)
{
	(*i)++;
	return (ft_itoa(sh->last_exit_status));
}

/* Retourne une variable vide ($) */
char	*expand_empty_var(void)
{
	return (ft_strdup("$"));
}

/* Récupère la valeur d'une variable d'environnement */
char	*get_var_value(t_shell *sh, char *var_name)
{
	char	*var_value;
	char	*result;

	var_value = get_env_value(sh->env, var_name);
	if (var_value)
		result = var_value;
	else
		result = ft_strdup("");
	return (result);
}

/* Expande une variable d'environnement */
char	*expand_var(t_shell *sh, char *line, int *i)
{
	char	*var_name;
	char	*result;
	int		start;
	int		len;

	(*i)++;
	if (line[*i] == '?')
		return (expand_exit_status(sh, i));
	start = *i;
	while (line[*i] && (ft_isalnum(line[*i]) || line[*i] == '_'))
		(*i)++;
	len = *i - start;
	if (len == 0)
		return (expand_empty_var());
	var_name = ft_substr(line, start, len);
	if (!var_name)
		return (NULL);
	result = get_var_value(sh, var_name);
	free(var_name);
	return (result);
}
