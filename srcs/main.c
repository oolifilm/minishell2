/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leaugust <leaugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 11:35:29 by julien            #+#    #+#             */
/*   Updated: 2025/04/24 21:25:00 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	process_command(t_shell *sh, t_token_list *tokens_list, char *input)
{
	t_token	*tmp;

	tmp = tokens_list->head;
	while (tmp)
		tmp = tmp->next;
	if (parse_tokens(sh, tokens_list))
	{
		tmp = tokens_list->head;
		exec_cmd(sh, tmp, input);
	}
}

static void	process_input(t_shell *sh, char *input)
{
	t_token_list	*tokens_list;

	tokens_list = tokenize_input(input);
	if (!input)
	{
		free_tokens(tokens_list);
		handle_eof();
	}
	add_history(input);
	if (tokens_list && tokens_list->head)
		process_command(sh, tokens_list, input);
	else
	{
		free_tokens(tokens_list);
		free(input);
		return ;
	}
	free(input);
	free_tokens(tokens_list);
}

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	char	*prompt;
	t_shell	*sh;

	(void)argc;
	(void)argv;
	sh = NULL;
	set_sig_action();
	sh = init_shell(envp);
	if (!sh)
		return (1);
	while (1)
	{
		prompt = "minishell$> ";
		input = readline(prompt);
		process_input(sh, input);
	}
	return (0);
}
