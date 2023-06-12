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
#include <limits.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
 #include <fcntl.h>
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "libft/libft.h"

#define READ_END 0
#define WRITE_END 1

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
	int		n_pipes;
	int		n_processes;
	int		count_pipes;
	int		count_pids;
	int		pipes_stdout; // Posible uso para guardar el estado de antes de empezar redireccionamientos con pipes
	int		**fd;
	int		*pid;
	char	**exec_args;
	int 	stdout;
} t_cmd;

#define HEREDOC_REDIRECT "<<"
#define HEREDOC_QUOTE '<<Q'
#define INPUT_REDIRECT '<'
#define INPUT_REDIRECT '<'
#define PIPE '|'
#define OUTPUT_REDIRECT '>'
#define APPEND_REDIRECT ">>"
#define ARGUMENT '-'
#define DOUBLE_QUOTE '"'
#define ARGUMENT '-'
#define SINGLE_QUOTE '\''
#define VARIABLE '$'

/* builtins.c */
int		is_builtin(t_cmd *cmd, int n_token);
int 	ft_echo(t_cmd *cmd, int echo_token);
void	ft_cd(t_cmd *cmd, int cd_token);
int		ft_env(t_cmd *cmd);
int		ft_pwd(t_cmd *cmd);
void 	ft_export(t_cmd *cmd, int export_token);
bool 	compareVariableName(const char* variable, const char* name);
void 	ft_unset(t_cmd *cmd, int unset_token);
void 	execute_builtin_exit(t_cmd *cmd, int exit_code);
void	ft_exit(t_cmd *cmd, int exit_token);

/* env.c */
char	*ft_getenv(const char *name, char **env);
// int 	ft_setenv(const char *name, const char *value, int overwrite);

/* execute.c */
void 	executor(t_cmd *cmd);
void	execute(t_cmd *cmd);
void	execute_builtin(t_cmd *cmd, int n_token);
void 	redirecting_pipes(t_cmd *cmd);
void    execute_pipes(t_cmd *cmd, int i);
void    execute_last_pipes(t_cmd *cmd, int len, int stdout);
void    execute_middle_pipes(t_cmd **cmd, int i);
void    execute_first_pipes(t_cmd *cmd, int i);
char	*command_dir(t_cmd *cmd, char *command);
int 	is_command_exists(t_cmd *cmd, char *command);
char 	*build_command_path(const char *base_path, const char *command);


/* expander.c */
void	print_vars(t_cmd *cmd);

/* parser.c */
void	parse_args(t_cmd *cmd);

/* pipes_utils.c */
void    init_pipes(t_cmd *cmd);
void    wait_close_pipes(t_cmd *cmd);
int 	find_len_command_pipes(t_cmd *cmd, int i);
void 	count_pipes(t_cmd *cmd);

/* prompt.c */
char	*get_prompt(t_cmd *custom_prompt);
void	print_minishell();

/* token_utils.c */
void 	clean_tokens(t_cmd *cmd);
void 	print_tokens(t_cmd *cmd);
void 	save_token(t_cmd *cmd, char *token);
int		is_special(char c);
int		is_argument(char c);
int		is_argument_extension(t_cmd *cmd, int i);
int		is_pipe(char c);
int		is_special2(char c);
int		is_variable(t_cmd *cmd, int len);
int 	is_double_quote(t_cmd *cmd, int len);
int 	is_single_quote(t_cmd *cmd, int len);
void 	error_special(); // Meter perror mejor?
int 	check_len_special(t_cmd *cmd, int len);
int 	is_token(t_cmd *cmd, int len);
int		check_len_token(t_cmd *cmd, int len);
int		find_variables(char **token);

/* utils.c */
char	*ft_strtok(char *str, const char *delim);
int 	is_number(const char *str);
#endif

/* redirects.c */
void	output_redirect(t_cmd *cmd);
void 	close_redirect(t_cmd *cmd);

int 	is_output_redirect(t_cmd *cmd);
void	output_multiple_redirect(t_cmd *cmd);
int 	is_redirect_pipes(t_cmd *cmd, int i);
void	append_redirect(t_cmd *cmd);
void	append_multiple_redirect(t_cmd *cmd);
int 	is_append_redirect(t_cmd *cmd);
int		find_first_output_redirect(t_cmd *cmd);
int		find_last_output_redirect(t_cmd *cmd);
int 	find_first_append_redirect(t_cmd *cmd);
int 	find_last_append_redirect(t_cmd *cmd);