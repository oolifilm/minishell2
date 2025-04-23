/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leaugust <leaugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 15:27:16 by leaugust          #+#    #+#             */
/*   Updated: 2025/04/23 12:51:03 by leaugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	is_invalid_first_token(t_token *head)
{
	if (head->input && (head->input[0] == ';' || head->input[0] == '&'
			|| head->input[0] == '(' || head->input[0] == ')'))
	{
		printf("[ERROR] ;, & and () can't be the first argument.\n");
		return (1);
	}
	if (head->type == PIPE)
	{
		printf("[ERROR] Pipe cannot be the first token.\n");
		return (1);
	}
	if (head->type == REDIR_IN || head->type == REDIR_OUT
		|| head->type == APPEND || head->type == HEREDOC)
	{
		if (!head->next || (head->next->type != STRING
				&& head->next->type != REDIR_FILE && head->next->type != ENV))
		{
			printf("[ERROR] Redirection must be followed by a target.\n");
			return (1);
		}
	}
	return (0);
}

int	handle_pipes(t_token *tokens)
{
	while (tokens)
	{
		if (tokens->type == STRING)
		{
			if (!tokens->quote_state && ft_strchr(tokens->input, '|'))
			{
				printf("[ERROR] Pipe found in unquoted string.\n");
				return (1);
			}
		}
		if (tokens->type == PIPE)
		{
			if (!tokens->next)
			{
				printf("[ERROR] Pipe must be followed by a command.\n");
				return (1);
			}
			else if (tokens->next->type == PIPE)
			{
				printf("[ERROR] Found consecutive pipes.\n");
				return (1);
			}
			if (tokens->next->type != CMD)
				return (printf("[ERROR] Pipe must be followed by a command.\n"),
					1);
		}
		tokens = tokens->next;
	}
	return (0);
}

int	has_invalid_redirection(t_token *tokens)
{
	while (tokens)
	{
		if (tokens->type == REDIR_IN || tokens->type == REDIR_OUT
			|| tokens->type == APPEND || tokens->type == HEREDOC)
		{
			if (!tokens->next)
			{
				printf("[ERROR] Redirection must be followed by a target.\n");
				return (1);
			}
			if (tokens->next->type == REDIR_IN
				|| tokens->next->type == REDIR_OUT
				|| tokens->next->type == APPEND
				|| tokens->next->type == HEREDOC)
			{
				printf("[ERROR] Invalid redirection: cannot chain redirection symbols.\n");
				return (1);
			}
			if (tokens->next->type != REDIR_FILE && tokens->next->type != STRING
				&& tokens->next->type != ENV)
			{
				printf("[ERROR] Unexpected token after redirection.\n");
				return (1);
			}
		}
		tokens = tokens->next;
	}
	return (0);
}

int	has_unclosed_quote(char *input)
{
	int	in_single_quote;
	int	in_double_quote;

	in_single_quote = 0;
	in_double_quote = 0;
	while (*input)
	{
		if (*input == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (*input == '"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		input++;
	}
	if (in_single_quote || in_double_quote)
		return (1);
	return (0);
}

int	parse_tokens(t_shell *sh, t_token_list *tokens)
{
	if (!tokens || !tokens->head)
	{
		set_exit_code(sh, 0);
		return (0);
	}
	if (is_invalid_first_token(tokens->head))
	{
		set_exit_code(sh, 2);
		return (0);
	}
	if (handle_pipes(tokens->head))
	{
		set_exit_code(sh, 2);
		return (0);
	}
	if (has_invalid_redirection(tokens->head))
	{
		set_exit_code(sh, 2);
		return (0);
	}
	return (1);
}
