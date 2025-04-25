/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julien <julien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 10:57:49 by julien            #+#    #+#             */
/*   Updated: 2025/04/25 19:20:06 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

/* Crée un nouveau token pour chaque élément
(commande, argument, opérateur, etc.). */

t_token	*new_token(char *input, t_token_type type, t_quote_state quote_state)
{
	t_token	*token;
		static char pipe_char[2] = "|";

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	if (type == PIPE && input && input[0] == '|' && input[1] == '\0')
	{
		token->input = pipe_char;
	}
	else
	{
		token->input = ft_strdup(input);
		if (!token->input)
		{
			free(token);
			return (NULL);
		}
	}
	token->type = type;
	token->quote_state = quote_state;
	token->next = NULL;
	return (token);
}

/* Ajoute un token à la liste chaînée. */

t_token	*add_token(t_token_list *tokens, char *input, t_token_type type,
		t_quote_state quoted)
{
	t_token	*new;

	new = new_token(input, type, quoted);
	if (!new)
		return (NULL);
	if (!tokens->head)
	{
		tokens->head = new;
		tokens->cur = new;
	}
	else
	{
		tokens->cur->next = new;
		tokens->cur = new;
	}
	tokens->size++;
	return (new);
}

/* Ignore les WHITE SPACE pour positionner i directement
sur le premier caractère significatif de l'entrée. */

void	skip_spaces(char *input, int *i)
{
	while (input[*i] == ' ' || input[*i] == '\t' || input[*i] == '\n')
		(*i)++;
}

void	free_tokens(t_token_list *tokens_list)
{
	t_token	*tmp;
	t_token	*tokens;

	if (!tokens_list)
		return ;
	tokens = tokens_list->head;
	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		if (tmp->type != PIPE || !tmp->input || tmp->input[0] != '|'
			|| tmp->input[1] != '\0')
			free(tmp->input);
		free(tmp);
	}
	free(tokens_list);
}

/* Renvoie une chaîne de caractères correspondant
au type de token (t_token_type) passé en paramètre. */

// char	*get_token_type_str(t_token_type type)
// {
// 	if (type == CMD)
// 		return ("CMD");
// 	else if (type == ARG)
// 		return ("ARG");
// 	else if (type == PIPE)
// 		return ("PIPE");
// 	else if (type == REDIR_IN)
// 		return ("REDIR_IN");
// 	else if (type == REDIR_OUT)
// 		return ("REDIR_OUT");
// 	else if (type == APPEND)
// 		return ("APPEND");
// 	else if (type == HEREDOC)
// 		return ("HEREDOC");
// 	else if (type == EXIT)
// 		return ("EXIT");
// 	else if (type == ENV)
// 		return ("ENV");
// 	else if (type == REDIR_FILE)
// 		return ("REDIR_FILE");
// 	else if (type == STRING)
// 		return ("STRING");
// 	else if (type == DOLLAR)
// 		return ("DOLLAR");
// 	return ("UNKNOWN");
// }
