# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: julien <julien@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/22 14:08:51 by leaugust          #+#    #+#              #
#    Updated: 2025/04/25 16:51:36 by julien           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	minishell
CC			=	cc -g3
CFLAGS		=	-Wall -Wextra -Werror -I/opt/homebrew/opt/readline/include
LIBFTDIR 	=	libft
LDFLAGS 	=	-lreadline -L$(LIBFTDIR) -lft -L/opt/homebrew/opt/readline/lib

# Sources pour le lexer (anciennement tokenizer)
SRC_LEXER	=	srcs/parsing/lexer/tokenizer.c \
				srcs/parsing/lexer/tokenizer_buffer.c \
				srcs/parsing/lexer/tokenizer_chars.c \
				srcs/parsing/lexer/tokenizer_chars_utils.c \
				srcs/parsing/lexer/tokenizer_process.c \
				srcs/parsing/lexer/tokenizer_utils.c \
				srcs/parsing/lexer/token_utils.c \
				srcs/parsing/lexer/token_operators.c \
				srcs/parsing/lexer/token_operators_utils.c \
				srcs/parsing/lexer/token_quotes.c \
				srcs/parsing/lexer/token_quotes_utils.c \
				srcs/parsing/lexer/token_quotes_dquote.c \
				srcs/parsing/lexer/token_command.c \
				srcs/parsing/lexer/token_command_utils.c \
				srcs/parsing/lexer/double_quoted.c \
				srcs/parsing/lexer/double_quoted_len.c \
				srcs/parsing/lexer/double_quoted_utils.c \
				srcs/parsing/lexer/double_quoted_copy.c \
				srcs/parsing/lexer/token_is_redir.c \
				srcs/parsing/lexer/token_is_redir_utils.c \
				srcs/parsing/lexer/token_expand.c \
				srcs/parsing/lexer/token_expand_utils.c \
				srcs/parsing/lexer/token_expand_dquote.c \
				srcs/parsing/lexer/token_expand_dquote2.c \
				srcs/parsing/lexer/quotes_utils.c

# Sources pour le parser
SRC_PARSER	=	srcs/parsing/parser/parsing_utils.c \
				srcs/parsing/parser/parsing.c \
				srcs/parsing/parser/parsing_validation.c \
				srcs/parsing/parser/parsing_redirection.c \
				srcs/parsing/parser/heredoc.c \
				srcs/parsing/parser/heredoc_input.c \
				srcs/parsing/parser/heredoc_utils.c

# Sources pour l'expander
SRC_EXPANDER =	srcs/parsing/expander/expand.c \
				srcs/parsing/expander/expand_utils.c \
				srcs/parsing/expander/expand_var.c \
				srcs/parsing/expander/expand_null_utils.c

# Sources pour l'exécuteur
SRC_EXECUTOR =	srcs/execution/executor/exec_argv.c \
				srcs/execution/executor/exec_argv_utils.c \
				srcs/execution/executor/exec_cmd.c \
				srcs/execution/executor/exec_cmd_utils.c \
				srcs/execution/executor/exec_builtin_helper.c \
				srcs/execution/executor/exec_heredoc.c \
				srcs/execution/executor/exec_utils.c \
				srcs/execution/executor/exec_builtin.c \
				srcs/execution/executor/exec_path.c \
				srcs/execution/executor/exit_status.c \
				srcs/execution/executor/handle_error.c

# Sources pour les redirections
SRC_REDIRECT =	srcs/execution/redirections/exec_redirect.c \
				srcs/execution/redirections/exec_redirect_utils.c

# Sources pour les pipes
SRC_PIPES	=	srcs/execution/pipes/exec_pipe.c \
				srcs/execution/pipes/exec_pipe_utils.c

# Sources pour les builtins
SRC_BUILTINS =	srcs/builtins/ft_cd.c \
				srcs/builtins/ft_cd_utils.c \
				srcs/builtins/ft_echo.c \
				srcs/builtins/ft_echo_utils.c \
				srcs/builtins/ft_env.c \
				srcs/builtins/ft_exit.c \
				srcs/builtins/export/ft_export.c \
				srcs/builtins/export/ft_export_env.c \
				srcs/builtins/ft_pwd.c \
				srcs/builtins/ft_unset.c \
				srcs/builtins/ft_unset_utils.c \
				srcs/builtins/export/ft_export_utils.c \
				srcs/builtins/export/ft_export_print.c \
				srcs/builtins/export/ft_export_sort.c

# Sources pour le core
SRC_CORE	=	srcs/core/signals/signals.c \
				srcs/core/main/main.c \
				srcs/core/main/main_utils.c \
				srcs/core/init/shell_init.c

# Tous les fichiers source
SRC			=	$(SRC_LEXER) \
				$(SRC_PARSER) \
				$(SRC_EXPANDER) \
				$(SRC_EXECUTOR) \
				$(SRC_REDIRECT) \
				$(SRC_PIPES) \
				$(SRC_BUILTINS) \
				$(SRC_CORE)

OBJ 		= 	$(SRC:.c=.o)

LIBFTDIR 	=	libft
LIBFT		=	$(LIBFTDIR)/libft.a

ROSE		=	\033[1;38;5;206m
LILA		=	\033[1;38;5;177m
NC			=	\033[0m

all: $(NAME)

$(LIBFT):
	@echo "$(LILA)COMPILATION DE LA LIBFT$(NC)"
	@make -C $(LIBFTDIR) --silent
	@echo "$(ROSE)LIBFT CRÉÉ$(NC)"

$(NAME): $(LIBFT) $(OBJ)
	@echo "$(LILA)COMPILATION DE MINISHELL$(NC)"
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LDFLAGS)
	@echo "$(ROSE)MINISHELL CRÉÉ$(NC)"

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "$(LILA)SUPPRESSION DES OBJECTS DANS MINISHELL$(NC)"
	@rm -f $(OBJ)
	@echo "$(ROSE)DONE$(NC)"

fclean: clean
	@echo "$(LILA)SUPPRESSION DE L'EXÉCUTABLE DE MINISHELL$(NC)"
	@rm -f $(NAME)
	@make -C $(LIBFTDIR) fclean --silent
	@echo "$(ROSE)DONE$(NC)"

re: fclean all

.PHONY: all clean fclean re
