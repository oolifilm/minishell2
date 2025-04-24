/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_expand_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julien <julien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 23:45:00 by julien            #+#    #+#             */
/*   Updated: 2025/04/24 23:45:00 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

/* Traite l'expansion des variables d'environnement dans les commandes */
void	expand_env_var_cmd(t_shell *sh, t_token *token)
{
	char	*value;
	char	*old;

	value = get_env_value(sh->env, token->input);
	old = token->input;
	if (value)
		token->input = ft_strdup(value);
	else
		token->input = ft_strdup("");
	free(old);
	if (ft_strchr(token->input, '/'))
		token->type = STRING;
	else
		token->type = STRING;
}

/* Remplace le token par sa valeur après expansion */
void	expand_dollar_token(t_shell *sh, t_token *token, char *value,
	char *old)
{
	(void)sh;
	if (value)
		token->input = value;
	else
		token->input = ft_strdup("");
	free(old);
	token->type = STRING;
}

/* Récupère la valeur d'une variable d'environnement */
char	*get_dollar_value(t_shell *sh, t_token *token, char **key)
{
	char	*value;

	if (token->input[1] == '?')
		value = ft_itoa(sh->last_exit_status);
	else if (ft_isalpha(token->input[1]) || token->input[1] == '_')
	{
		*key = ft_substr(token->input, 1, ft_strlen(token->input) - 1);
		if (!*key)
			return (NULL);
		value = get_env_value(sh->env, *key);
	}
	else if (token->input[1] == '\0' || ft_isspace(token->input[1])
		|| token->input[1] == '|' || token->input[1] == '<'
		|| token->input[1] == '>')
		value = ft_strdup("$");
	else
		value = ft_strdup(token->input);
	return (value);
}

/* Traite l'expansion des variables commençant par $ */
void	expand_dollar_var(t_shell *sh, t_token *token)
{
	char	*key;
	char	*value;
	char	*old;

	key = NULL;
	value = get_dollar_value(sh, token, &key);
	if (key)
		free(key);
	if (!value && key)
		return ;
	old = token->input;
	expand_dollar_token(sh, token, value, old);
}
