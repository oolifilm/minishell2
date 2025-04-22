/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   double_quoted.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julien <julien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 10:39:41 by julien            #+#    #+#             */
/*   Updated: 2025/04/22 23:13:03 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

/* Calcule la longueur d'une chaîne entre guillemets.*/

static int	cal_quoted_len(char *input, int j)
{
	int		len;
	char	var_name[256];
	char	*env_value;

	len = 0;
	while (input[j] && input[j] != '"')
	{
		if (input[j] == '$' && input[j + 1])
		{
			j++;  // Avance après le $
			
			// Gestion spéciale de $?
			if (input[j] == '?')
			{
				// On alloue arbitrairement 3 caractères (pour les valeurs 0-999)
				len += 1;  // Typiquement "0" pour succès
				j++;
			}
			// Gestion des variables d'environnement normales
			else if (ft_isalpha(input[j]) || input[j] == '_') 
			{
				int var_len = 0;
				while (input[j] && (ft_isalnum(input[j]) || input[j] == '_') && var_len < 255)
				{
					var_name[var_len++] = input[j];
					j++;
				}
				var_name[var_len] = '\0';
				
				// Chercher la variable d'environnement
				env_value = getenv(var_name);
				if (env_value)
					len += ft_strlen(env_value);
			}
			else  // Si ce n'est ni $? ni une variable d'environnement valide
			{
				len++;  // Pour le $
				if (input[j] && input[j] != '"')
				{
					len++;  // Pour le caractère suivant
					j++;
				}
			}
		}
		else
		{
			len++;
			j++;
		}
	}
	return (len);
}

/* Remplit result avec le contenu de input. */

static void	copy_quoted_and_expand(char *input, char *result, int *j, int *len)
{
	char	var_name[256];
	char	*env_value;

	while (input[*j] && input[*j] != '"')
	{
		if (input[*j] == '$' && input[*j + 1])
		{
			(*j)++;  // Avance après le $
			
			// Gestion spéciale de $?
			if (input[*j] == '?')
			{
				// Pour simplicité, on met toujours "0" comme code de retour pour l'instant
				result[(*len)++] = '0';
				(*j)++;
			}
			// Gestion des variables d'environnement normales
			else if (ft_isalpha(input[*j]) || input[*j] == '_') 
			{
				int var_len = 0;
				while (input[*j] && (ft_isalnum(input[*j]) || input[*j] == '_') && var_len < 255)
				{
					var_name[var_len++] = input[*j];
					(*j)++;
				}
				var_name[var_len] = '\0';
				
				// Chercher la variable d'environnement
				env_value = getenv(var_name);
				if (env_value)
				{
					ft_strlcpy(result + *len, env_value, ft_strlen(env_value) + 1);
					*len += ft_strlen(env_value);
				}
			}
			else  // Si ce n'est ni $? ni une variable d'environnement valide
			{
				result[(*len)++] = '$';
				if (input[*j] && input[*j] != '"')
					result[(*len)++] = input[(*j)++];
			}
		}
		else
			result[(*len)++] = input[(*j)++];
	}
}

static void	fill_quoted_content(char *input, char *result, int *i)
{
	int	j;
	int	len;

	j = *i;
	len = 0;
	copy_quoted_and_expand(input, result, &j, &len);
	result[len] = '\0';
	*i = j;
}

/* Extrait et traite le contenu d'une chaîne entre guillemets. */

static char	*handle_double_quoted(char *input, int *i)
{
	char	*result;
	int		j;
	int		len;

	j = *i;
	len = cal_quoted_len(input, j);
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	fill_quoted_content(input, result, &j);
	*i = j;
	return (result);
}

/* Wrappe handle_double_quoted. */

char	*expand_var_in_dquotes(char *str)
{
	char	*result;
	int		i;

	i = 0;
	result = handle_double_quoted(str, &i);
	return (result);
}
