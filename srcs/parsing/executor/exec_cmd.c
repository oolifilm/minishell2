/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leaugust <leaugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 21:30:28 by julien            #+#    #+#             */
/*   Updated: 2025/04/24 13:39:50 by leaugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	exec_builtin_cmd(t_shell *sh, t_token *token, char *input)
{
	char	**argv;
	char	*cmd;
	int		ret;

	(void)input;
	ret = 0;
	cmd = ft_strdup(token->input);
	if (!cmd)
		return (set_exit_code(sh, 1), perror("minishell"), 1);
	argv = build_argv(sh, token);
	if (!argv)
	{
		free(cmd);
		return (set_exit_code(sh, 1), perror("minishell"), 1);
	}
	if (ft_strcmp(cmd, "echo") == 0)
		ret = ft_echo(sh, argv);
	else if (ft_strcmp(cmd, "exit") == 0)
		ret = ft_exit(sh, argv);
	else if (ft_strcmp(cmd, "cd") == 0)
		ret = ft_cd(sh, argv);
	else if (ft_strcmp(cmd, "export") == 0)
		ret = ft_export(sh, argv);
	else if (ft_strcmp(cmd, "env") == 0)
		ret = ft_env(sh, argv);
	else if (ft_strcmp(cmd, "pwd") == 0)
		ret = ft_pwd(sh, argv);
	else if (ft_strcmp(cmd, "unset") == 0)
		ret = ft_unset(sh, argv);
	free(cmd);
	ft_free_split(argv);
	return (ret);
}

int	exec_builtin_redirect(t_shell *sh, t_token *token, char *input)
{
	pid_t	pid;
	int		status;
	int		redir_code;

	status = 0;
	pid = fork();
	if (pid < 0)
		return (set_exit_code(sh, 1), perror("minishell"), 1);
	if (pid == 0)
	{
		redir_code = handle_redirect(token, input);
		if (redir_code != 0)
			exit(redir_code);
		exit(exec_builtin_cmd(sh, token, input));
	}
	waitpid(pid, &status, 0);
	return (handle_exit_status(sh, status, token->input));
}

int	run_ext_child(t_shell *sh, t_token *token, char *path, char **argv)
{
	int	redir_code;

	redir_code = handle_redirect(token, NULL);
	if (redir_code != 0)
		exit(redir_code);
	execve(path, argv, sh->env);
	exit(handle_execve_err(path, argv));
}

int	exec_ext_cmd(t_shell *sh, t_token *token, char **argv)
{
	char	*path;
	pid_t	pid;
	int		status;
	int		exit_code;

	status = 0;
	if (!argv || !argv[0])
		return (set_exit_code(sh, ERR_GENERAL), ERR_GENERAL);
	path = get_path(argv[0]);
	if (!path)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(argv[0], STDERR_FILENO);
		if (errno == EACCES)
		{
			ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
			ft_free_split(argv);
			return (set_exit_code(sh, ERR_NOT_EXEC), ERR_NOT_EXEC);
		}
		else
		{
			ft_putstr_fd(": command not found\n", STDERR_FILENO);
			ft_free_split(argv);
			return (set_exit_code(sh, ERR_CMD_NOT_FOUND), ERR_CMD_NOT_FOUND);
		}
	}
	pid = fork();
	if (pid < 0)
		return (perror("minishell"), free(path), ft_free_split(argv),
			ERR_GENERAL);
	if (pid == 0)
		run_ext_child(sh, token, path, argv);
	else
		waitpid(pid, &status, 0);
	free(path);
	ft_free_split(argv);
	exit_code = handle_exit_status(sh, status, token->input);
	return (exit_code);
}

int	open_heredoc_file(t_token *token)
{
	char	*delimiter;
	char	*line;
	int		fd;

	// Récupérer le délimiteur du heredoc
	if (token->next)
    delimiter = token->next->input;
else
    delimiter = NULL;
	if (!delimiter)
	{
		printf("[ERROR] Missing heredoc delimiter.\n");
		return (-1);
	}

	// Ouvrir un fichier temporaire pour le heredoc
	fd = open("/tmp/heredoc_file.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("heredoc file open failed");
		return (-1);
	}

	// Lire les lignes du heredoc et les écrire dans le fichier jusqu'à ce que le délimiteur soit trouvé
	while (1)
	{
		line = readline("> ");
		if (!line)
			break;
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}

	return (fd);  // Retourne le file descriptor du heredoc
}

void	exec_cmd(t_shell *sh, t_token *token, char *input)
{
	char			**argv;
	int				ret;
	t_token_list	*tokens_list;
	int				heredoc_fd = -1; // Initialisation du fd pour le heredoc

	if (token->type != CMD)
		return ;

	// Gère les heredocs avant de commencer l'exécution
	t_token *cur = token;
while (cur)
{
	if (cur->type == HEREDOC)
	{
		heredoc_fd = open_heredoc_file(cur);
		if (heredoc_fd < 0)
		{
			sh->last_exit_status = ERR_GENERAL;
			set_exit_code(sh, ERR_GENERAL);
			return ;
		}
		break; // Si plusieurs heredocs, on gère que le premier pour l’instant
	}
	cur = cur->next;
}

	// Prépare les tokens pour le traitement suivant
	tokens_list = init_token_list();
	if (!tokens_list)
		return ;
	tokens_list->head = token;
	sh->current_tokens = tokens_list;

	// Cas de commande à exécuter
	if (contains_pipe(token))
		ret = exec_pipe(sh, token, input);
	else
	{
		expand_token_list(sh, token);
		if (is_builtin(token->input))
		{
			// Traitement des commandes intégrées (builtin)
			if (has_redirect(token))
				ret = exec_builtin_redirect(sh, token, input);
			else
				ret = exec_builtin_cmd(sh, token, input);
		}
		else
		{
			// Si c'est une commande externe, on gère la redirection
			if (access(token->input, F_OK) == 0 && !ft_strchr(token->input, '/'))
			{
				ft_putstr_fd("minishell: ", STDERR_FILENO);
				ft_putstr_fd(token->input, STDERR_FILENO);
				ft_putstr_fd(": command not found\n", STDERR_FILENO);
				sh->last_exit_status = ERR_CMD_NOT_FOUND;
				set_exit_code(sh, ERR_CMD_NOT_FOUND);
				sh->current_tokens = NULL;
				free(tokens_list);
				return ;
			}
			argv = build_argv(sh, token);
			if (!argv)
			{
				sh->current_tokens = NULL;
				free(tokens_list);
				return ;
			}

			// Gestion de la redirection de stdin pour le heredoc
			if (heredoc_fd != -1)
			{
				dup2(heredoc_fd, STDIN_FILENO);  // Redirige le stdin vers le fd heredoc
				close(heredoc_fd);  // Ferme le fd après redirection
			}

			// Exécution de la commande externe
			ret = exec_ext_cmd(sh, token, argv);
		}
	}

	// Mise à jour du statut de sortie
	sh->last_exit_status = ret;
	set_exit_code(sh, ret);
	sh->current_tokens = NULL;
	free(tokens_list);
}

