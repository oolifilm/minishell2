/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leaugust <leaugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 01:03:20 by julien            #+#    #+#             */
/*   Updated: 2025/04/23 15:54:49 by leaugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Compare deux chaînes de caractères et retourne 1 si elles sont égales
int	ft_strequ(const char *s1, const char *s2)
{
	if (!s1 || !s2)
		return (0);
	while (*s1 && *s2)
	{
		if (*s1 != *s2)
			return (0);
		s1++;
		s2++;
	}
	return (*s1 == *s2);
}

// Traite un token provenant d'information de token (avec quote_state)
void	process_token_with_info(t_shell *sh, char *token,
		t_token **cur_token_ptr)
{
	if ((*cur_token_ptr)->quote_state == SINGLE_QUOTE)
	{
		ft_putstr_fd(token, STDOUT_FILENO);
		*cur_token_ptr = (*cur_token_ptr)->next;
	}
	else if (ft_strcmp(token, "$") == 0)
	{
		ft_putstr_fd("$", STDOUT_FILENO);
		*cur_token_ptr = (*cur_token_ptr)->next;
	}
	else if (token[0] == '$')
	{
		handle_dollar_token(sh, token, 1);
		*cur_token_ptr = (*cur_token_ptr)->next;
	}
	else
	{
		ft_putstr_fd(token, STDOUT_FILENO);
		*cur_token_ptr = (*cur_token_ptr)->next;
	}
}

// Traite un token sans information associée
void	process_token_without_info(t_shell *sh, char *token)
{
	char	*content;

	if (token[0] == '\'' && token[ft_strlen(token) - 1] == '\'')
	{
		content = ft_substr(token, 1, ft_strlen(token) - 2);
		ft_putstr_fd(content, STDOUT_FILENO);
		free(content);
	}
	else if (ft_strcmp(token, "$") == 0)
		ft_putstr_fd("$", STDOUT_FILENO);
	else if (token[0] == '$')
		handle_dollar_token(sh, token, 0);
	else
		ft_putstr_fd(token, STDOUT_FILENO);
}

// Détermine si un espace doit être ajouté entre les tokens
int	should_add_space(char **argv, int i)
{
	if (!argv[i + 1])
		return (0);
	if (ft_strequ(argv[i], "[") || ft_strequ(argv[i + 1], "]")
		|| ft_strequ(argv[i + 1], "%"))
		return (0);
	if (argv[i][0] == '$' && argv[i][1] == '?' && argv[i][2] == '\0' && argv[i
		+ 1][0] == '$')
		return (0);
	if ((argv[i][0] == '$' && argv[i][1] != '\0' && !ft_isalpha(argv[i][1])
			&& argv[i][1] != '_' && argv[i][1] != '?') || (argv[i + 1][0] == '$'
			&& argv[i + 1][1] != '\0' && !ft_isalpha(argv[i + 1][1]) && argv[i
			+ 1][1] != '_' && argv[i + 1][1] != '?'))
		return (0);
	return (1);
}

// Trouve le premier token après la commande et les options
t_token	*find_first_valid_token(t_shell *sh, int i)
{
	t_token	*cur_token;

	cur_token = NULL;
	if (sh->current_tokens && sh->current_tokens->head)
	{
		cur_token = sh->current_tokens->head;
		if (cur_token)
			cur_token = cur_token->next;
		while (i > 1 && cur_token && is_echo_option_n(cur_token->input))
		{
			cur_token = cur_token->next;
			i--;
		}
	}
	return (cur_token);
}
