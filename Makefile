# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: leaugust <leaugust@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/22 14:08:51 by leaugust          #+#    #+#              #
#    Updated: 2025/04/23 16:09:47 by leaugust         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	minishell
CC			=	cc -g3
CFLAGS		=	-Wall -Wextra -Werror -I/opt/homebrew/opt/readline/include
LIBFTDIR 	=	libft
LDFLAGS 	=	-lreadline -L$(LIBFTDIR) -lft -L/opt/homebrew/opt/readline/lib

SRC			=	srcs/parsing/tokenizer/tokenizer.c \
				srcs/parsing/tokenizer/tokenizer_buffer.c \
				srcs/parsing/tokenizer/tokenizer_chars.c \
				srcs/parsing/tokenizer/tokenizer_chars_utils.c \
				srcs/parsing/tokenizer/tokenizer_process.c \
				srcs/parsing/tokenizer/tokenizer_utils.c \
				srcs/parsing/tokenizer/token_utils.c \
				srcs/parsing/tokenizer/token_operators.c \
				srcs/parsing/tokenizer/token_operators_utils.c \
				srcs/parsing/tokenizer/token_quotes.c \
				srcs/parsing/tokenizer/token_quotes_utils.c \
				srcs/parsing/tokenizer/token_quotes_dquote.c \
				srcs/parsing/tokenizer/token_command.c \
				srcs/parsing/tokenizer/token_command_utils.c \
				srcs/parsing/tokenizer/double_quoted.c \
				srcs/parsing/tokenizer/double_quoted_len.c \
				srcs/parsing/tokenizer/double_quoted_utils.c \
				srcs/parsing/tokenizer/double_quoted_copy.c \
				srcs/parsing/tokenizer/token_is_redir.c \
				srcs/parsing/tokenizer/token_is_redir_utils.c \
				srcs/parsing/tokenizer/token_expand.c \
				srcs/parsing/tokenizer/token_expand_utils.c \
				srcs/parsing/tokenizer/token_expand_dquote.c \
				srcs/parsing/tokenizer/token_expand_dquote2.c \
				srcs/parsing/tokenizer/quotes_utils.c \
				srcs/parsing/parser/parsing_utils.c \
				srcs/parsing/parser/parsing.c \
				srcs/parsing/parser/parsing_validation.c \
				srcs/parsing/parser/parsing_redirection.c \
				srcs/parsing/parser/heredoc.c \
				srcs/parsing/parser/expand.c \
				srcs/parsing/parser/expand_utils.c \
				srcs/parsing/parser/expand_var.c \
				srcs/parsing/parser/expand_null_utils.c \
				srcs/parsing/parser/heredoc_input.c \
				srcs/parsing/parser/heredoc_utils.c \
				srcs/parsing/executor/exec_argv.c \
				srcs/parsing/executor/exec_argv_utils.c \
				srcs/parsing/executor/exec_cmd.c \
				srcs/parsing/executor/exec_cmd_utils.c \
				srcs/parsing/executor/exec_builtin_helper.c \
				srcs/parsing/executor/exec_heredoc.c \
				srcs/parsing/executor/exec_utils.c \
				srcs/parsing/executor/exec_redirect.c \
				srcs/parsing/executor/exec_redirect_utils.c \
				srcs/parsing/executor/exec_builtin.c \
				srcs/parsing/executor/exec_path.c \
				srcs/parsing/executor/exec_pipe.c \
				srcs/parsing/executor/exec_pipe_utils.c \
				srcs/parsing/executor/exit_status.c \
				srcs/parsing/executor/handle_error.c \
				srcs/builtins/ft_cd.c \
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
				srcs/builtins/export/ft_export_sort.c \
				srcs/parsing/signals/signals.c \
				srcs/main.c \
				srcs/main_utils.c \
				srcs/shell_init.c

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