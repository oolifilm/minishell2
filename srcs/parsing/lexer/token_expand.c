/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_expand.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julien <julien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 16:45:07 by jbanchon          #+#    #+#             */
/*   Updated: 2025/04/24 23:40:49 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static char	*expand_exit_token_with_text(t_shell *sh, t_token *token);

static char	*expand_exit_token(t_shell *sh, t_token *token)
{
	if (ft_strcmp(token->input, "?") == 0)
		return (ft_itoa(sh->last_exit_status));
	return (expand_exit_token_with_text(sh, token));
}

static char	*expand_exit_token_with_text(t_shell *sh, t_token *token)
{
	char	*status;
	char	*result;

	status = ft_itoa(sh->last_exit_status);
	if (!status)
		return (NULL);
	result = malloc(ft_strlen(status) + ft_strlen(token->input));
	if (!result)
	{
		free(status);
		return (NULL);
	}
	ft_strlcpy(result, status, ft_strlen(status) + 1);
	if (token->input[1] != '\0')
		ft_strlcat(result, token->input + 1, ft_strlen(status)
			+ ft_strlen(token->input));
	free(status);
	return (result);
}

static char	*expand_env_token(t_shell *sh, t_token *token)
{
	char	*value;

	value = get_env_value(sh->env, token->input);
	if (value)
		return (value);
	else
		return (ft_strdup(""));
}

char	*expand_token(t_shell *sh, t_token *token)
{
	if (token->quote_state == SINGLE_QUOTE)
		return (ft_strdup(token->input));
	if (token->type == EXIT)
		return (expand_exit_token(sh, token));
	else if (token->type == ENV)
		return (expand_env_token(sh, token));
	else if (token->type == STRING && ft_strcmp(token->input, "$") == 0)
		return (ft_strdup("$"));
	else
		return (ft_strdup(token->input));
}

/* Prototypes des fonctions dans token_expand_utils.c */
void		expand_env_var_cmd(t_shell *sh, t_token *token);
void		expand_dollar_token(t_shell *sh, t_token *token, char *value,
				char *old);
char		*get_dollar_value(t_shell *sh, t_token *token, char **key);
void		expand_dollar_var(t_shell *sh, t_token *token);

void	expand_token_list(t_shell *sh, t_token *token)
{
	while (token)
	{
		if (token->quote_state == SINGLE_QUOTE)
		{
			token = token->next;
			continue ;
		}
		if (token->type == CMD && ft_strcmp(token->input, "$?") == 0)
		{
			token = token->next;
			continue ;
		}
		if (token->type == CMD && get_env_value(sh->env, token->input))
			expand_env_var_cmd(sh, token);
		else if (token->input[0] == '$')
			expand_dollar_var(sh, token);
		else if (token->quote_state == DOUBLE_QUOTE && ft_strchr(token->input,
				'$'))
			expand_double_quoted_vars(sh, token);
		token = token->next;
	}
}
