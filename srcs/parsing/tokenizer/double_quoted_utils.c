/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   double_quoted_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julien <julien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 23:41:03 by julien            #+#    #+#             */
/*   Updated: 2025/04/22 23:42:38 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

/*
** Traite le cas d'un point d'interrogation après un signe dollar
*/
void	process_question_mark(int *j, int *len, char *result)
{
	if (result)
		result[(*len)] = '0';
	(*len)++;
	(*j)++;
}

/*
** Traite le cas d'une variable d'environnement après un signe dollar
*/
void	process_env_var(char *input, int *j, int *len, char *result)
{
	char	var_name[256];
	char	*env_value;
	int		var_len;

	var_len = 0;
	while (input[*j] && (ft_isalnum(input[*j]) || input[*j] == '_') 
		&& var_len < 255)
	{
		var_name[var_len++] = input[*j];
		(*j)++;
	}
	var_name[var_len] = '\0';
	env_value = getenv(var_name);
	if (env_value && result)
	{
		ft_strlcpy(result + *len, env_value, ft_strlen(env_value) + 1);
		*len += ft_strlen(env_value);
	}
	else if (env_value)
		*len += ft_strlen(env_value);
}

/*
** Traite le cas d'un dollar non suivi d'une variable à expandre
*/
void	process_dquoted_dollar(char *input, int *j, int *len, char *result)
{
	if (result)
		result[(*len)] = '$';
	(*len)++;
	if (input[*j] && input[*j] != '"')
	{
		if (result)
			result[(*len)] = input[*j];
		(*len)++;
		(*j)++;
	}
}
