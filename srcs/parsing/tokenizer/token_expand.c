/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_expand.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbanchon <jbanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 16:45:07 by jbanchon          #+#    #+#             */
/*   Updated: 2025/04/22 15:32:08 by jbanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

char	*expand_token(t_shell *sh, t_token *token)
{
	char	*value;

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

void	expand_token_list(t_shell *sh, t_token *token)
{
	char	*key;
	char	*value;
	char	*old;

	while (token)
	{
		if (token->input[0] == '$' && token->quote_state != SINGLE_QUOTE)
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
		token = token->next;
	}
}
