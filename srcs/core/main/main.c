/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbanchon <jbanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 11:35:29 by julien            #+#    #+#             */
/*   Updated: 2025/04/25 17:47:00 by jbanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

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
		return ;
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
	free_tokens(tokens_list);
	free(input);
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
		if (!input)
		{
			ft_putstr_fd("exit\n", STDOUT_FILENO);
			break ;
		}
		process_input(sh, input);
	}
	free_shell(sh);
	return (0);
}
