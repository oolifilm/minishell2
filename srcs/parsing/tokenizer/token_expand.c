/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_expand.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leaugust <leaugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 16:45:07 by jbanchon          #+#    #+#             */
/*   Updated: 2025/04/18 18:10:31 by leaugust         ###   ########.fr       */
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

void expand_token_list(t_shell *sh, t_token *token)
{
    char    *key;
    char    *value;
    char    *old;

    while (token)
    {
        // Si le token commence par '$' et n'est pas entre guillemets simples
        if (token->input[0] == '$' && token->quote_state != SINGLE_QUOTE)
        {
            // Si c'est $? (statut de la dernière commande)
            if (token->input[1] == '?')
            {
                value = ft_itoa(sh->last_exit_status);
            }
            else
            {
                // Si ce n'est pas suivi d'une variable valide, on garde le '$' intact
                if (ft_isalpha(token->input[1]) || token->input[1] == '_')
                {
                    key = ft_substr(token->input, 1, ft_strlen(token->input) - 1);
                    if (!key)
                        return;

                    // On cherche la valeur de l'environnement
                    value = get_env_value(sh->env, key);
                    free(key);
                }
                else
                {
                    // Si ce n'est pas suivi d'une variable valide, gardons le '$'
                    value = ft_strdup("$");
                }
            }

            // Remplacer l'ancien input par la valeur calculée
            old = token->input;
            if (value)
                token->input = value;
            else
                token->input = ft_strdup("");  // Si pas trouvé, vide

            free(old);
            token->type = STRING;  // Assurer que le type reste STRING
        }

        // Passer au token suivant
        token = token->next;
    }
}

