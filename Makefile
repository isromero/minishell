# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/29 20:01:05 by isromero          #+#    #+#              #
#    Updated: 2024/03/29 20:01:05 by isromero         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	minishell
CC			=	gcc
SRCS		=	builtins/builtins.c builtins/builtins2.c builtins/builtins3.c builtins/builtins_utils.c builtins/builtins_utils2.c environment/env.c executor/normal/execute.c expander/expander.c \
				minishell.c parser/parser.c prompt/prompt.c parser/token_utils.c executor/pipes/pipes_utils.c \
				utils.c redirects/output_redirects.c redirects/output_redirects_utils.c redirects/append_redirects.c redirects/input_redirects.c signals.c signals2.c executor/try_access.c \
				get_next_line/get_next_line.c get_next_line/get_next_line_utils.c redirects/execute_redirects.c \
				executor/normal/execute_utils.c executor/normal/fork_processes.c executor/pipes/redirecting_pipes.c \
				checks/is_check.c checks/is_check2.c checks/is_check3.c executor/pipes/fork_pipes_processes.c \
				parser/quote_manage.c executor/pipes/execute_pipes.c redirects/heredoc_redirects_utils.c \
				redirects/heredoc_redirects.c checks/is_check4.c redirects/input_redirects_utils.c redirects/append_redirects_utils.c \
				expander/expander_heredoc.c expander/expander_utils.c executor/normal/get_cmd.c

LIBFT		=	libft/libft.a

CFLAGS		=	-Wall -Werror -Wextra #-g -fsanitize=address,undefined
LDFLAGS		=	-lreadline
INCFLAGS	=	-I /usr/local/include
RM			=	rm -f

all:		$(NAME)

OBJ_DIR		=	obj
OBJS		=	$(addprefix $(OBJ_DIR)/,$(SRCS:.c=.o))

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCFLAGS) -c $< -o $@

$(LIBFT):
			@make -C libft >/dev/null 2>&1
			@make clean -C libft >/dev/null 2>&1

$(NAME):	$(OBJS) $(LIBFT)
			$(CC) $(OBJS) $(LIBFT) $(CFLAGS) $(LDFLAGS) -o $(NAME) >/dev/null 2>&1

clean:
	@$(RM) $(OBJS) >/dev/null 2>&1
	@$(RM) -r $(OBJ_DIR) >/dev/null 2>&1

fclean: clean
	@$(RM) $(NAME) >/dev/null 2>&1
	@make fclean -C libft/ >/dev/null 2>&1

re: fclean all

.PHONY: all clean fclean re
