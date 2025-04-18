/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbanchon <jbanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 14:11:12 by leaugust          #+#    #+#             */
/*   Updated: 2025/04/18 16:59:43 by jbanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

/***********************************/
/*==========TOKEN STRUCT==========*/
/*********************************/

typedef enum e_token_type
{
	CMD,
	ARG,
	REDIR_IN,
	REDIR_OUT,
	APPEND,
	HEREDOC,
	PIPE,
	DOLLAR,
	EXIT,
	ENV,
	STRING,
	REDIR_FILE,
}					t_token_type;

typedef enum e_quote_state
{
	NO_QUOTE,
	SINGLE_QUOTE,
	DOUBLE_QUOTE,
}					t_quote_state;

typedef struct s_token
{
	char			*input;
	t_token_type	type;
	t_quote_state	quote_state;
	struct s_token	*next;
}					t_token;

typedef struct s_token_list
{
	t_token			*head;
	t_token			*cur;
	int				size;
}					t_token_list;

/************************************/
/*==========PARSER STRUCT==========*/
/**********************************/

typedef struct s_cmd
{
	char			**args;
	int				infile;
	int				outfile;
	t_token_list	*token_list;
	struct s_cmd	*next;
}					t_cmd;

typedef struct s_shell
{
	char			**env;
	int				last_exit_status;
}					t_shell;

/********************************/
/*==========TOKENIZER==========*/
/******************************/

/*=====DOUBLE_QUOTED=====*/

int					get_env_var_name(char *input, int j, char *var_name);
char				*expand_var_in_dquotes(char *str);

/*=====TOKEN_CMD=====*/

void				token_is_command(char *input, int *i, t_token_list *tokens,
						int *is_first_word);
void				assign_env_var(char *input, int *i, t_token_list *tokens);

/*=====TOKEN__IS_REDIR=====*/

void				assign_redirection(char *input, int *i,
						t_token_list *tokens);

/*=====TOKEN_OPERATORS=====*/

void				assign_pipe(char input, t_token_list *tokens);
void				assign_dollar(char *input, int *i, t_token_list *tokens);

/*=====TOKEN_QUOTES=====*/

void				handle_quoted_content(char *input, int *i,
						t_token_list *tokens, char quote_type);
int					is_token_breaker(char c);
int					ft_isspace(char c);
int					is_quote(char c);
void				assign_cmd_types(t_token *token);
char				*remove_quotes(const char *s);

/*=====TOKEN_UTILS=====*/

t_token				*new_token(char *input, t_token_type type,
						t_quote_state quote_state);
t_token				*add_token(t_token_list *tokens, char *input,
						t_token_type type, t_quote_state quoted);
void				skip_spaces(char *input, int *i);
void				free_tokens(t_token_list *tokens_list);
char				*get_token_type_str(t_token_type type);

/*=====TOKENIZER=====*/

t_token_list		*tokenize_input(char *input);
t_token_list		*init_token_list(void);

void				expand_token_list(t_shell *sh, t_token *token);
char				*expand_token(t_shell *sh, t_token *token);
char				*ft_strtrim(const char *s1, const char *set);

/******************************/
/*==========PARSING==========*/
/****************************/

int					parse_tokens(t_token_list *tokens);
int					is_invalid_first_token(t_token *head);
int					has_invalid_redirection(t_token *tokens);
int					handle_pipes(t_token *tokens);
int					has_unclosed_quote(char *input);
int					is_ignorable_input(const char *line);
int					is_ignored_char(char c);

/***************************/
/*==========EXEC==========*/
/*************************/

/*======EXEC======*/

char				**build_argv(t_shell *sh, t_token *token);
char				**fill_argv(t_shell *sh, t_token *token, char **argv);
int					exec_builtin_cmd(t_shell *sh, t_token *token, char *input);
void				exec_cmd(t_shell *sh, t_token *token, char *input);
int					exec_ext_cmd(t_shell *sh, t_token *token, char **argv);
int					run_ext_child(t_shell *sh, t_token *token, char *path,
						char **argv);
int					handle_execve_err(char *path, char **argv);
int					exec_builtin_redirect(t_shell *sh, t_token *token,
						char *input);
int					handle_redirect(t_token *token, char *input);
int					has_redirect(t_token *token);
int					ft_free_split(char **tab);
int					is_builtin(char *cmd);
char				*get_path(char *cmd);
int					exec_pipe(t_shell *sh, t_token *cmd, char *input);
int					contains_pipe(t_token *token);
int					set_exit_code(t_shell *sh, int status);
int					get_exit_code(t_shell *sh);
char				*get_env_value(char **env, const char *key);
int					handle_path_error(char *shell, char *cmd);
char				*ft_strjoin3(const char *s1, const char *s2,
						const char *s3);
int					handle_path_error(char *shell, char *cmd);
int					handle_execve_err(char *path, char **argv);

/*******************************/
/*==========BUILTINS==========*/
/*****************************/
/*=====FT_CD=====*/

int					ft_cd(t_shell *sh, char **argv);
int					handle_command(t_shell *sh, t_token_list *tokens);

/*=====FT_ECHO=====*/

int					ft_echo(t_shell *sh, char **argv);

/*=====FT_ENV=====*/

int					ft_env(t_shell *sh, char **argv);

/*=====FT_EXIT=====*/

int					ft_exit(t_shell *sh, char **argv);

/*=====FT_EXPORT=====*/

int					ft_export(t_shell *sh, char **argv);
int					add_env(t_shell *sh, const char *var);
char				**init_env(char **envp);

/*=====FT_PWD=====*/

int					ft_pwd(t_shell *sh, char **argv);

/*=====FT_UNSET=====*/

int					ft_unset(t_shell *sh, char **argv);
void				remove_env_var(t_shell *sh, char *var);
int					is_valid_identifier(const char *str);

/*=====BUILTIN_UTILS=====*/

int					is_valid_env_var(const char *var);
void				print_env_var(char *var);
int					compare_vars(const void *a, const void *b);
int					print_sorted_env(t_shell *sh);

/*=====SIGNALS=====*/

void				sigint_handler(int sig);
void				set_sig_action(void);
void				handle_eof(void);

#endif