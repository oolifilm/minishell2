/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   double_quoted_len.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julien <julien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 23:41:35 by julien            #+#    #+#             */
/*   Updated: 2025/04/22 23:42:25 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

/*
** Calcule la longueur d'une variable d'environnement
*/
static int	calc_env_var_len(char *input, int *j)
{
	char	var_name[256];
	char	*env_value;
	int		var_len;
	int		len;

	var_len = 0;
	len = 0;
	while (input[*j] && (ft_isalnum(input[*j]) || input[*j] == '_') 
		&& var_len < 255)
	{
		var_name[var_len++] = input[*j];
		(*j)++;
	}
	var_name[var_len] = '\0';
	env_value = getenv(var_name);
	if (env_value)
		len += ft_strlen(env_value);
	return (len);
}

/*
** Calcule la longueur d'un token après expansion pour le cas du $
*/
static int	calc_dollar_len(char *input, int *j)
{
	int	len;

	len = 1;
	if (input[*j] && input[*j] != '"')
	{
		len++;
		(*j)++;
	}
	return (len);
}

/*
** Calcule la longueur d'une chaîne entre guillemets en gérant les différents cas d'expansion
*/
static int	calc_quoted_expansion_len(char *input, int *j)
{
	int	len;

	len = 0;
	if (input[*j] == '?')
	{
		len += 1;
		(*j)++;
	}
	else if (ft_isalpha(input[*j]) || input[*j] == '_')
		len += calc_env_var_len(input, j);
	else
		len += calc_dollar_len(input, j);
	return (len);
}

/*
** Calcule la longueur d'une chaîne entre guillemets.
*/
int	cal_quoted_len(char *input, int j)
{
	int	len;

	len = 0;
	while (input[j] && input[j] != '"')
	{
		if (input[j] == '$' && input[j + 1])
		{
			j++;
			len += calc_quoted_expansion_len(input, &j);
		}
		else
		{
			len++;
			j++;
		}
	}
	return (len);
}
