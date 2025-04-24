/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leaugust <leaugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 15:27:16 by leaugust          #+#    #+#             */
/*   Updated: 2025/04/24 13:03:50 by leaugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	is_invalid_first_token(t_token *head)
{
	if (head->input && (head->input[0] == ';' || head->input[0] == '&'
			|| head->input[0] == '(' || head->input[0] == ')'))
	{
		printf("[ERROR] ;, & and () can't be the first argument.\n");
		return (1);
	}
	if (head->type == PIPE)
	{
		printf("[ERROR] Pipe cannot be the first token.\n");
		return (1);
	}
	if (head->type == REDIR_IN || head->type == REDIR_OUT
		|| head->type == APPEND || head->type == HEREDOC)
	{
		if (!head->next || (head->next->type != STRING
				&& head->next->type != REDIR_FILE && head->next->type != ENV))
		{
			printf("[ERROR] Redirection must be followed by a target.\n");
			return (1);
		}
	}
	return (0);
}

int	handle_pipes(t_token *tokens)
{
	while (tokens)
	{
		if (tokens->type == STRING)
		{
			if (!tokens->quote_state && ft_strchr(tokens->input, '|'))
			{
				printf("[ERROR] Pipe found in unquoted string.\n");
				return (1);
			}
		}
		if (tokens->type == PIPE)
		{
			if (!tokens->next)
			{
				printf("[ERROR] Pipe must be followed by a command.\n");
				return (1);
			}
			else if (tokens->next->type == PIPE)
			{
				printf("[ERROR] Found consecutive pipes.\n");
				return (1);
			}
			if (tokens->next->type != CMD)
				return (printf("[ERROR] Pipe must be followed by a command.\n"),
					1);
		}
		tokens = tokens->next;
	}
	return (0);
}

int	has_invalid_redirection(t_token *tokens)
{
	while (tokens)
	{
		if (tokens->type == REDIR_IN || tokens->type == REDIR_OUT
			|| tokens->type == APPEND || tokens->type == HEREDOC)
		{
			if (!tokens->next)
			{
				printf("[ERROR] Redirection must be followed by a target.\n");
				return (1);
			}
			if (tokens->next->type == REDIR_IN
				|| tokens->next->type == REDIR_OUT
				|| tokens->next->type == APPEND
				|| tokens->next->type == HEREDOC)
			{
				printf("[ERROR] Invalid redirection: cannot chain redirection symbols.\n");
				return (1);
			}
			if (tokens->next->type != REDIR_FILE && tokens->next->type != STRING
				&& tokens->next->type != ENV)
			{
				printf("[ERROR] Unexpected token after redirection.\n");
				return (1);
			}
			// Si on a un heredoc, il doit être suivi du bon delimitateur
			if (tokens->type == HEREDOC && (!tokens->next
					|| tokens->next->type != STRING))
			{
				printf("[ERROR] Heredoc must be followed by a delimiter.\n");
				return (1);
			}
		}
		tokens = tokens->next;
	}
	return (0);
}

int	has_unclosed_quote(char *input)
{
	int	in_single_quote;
	int	in_double_quote;

	in_single_quote = 0;
	in_double_quote = 0;
	while (*input)
	{
		if (*input == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (*input == '"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		input++;
	}
	if (in_single_quote || in_double_quote)
		return (1);
	return (0);
}

int	process_heredoc_input(char *delimiter)
{
	char	*line;
	int		fd;

	// Créer un fichier temporaire pour le heredoc
	// Tu pourrais aussi utiliser un pipe si nécessaire,
	// mais ici on utilise un fichier temporaire.
	fd = open("/tmp/heredoc_file.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("Failed to open temporary heredoc file");
		return (-1);
	}
	// Lire les lignes jusqu'à ce que le délimiteur soit trouvé
	while (1)
	{
		// Afficher l'invite de heredoc
		line = readline("> ");
		if (!line)
		{
			// En cas d'erreur ou EOF
			break ;
		}
		// Si la ligne correspond au délimiteur, sortir de la boucle
		if (strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		// Écrire la ligne dans le fichier heredoc
		write(fd, line, strlen(line));
		write(fd, "\n", 1); // Ajouter une nouvelle ligne après chaque entrée
		free(line);         // Libérer la mémoire allouée pour la ligne
	}
	return (fd); // Retourner le file descriptor du fichier temporaire
}

int handle_heredocs(t_token *token)
{
    while (token)
    {
        // Vérifie si le token est de type HEREDOC
        if (token->type == HEREDOC)
        {
            // Débogage pour voir ce qui se passe
            printf("[DEBUG] HEREDOC trouvé, type: %d\n", token->type);
            
            // Vérifie que le token suivant existe et contient un délimiteur
            char *delimiter;
if (token->next)
    delimiter = token->next->input;
else
    delimiter = NULL;
            
            // Si le délimiteur est manquant, affichage d'un message d'erreur
            if (!delimiter)
            {
                printf("[ERROR] Heredoc must be followed by a delimiter.\n");
                return 1;
            }
            
            // Débogage pour vérifier le délimiteur
            printf("[DEBUG] Délimiteur trouvé: %s\n", delimiter);

            // Traitement du heredoc
            int fd = process_heredoc_input(delimiter);  // Gère l'input et retourne le fd
            if (fd < 0)
            {
                return 1;  // Échec lors du traitement de l'input
            }

            // Assigner le file descriptor au token
            token->heredoc_fd = fd;
        }
        token = token->next;  // Passer au token suivant
    }
    return 0;
}


int	parse_tokens(t_shell *sh, t_token_list *tokens)
{
	if (!tokens || !tokens->head)
	{
		set_exit_code(sh, 0);
		return (0);
	}
	if (is_invalid_first_token(tokens->head))
	{
		set_exit_code(sh, 2);
		return (0);
	}
	if (handle_pipes(tokens->head))
	{
		set_exit_code(sh, 2);
		return (0);
	}
	if (has_invalid_redirection(tokens->head))
	{
		set_exit_code(sh, 2);
		return (0);
	}
	// Gérer les heredocs avant de passer à l'exécution
	if (handle_heredocs(tokens->head))
	{
		set_exit_code(sh, 2);
		return (0);
	}
	return (1);
}


/*
char *delimiter;
if (token->next)
    delimiter = token->next->input;
else
    delimiter = NULL;

	*/