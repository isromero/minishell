# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/13 09:42:22 by isromero          #+#    #+#              #
#    Updated: 2023/05/13 09:42:22 by isromero         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	minishell
CC			=	gcc
SRCS		=	builtins/builtins.c builtins/builtins2.c builtins/builtins_utils.c environment/env.c executor/normal/execute.c expander/expander.c \
				minishell.c parser/parser.c prompt/prompt.c parser/token_utils.c executor/pipes/pipes_utils.c \
				utils.c redirects/output_redirects.c redirects/input_redirects.c signals.c executor/try_access.c \
				get_next_line/get_next_line.c get_next_line/get_next_line_utils.c redirects/execute_redirects.c \
				executor/normal/execute_utils.c executor/normal/fork_processes.c executor/pipes/redirecting_pipes.c \
				is_check.c is_check2.c is_check3.c executor/pipes/fork_pipes_processes.c parser/quote_manage.c \
				parser/quote_manage2.c executor/pipes/execute_pipes_utils.c executor/pipes/execute_pipes.c

LIBFT		=	libft/libft.a

CFLAGS		=	-Wall -Werror -Wextra -lreadline -g -fsanitize=address,undefined
RM			=	rm -f
#PONER LAS FLAGS NECESARIAS PARA LOS MACS DE 42

all:		$(NAME)

OBJ_DIR		=	obj
OBJS		=	$(addprefix $(OBJ_DIR)/,$(SRCS:.c=.o))

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
			@make -C libft >/dev/null 2>&1
			@make clean -C libft >/dev/null 2>&1

$(NAME):	$(OBJS) $(LIBFT)
			$(CC) $(OBJS) $(LIBFT) $(CFLAGS) -o $(NAME) >/dev/null 2>&1

clean:
	@$(RM) $(OBJS) >/dev/null 2>&1
	@$(RM) -r $(OBJ_DIR) >/dev/null 2>&1

fclean: clean
	@$(RM) $(NAME) >/dev/null 2>&1
	@make fclean -C libft/ >/dev/null 2>&1

re: fclean all

.PHONY: all clean fclean re
