/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julien <julien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 17:46:05 by jbanchon          #+#    #+#             */
/*   Updated: 2025/04/24 23:41:58 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

/* Initialise une nouvelle liste chaînée
de tokens (t_token_list). */

t_token_list	*init_token_list(void)
{
	t_token_list	*tokens;

	tokens = (t_token_list *)malloc(sizeof(t_token_list));
	if (!tokens)
		return (NULL);
	tokens->head = NULL;
	tokens->cur = NULL;
	return (tokens);
}

void	assign_cmd_types(t_token *token)
{
	int		expecting_cmd;
	char	*cleaned_cmd;

	expecting_cmd = 1;
	while (token)
	{
		if (token->type == PIPE)
			expecting_cmd = 1;
		else if (expecting_cmd && (token->type == STRING || token->type == ENV
				|| token->type == DOLLAR))
		{
			token->type = CMD;
			cleaned_cmd = remove_quotes(token->input);
			if (cleaned_cmd)
			{
				free(token->input);
				token->input = cleaned_cmd;
			}
			expecting_cmd = 0;
		}
		else if (token->type == CMD || token->type == ARG)
			expecting_cmd = 0;
		token = token->next;
	}
}

t_token_list	*tokenize_input(char *input)
{
	t_token_list	*tokens;
	int				i;
	int				is_first_word;

	is_first_word = 1;
	tokens = NULL;
	if (!input || is_ignorable_input(input))
		return (NULL);
	if (!init_tokenizer(input, &tokens, &i))
		return (NULL);
	if (process_tokens(input, tokens, &i, &is_first_word))
	{
		free_tokens(tokens);
		return (NULL);
	}
	if (!tokens->head)
	{
		free(tokens);
		return (NULL);
	}
	assign_cmd_types(tokens->head);
	return (tokens);
}
