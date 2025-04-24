/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julien <julien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 21:25:00 by julien            #+#    #+#             */
/*   Updated: 2025/04/24 22:40:00 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int	init_argv_array(t_token_list *tokens, char ***argv)
{
	*argv = malloc(sizeof(char *) * (tokens->size + 2));
	if (!*argv)
		return (0);
	(*argv)[0] = ft_strdup(tokens->head->input);
	if (!(*argv)[0])
	{
		free(*argv);
		return (0);
	}
	return (1);
}

static int	add_token_to_argv(char **argv, int argc, char *token_input)
{
	argv[argc] = ft_strdup(token_input);
	if (!argv[argc])
	{
		while (--argc >= 0)
			free(argv[argc]);
		free(argv);
		return (0);
	}
	return (1);
}

char	**create_argv_from_input(t_token_list *tokens)
{
	char	**argv;
	int		argc;
	t_token	*tmp;

	if (!init_argv_array(tokens, &argv))
		return (NULL);
	tmp = tokens->head->next;
	argc = 1;
	while (tmp)
	{
		if (tmp->input)
		{
			if (!add_token_to_argv(argv, argc, tmp->input))
				return (NULL);
			argc++;
		}
		tmp = tmp->next;
	}
	argv[argc] = NULL;
	return (argv);
}
