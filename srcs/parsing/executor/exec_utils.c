/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julien <julien@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 21:30:30 by julien            #+#    #+#             */
/*   Updated: 2025/04/23 02:26:27 by julien           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	has_redirect(t_token *token)
{
	t_token	*cur;

	cur = token;
	while (cur)
	{
		if (cur->type == REDIR_IN || cur->type == REDIR_OUT
			|| cur->type == APPEND || cur->type == HEREDOC)
			return (1);
		cur = cur->next;
	}
	return (0);
}

char	**fill_argv(t_shell *sh, t_token *token, char **argv)
{
	t_token	*cur;
	int		i;

	i = 1;
	cur = token->next;
	while (cur)
	{
		if (cur->type == STRING || cur->type == ARG || cur->type == ENV
			|| cur->type == EXIT)
		{
			// Ne pas expanser les tokens venant de guillemets simples
			if (cur->quote_state == SINGLE_QUOTE)
				argv[i] = ft_strdup(cur->input);
			else
				argv[i] = expand_token(sh, cur);
			
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
	t_token	*cur;

	count = 1;
	cur = token->next;
	if (token->type == CMD && token->input && token->input[0] == '\0')
	{
		sh->last_exit_status = 127;
		return (NULL);
	}
	while (cur)
	{
		if (cur->type == STRING || cur->type == ARG || cur->type == ENV
			|| cur->type == EXIT)
			count++;
		cur = cur->next;
	}
	argv = malloc(sizeof(char *) * (count + 1));
	if (!argv)
		return (NULL);
	argv[0] = ft_strdup(token->input);
	return (fill_argv(sh, token, argv));
}

// Cette fonction nettoie le nom du fichier en retirant les guillemets entourant le nom, mais préserve les espaces
static char *clean_filename(char *input)
{
    char *clean;
    int i, j;
    int len;
    
    if (!input)
        return NULL;
    len = ft_strlen(input);
    clean = malloc(len + 1);
    if (!clean)
        return NULL;
    if ((len >= 2) && 
        ((input[0] == '"' && input[len - 1] == '"') || 
         (input[0] == '\'' && input[len - 1] == '\'')))
    {
        i = 1;
        j = 0;
        while (i < len - 1)
        {
            clean[j] = input[i];
            i++;
            j++;
        }
        clean[j] = '\0';
    }
    else
        ft_strlcpy(clean, input, len + 1);
    return clean;
}

int	handle_redirect(t_token *token, char *input)
{
	t_token	*cur;
	int		fd;
    char    *clean_file;
    int     ret = 0;

	(void)input;
	cur = token;
	while (cur && !ret)
	{
		if (cur->type == REDIR_IN)
		{
            if (!cur->next || !cur->next->input)
                return (ft_putstr_fd("minishell: syntax error near unexpected token\n", 2), ERR_GENERAL);
                
            // Nettoyer et ouvrir le fichier
            clean_file = clean_filename(cur->next->input);
            if (!clean_file)
                return (perror("minishell"), ERR_GENERAL);
                
            fd = open(clean_file, O_RDONLY);
            if (fd < 0)
            {
                free(clean_file);
                return (print_exec_err(cur->next->input), ERR_GENERAL);
            }
            
            if (dup2(fd, STDIN_FILENO) < 0)
            {
                close(fd);
                free(clean_file);
                return (perror("minishell"), ERR_GENERAL);
            }
            
            close(fd);
            free(clean_file);
		}
		else if (cur->type == REDIR_OUT)
		{
            if (!cur->next || !cur->next->input)
                return (ft_putstr_fd("minishell: syntax error near unexpected token\n", 2), ERR_GENERAL);
                
            // Nettoyer et ouvrir le fichier
            clean_file = clean_filename(cur->next->input);
            if (!clean_file)
                return (perror("minishell"), ERR_GENERAL);
                
            fd = open(clean_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0)
            {
                free(clean_file);
                print_exec_err(cur->next->input);
                return (ERR_GENERAL);
            }
            
            if (dup2(fd, STDOUT_FILENO) < 0)
            {
                close(fd);
                free(clean_file);
                return (perror("minishell"), ERR_GENERAL);
            }
            
            close(fd);
            free(clean_file);
		}
		else if (cur->type == APPEND)
		{
            if (!cur->next || !cur->next->input)
                return (ft_putstr_fd("minishell: syntax error near unexpected token\n", 2), ERR_GENERAL);
                
            // Nettoyer et ouvrir le fichier
            clean_file = clean_filename(cur->next->input);
            if (!clean_file)
                return (perror("minishell"), ERR_GENERAL);
                
            fd = open(clean_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd < 0)
            {
                free(clean_file);
                return (print_exec_err(cur->next->input), ERR_GENERAL);
            }
            
            if (dup2(fd, STDOUT_FILENO) < 0)
            {
                close(fd);
                free(clean_file);
                return (perror("minishell"), ERR_GENERAL);
            }
            
            close(fd);
            free(clean_file);
		}
		cur = cur->next;
	}
	return (0);
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	return (0);
}
