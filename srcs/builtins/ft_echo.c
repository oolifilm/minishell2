/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julien <julien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 21:28:18 by julien            #+#    #+#             */
/*   Updated: 2025/04/23 01:05:46 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Vérifie si l'argument est l'option -n
int	is_echo_option_n(char *arg)
{
	int	i;

	if (!arg || arg[0] != '-')
		return (0);
	i = 1;
	if (!arg[i])
		return (0);
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

// Traite un token qui commence par $ avec gestion de $?, $VAR, etc.
void	handle_dollar_token(t_shell *sh, char *token, int is_with_token)
{
	char	*expanded_value;

	if (ft_isalpha(token[1]) || token[1] == '_')
	{
		expanded_value = get_env_value(sh->env, token + 1);
		if (expanded_value)
		{
			ft_putstr_fd(expanded_value, STDOUT_FILENO);
			free(expanded_value);
		}
		else if (!is_with_token)
			ft_putstr_fd("$", STDOUT_FILENO);
	}
	else if (token[1] == '?') 
	{
		char *exit_status = ft_itoa(sh->last_exit_status);
		ft_putstr_fd(exit_status, STDOUT_FILENO);
		free(exit_status);
		if (token[2] != '\0')
			ft_putstr_fd(token + 2, STDOUT_FILENO);
	}
	else
		ft_putstr_fd(token, STDOUT_FILENO);
}

// Traite un token avec ses règles spécifiques (guillemets simples, etc.)
void	process_token(t_shell *sh, char *token, t_token **cur_token_ptr)
{
	if (*cur_token_ptr && ft_strcmp((*cur_token_ptr)->input, token) == 0)
		process_token_with_info(sh, token, cur_token_ptr);
	else
		process_token_without_info(sh, token);
}

// Process arguments pour la commande echo
int	process_echo_args(t_shell *sh, char **argv, int i, int newline)
{
	t_token *cur_token;
	
	cur_token = find_first_valid_token(sh, i);
	while (argv[i])
	{
		process_token(sh, argv[i], &cur_token);
		
		if (should_add_space(argv, i))
			ft_putstr_fd(" ", STDOUT_FILENO);
		i++;
	}
	if (newline)
		ft_putstr_fd("\n", STDOUT_FILENO);
	return (0);
}

// La fonction principale echo
int	ft_echo(t_shell *sh, char **argv)
{
	int		i;
	int		newline;
	
	i = 1;
	newline = 1;
	while (argv[i] && is_echo_option_n(argv[i]))
	{
		newline = 0;
		i++;
	}
	process_echo_args(sh, argv, i, newline);
	sh->last_exit_status = 0;
	return (0);
}
