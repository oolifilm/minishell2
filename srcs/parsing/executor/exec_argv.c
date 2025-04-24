/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_argv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leaugust <leaugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 22:35:00 by julien            #+#    #+#             */
/*   Updated: 2025/04/24 22:35:00 by leaugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

/* Prototypes des fonctions déplacées dans exec_argv_utils.c */
int		should_skip_token(t_token *cur, int *skip_next);
int		is_arg_token(t_token *cur);
char	*get_arg_value(t_shell *sh, t_token *cur);
int		count_args(t_token *token);

char	**fill_argv(t_shell *sh, t_token *token, char **argv)
{
	t_token	*cur;
	int		i;
	int		skip_next;

	i = 1;
	skip_next = 0;
	cur = token->next;
	while (cur)
	{
		if (should_skip_token(cur, &skip_next))
		{
			cur = cur->next;
			continue ;
		}
		if (is_arg_token(cur))
		{
			argv[i] = get_arg_value(sh, cur);
			i++;
		}
		cur = cur->next;
	}
	argv[i] = NULL;
	return (argv);
}

char	**build_argv(t_shell *sh, t_token *token)
{
	char	**argv;
	int		count;

	if (token->type == CMD && token->input && token->input[0] == '\0')
	{
		sh->last_exit_status = 127;
		return (NULL);
	}
	count = count_args(token);
	argv = malloc(sizeof(char *) * (count + 1));
	if (!argv)
		return (NULL);
	argv[0] = ft_strdup(token->input);
	return (fill_argv(sh, token, argv));
}
