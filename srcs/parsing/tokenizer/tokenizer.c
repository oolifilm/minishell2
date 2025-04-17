/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leaugust <leaugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 17:46:05 by jbanchon          #+#    #+#             */
/*   Updated: 2025/04/17 18:35:52 by leaugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	is_ignored_char(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == ':' || c == '!');
}

int	is_ignorable_input(const char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (!is_ignored_char(line[i]))
			return (0);
		i++;
	}
	return (1);
}

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

/* Analyse la chaîne de caractères input et la
transforme en une liste chaînée de tokens (t_token_list).*/

static int	init_tokenizer(char *input, t_token_list **tokens, int *i)
{
	if (has_unclosed_quote(input))
	{
		printf("[ERROR] Lexer found an unclosed quote.\n");
		return (0);
	}
	*tokens = init_token_list();
	*i = 0;
	skip_spaces(input, i);
	return (1);
}

static void	process_tokens(char *input, t_token_list *tokens, int *i,
		int *is_first_word)
{
	size_t			len;
	char			buffer[1024];
	int				buffer_len;
	int				in_quotes;
	char			quote_type;
	char			*processed;
	t_quote_state	current_quote_state;

	len = ft_strlen(input);
	buffer_len = 0;
	in_quotes = 0;
	quote_type = 0;
	buffer[0] = '\0';
	(void)is_first_word;
	while (*i < (int)len)
	{
		if (in_quotes)
		{
			if (input[*i] == quote_type)
				in_quotes = 0;
			buffer[buffer_len++] = input[*i];
			(*i)++;
			continue ;
		}
		if (input[*i] == '\'' || input[*i] == '"')
		{
			in_quotes = 1;
			quote_type = input[*i];
			buffer[buffer_len++] = input[*i];
			(*i)++;
			continue ;
		}
		if (input[*i] == '$')
		{
			if (buffer_len > 0)
			{
				buffer[buffer_len] = '\0';
				processed = remove_quotes(buffer);
				current_quote_state = NO_QUOTE;
				if (quote_type == '\'')
					current_quote_state = SINGLE_QUOTE;
				else if (quote_type == '"')
					current_quote_state = DOUBLE_QUOTE;
				add_token(tokens, processed, STRING, current_quote_state);
				buffer_len = 0;
			}
			assign_dollar(input, i, tokens);
		}
		else if (input[*i] == '|')
		{
			if (buffer_len > 0)
			{
				buffer[buffer_len] = '\0';
				processed = remove_quotes(buffer);
				current_quote_state = NO_QUOTE;
				if (quote_type == '\'')
					current_quote_state = SINGLE_QUOTE;
				else if (quote_type == '"')
					current_quote_state = DOUBLE_QUOTE;
				add_token(tokens, processed, STRING, current_quote_state);
				buffer_len = 0;
			}
			assign_pipe(input[*i], tokens);
			(*i)++;
		}
		else if (input[*i] == '<' || input[*i] == '>')
		{
			if (buffer_len > 0)
			{
				buffer[buffer_len] = '\0';
				processed = remove_quotes(buffer);
				current_quote_state = NO_QUOTE;
				if (quote_type == '\'')
					current_quote_state = SINGLE_QUOTE;
				else if (quote_type == '"')
					current_quote_state = DOUBLE_QUOTE;
				add_token(tokens, processed, STRING, current_quote_state);
				buffer_len = 0;
			}
			assign_redirection(input, i, tokens);
		}
		else if (ft_isspace(input[*i]))
		{
			if (buffer_len > 0)
			{
				buffer[buffer_len] = '\0';
				processed = remove_quotes(buffer);
				current_quote_state = NO_QUOTE;
				if (quote_type == '\'')
					current_quote_state = SINGLE_QUOTE;
				else if (quote_type == '"')
					current_quote_state = DOUBLE_QUOTE;
				add_token(tokens, processed, STRING, current_quote_state);
				buffer_len = 0;
			}
			skip_spaces(input, i);
		}
		else
		{
			if (input[*i] == '\\' && input[*i + 1])
			{
				(*i)++;
				buffer[buffer_len++] = input[*i];
				(*i)++;
			}
			else
			{
				buffer[buffer_len++] = input[*i];
				(*i)++;
			}
		}
	}
	if (buffer_len > 0)
	{
		buffer[buffer_len] = '\0';
		processed = remove_quotes(buffer);
		current_quote_state = NO_QUOTE;
		if (quote_type == '\'')
			current_quote_state = SINGLE_QUOTE;
		else if (quote_type == '"')
			current_quote_state = DOUBLE_QUOTE;
		add_token(tokens, processed, STRING, current_quote_state);
	}
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
	process_tokens(input, tokens, &i, &is_first_word);
	if (!tokens->head)
	{
		free(tokens);
		return (NULL);
	}
	assign_cmd_types(tokens->head);
	return (tokens);
}
