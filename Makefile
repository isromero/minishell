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
SRCS		=	builtins.c env.c execute.c expander.c minishell.c parser.c \
				prompt.c token_utils.c pipes_utils.c utils.c output_redirects.c \
				input_redirects.c signals.c try_access.c get_next_line/get_next_line.c \
				get_next_line/get_next_line_utils.c execute_redirects.c execute_utils.c \
				fork_processes.c redirecting_pipes.c is_check.c is_check2.c is_check3.c \
				fork_pipes_processes.c quote_manage.c quote_manage2.c execute_pipes_utils.c \
				execute_pipes.c

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
