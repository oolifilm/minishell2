/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_print.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julien <julien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:22:45 by julien            #+#    #+#             */
/*   Updated: 2025/04/24 22:45:08 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static void	print_env_var_with_value(char *var, char *equal_sign)
{
	char	*var_name;
	size_t	name_len;

	name_len = equal_sign - var;
	var_name = malloc(name_len + 1);
	if (!var_name)
		return ;
	ft_strncpy(var_name, var, name_len);
	var_name[name_len] = '\0';
	write(1, "export ", 11);
	write(1, var_name, name_len);
	write(1, "=\"", 2);
	write(1, equal_sign + 1, ft_strlen(equal_sign + 1));
	write(1, "\"\n", 2);
	free(var_name);
}

void	print_env_var(char *var)
{
	char	*equal_sign;

	equal_sign = ft_strchr(var, '=');
	if (equal_sign)
		print_env_var_with_value(var, equal_sign);
	else
	{
		write(1, "export", 11);
		write(1, var, ft_strlen(var));
		write(1, "\n", 1);
	}
}
