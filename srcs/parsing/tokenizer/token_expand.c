/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_expand.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julien <julien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 16:45:07 by jbanchon          #+#    #+#             */
/*   Updated: 2025/04/22 22:43:23 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

char	*expand_token(t_shell *sh, t_token *token)
{
	char	*value;

	// Ne pas expanser les variables dans les guillemets simples
	if (token->quote_state == SINGLE_QUOTE)
		return (ft_strdup(token->input));
	
	if (token->type == EXIT)
		return (ft_itoa(sh->last_exit_status));
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
		// Ne jamais expanser les variables dans des guillemets simples
		if (token->quote_state == SINGLE_QUOTE)
		{
			token = token->next;
			continue;
		}
		
		// Skip expansion for $? when it's a command
		if (token->type == CMD && ft_strcmp(token->input, "$?") == 0)
		{
			token = token->next;
			continue;
		}
		
		if (token->input[0] == '$')
		{
			if (token->input[1] == '?')
			{
				value = ft_itoa(sh->last_exit_status);
			}
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
				else
					value = ft_strdup("$");
			}
			old = token->input;
			if (value)
				token->input = value;
			else
				token->input = ft_strdup("");
			free(old);
			token->type = STRING;
		}
		// Expansion des variables dans les guillemets doubles
		else if (token->quote_state == DOUBLE_QUOTE && ft_strchr(token->input, '$'))
		{
			expand_double_quoted_vars(sh, token);
		}
		
		token = token->next;
	}
}
