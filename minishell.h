/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 17:50:52 by isromero          #+#    #+#             */
/*   Updated: 2023/05/18 17:50:52 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "libft/libft.h"
#include <stdbool.h>

#define STYLE_BOLD "\033[1m"
#define STYLE_UNDERLINE "\033[4m"
#define STYLE_BLINK "\033[5m"

// Colores de texto
#define COLOR_BLACK "\033[30m"
#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN "\033[36m"
#define COLOR_WHITE "\033[37m"

// Colores de fondo
#define BACKGROUND_BLACK "\033[40m"
#define BACKGROUND_RED "\033[41m"
#define BACKGROUND_GREEN "\033[42m"
#define BACKGROUND_YELLOW "\033[43m"
#define BACKGROUND_BLUE "\033[44m"
#define BACKGROUND_MAGENTA "\033[45m"
#define BACKGROUND_CYAN "\033[46m"
#define BACKGROUND_WHITE "\033[47m"

typedef struct cmd
{
	char	*line;
	char	**token;
	int		n_tokens;
	char 	*prompt;
	char	**env;
} t_cmd;

typedef enum {
	INPUT_REDIRECT = '<',
	HEREDOC_REDIRECT = '<' << 8,
	HEREDOC_QUOTE = ('<' << 8) + 'Q',
	PIPE = '|',
	OUTPUT_REDIRECT = '>',
	APPEND_REDIRECT = ('>' << 8) + '>',
	COMMAND = 'C',
	ARGUMENT = '-',
	DOUBLE_QUOTE = '"',
	SINGLE_QUOTE = '\'',
	BUILTIN = ('b' << 16) + ('u' << 8) + 'i',
	VARIABLE = '$'
} token_type;

#endif