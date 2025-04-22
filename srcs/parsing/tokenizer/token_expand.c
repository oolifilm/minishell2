/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_expand.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julien <julien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 16:45:07 by jbanchon          #+#    #+#             */
/*   Updated: 2025/04/22 23:37:15 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

char	*expand_token(t_shell *sh, t_token *token)
{
	char	*value;
	char	*result;
	char	*status;

	if (token->quote_state == SINGLE_QUOTE)
		return (ft_strdup(token->input));
	
	if (token->type == EXIT)
	{
		// Si le token est simplement "?" (représente $?), renvoyer juste le code de sortie
		if (ft_strcmp(token->input, "?") == 0)
			return (ft_itoa(sh->last_exit_status));
		
		// Sinon, il contient des caractères supplémentaires après le ? (comme ?$)
		// On combine le code de sortie avec ces caractères
		status = ft_itoa(sh->last_exit_status);
		if (!status)
			return (NULL);
			
		// Allouer de l'espace pour le résultat (code de sortie + reste du token)
		result = malloc(ft_strlen(status) + ft_strlen(token->input));
		if (!result)
		{
			free(status);
			return (NULL);
		}
		
		// Copier le code de sortie
		ft_strlcpy(result, status, ft_strlen(status) + 1);
		
		// Ajouter les caractères qui suivent le ? (si présents)
		if (token->input[1] != '\0')
			ft_strlcat(result, token->input + 1, ft_strlen(status) + ft_strlen(token->input));
			
		free(status);
		return (result);
	}
	else if (token->type == ENV)
	{
		value = getenv(token->input);
		if (value)
			return (ft_strdup(value));
		else
			return (ft_strdup(""));
	}
	else if (token->type == STRING && ft_strcmp(token->input, "$") == 0)
		return (ft_strdup("$"));
	else
		return (ft_strdup(token->input));
}

/* 
** L'implémentation de expand_double_quoted_vars a été déplacée
** dans le fichier token_expand_dquote.c pour respecter
** les contraintes de la norminette
*/

void	expand_token_list(t_shell *sh, t_token *token)
{
	char	*key;
	char	*value;
	char	*old;

	while (token)
	{
		if (token->quote_state == SINGLE_QUOTE)
		{
			token = token->next;
			continue;
		}
		if (token->type == CMD && ft_strcmp(token->input, "$?") == 0)
		{
			token = token->next;
			continue;
		}
		if (token->input[0] == '$')
		{
			if (token->input[1] == '?')
				value = ft_itoa(sh->last_exit_status);
			else
			{
				if (ft_isalpha(token->input[1]) || token->input[1] == '_')
				{
					key = ft_substr(token->input, 1, ft_strlen(token->input)
							- 1);
					if (!key)
						return ;
					value = get_env_value(sh->env, key);
					free(key);
				}
				else if (token->input[1] == '\0' || ft_isspace(token->input[1]) || 
						token->input[1] == '|' || token->input[1] == '<' || 
						token->input[1] == '>')
					value = ft_strdup("$");
				else
					value = ft_strdup(token->input);
			}
			old = token->input;
			if (value)
				token->input = value;
			else
				token->input = ft_strdup("");
			free(old);
			token->type = STRING;
		}
		else if (token->quote_state == DOUBLE_QUOTE && ft_strchr(token->input, '$'))
			expand_double_quoted_vars(sh, token);
		token = token->next;
	}
}
