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

# include <stdlib.h>
# include <limits.h>
# include <unistd.h>
# include <string.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdbool.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <signal.h>
# include <dirent.h>
# include "libft/libft.h"
# include "get_next_line/get_next_line.h"

# define READ_END 0
# define WRITE_END 1

# define STYLE_BOLD "\033[1m"
# define STYLE_UNDERLINE "\033[4m"
# define STYLE_BLINK "\033[5m"

# define COLOR_BLACK "\033[30m"
# define COLOR_RED "\033[31m"
# define COLOR_GREEN "\033[32m"
# define COLOR_YELLOW "\033[33m"
# define COLOR_BLUE "\033[34m"
# define COLOR_MAGENTA "\033[35m"
# define COLOR_CYAN "\033[36m"
# define COLOR_WHITE "\033[37m"

# define BACKGROUND_BLACK "\033[40m"
# define BACKGROUND_RED "\033[41m"
# define BACKGROUND_GREEN "\033[42m"
# define BACKGROUND_YELLOW "\033[43m"
# define BACKGROUND_BLUE "\033[44m"
# define BACKGROUND_MAGENTA "\033[45m"
# define BACKGROUND_CYAN "\033[46m"
# define BACKGROUND_WHITE "\033[47m"

# define HEREDOC_REDIRECT "<<"
# define INPUT_REDIRECT '<'
# define PIPE '|'
# define OUTPUT_REDIRECT '>'
# define APPEND_REDIRECT ">>"
# define ARGUMENT '-'
# define DOUBLE_QUOTE '"'
# define ARGUMENT '-'
# define SINGLE_QUOTE '\''
# define VARIABLE '$'

typedef struct cmd
{
	char	*line;
	char	**token;
	int		n_tokens;
	char	*prompt;
	char	**env;
	int		n_pipes;
	int		n_processes;
	int		count_pipes;
	int		count_pids;
	int		pipes_stdout;
	int		**fd;
	int		*pid;
	char	**exec_args;
	int		stdout;
	int		stdin;
	char	**save_lines;
	bool	in_single_quote;
	bool	in_double_quote;
	int		*has_single_quote;
	int		*no_expand_vars;
	int		replaced_var;
	int		in_quote_delim_heredoc;
}	t_cmd;

typedef struct replace_vars
{
	size_t	token_len;
	char	*replaced_token;
	size_t	replaced_len;
	size_t	j;
	char	*value;
	size_t	var_start;
	size_t	var_len;
	char	*var;
}	t_replace_vars;

typedef struct replace_vars_heredoc
{
	char	*path;
	char	*var;
	int		var_length;
	int		j;
	size_t	replace_length;
	char	*replacement;
	char	*start;
	char	*end;
}	t_replace_vars_heredoc;

/* 
	BUILTINS 
*/
/* builtins/builtins_utils.c */
void	execute_builtin(t_cmd *cmd, int n_token);
int		len_var_in_env(t_cmd *cmd, char *token);
int		var_exists(t_cmd *cmd, char *token);
bool	compare_variable(const char *variable, const char *name);
char	**malloc_new_env_builtin(t_cmd *cmd, int len_of_env);

/* builtins/builtins.c */
int		print_echo(t_cmd *cmd, int echo_token, int first_iteration);
int		ft_echo(t_cmd *cmd, int echo_token);
void	set_pwd_env(t_cmd *cmd, char **new_env, char *cwd, char *oldpwd);
void	ft_cd(t_cmd *cmd, int cd_token);
void	ft_pwd(t_cmd *cmd);

/* builtins/builtins2.c */
void	ft_export(t_cmd *cmd, int export_token);
void	ft_unset(t_cmd *cmd, int unset_token);
void	ft_env(t_cmd *cmd);
void	ft_exit(t_cmd *cmd, int exit_token);
void	execute_builtin_exit(t_cmd *cmd, int exit_code);
void	ft_export2(t_cmd *cmd);

/* 
	CHECKS 
*/
/* checks/is_check1.c */
int		is_special(char c);
int		is_variable(char c);
int		is_redirects(char c);
int		is_redirects_double_char(char *token);
int		is_argument(char c);

/* checks/is_check2.c */
int		is_executable(t_cmd *cmd, char c);
int		is_argument_extension(t_cmd *cmd, int i);
int		is_pipe(char c);

int		is_builtin(t_cmd *cmd, int n_token);

/* checks/is_check3.c */
int		is_single_quote(t_cmd *cmd, int len);
int		is_double_quote(t_cmd *cmd, int len);
int		check_len_special(t_cmd *cmd, int len);
int		is_heredoc_redirect(t_cmd *cmd, int len);
int		is_append_redirect(t_cmd *cmd, int len);

/* checks/is_check4.c */
int		is_input_redirect(t_cmd *cmd, int len);
int		is_output_redirect(t_cmd *cmd, int len);

/* 
	ENVIRONMENT 
*/
/* environment/env.c */
char	*ft_getenv(const char *name, char **env);
void	malloc_env(t_cmd *cmd, char **env);
void	init_env(t_cmd *cmd, char **env);
void	get_default_env(t_cmd *cmd);

/* 
	EXECUTOR 
*/
/* executor/normal/execute_utils.c */
char	**get_exec_args(t_cmd *cmd, int i);
void	execute_command_exists(t_cmd *cmd, char *com, char **exec_args, int i);
void	execute_command_no_exists(t_cmd *cmd, int i);
int		execute_builtin_no_child(t_cmd	*cmd, int i);
void	execute_builtin_in_child(t_cmd *cmd, int i);

/* executor/normal/execute.c */
void	executor(t_cmd *cmd);
void	execute(t_cmd *cmd);

/* executor/normal/fork_processes.c */
int		parent_process(char *com, char **exec_args);
void	child_process(t_cmd *cmd, char *com, char **exec_args, int i);
int		execute_fork(t_cmd *cmd, int i);

/* executor/normal/get_cmd.c */
char	*command_dir_exists(char *dir, char *path, char *command);
char	*command_dir(t_cmd *cmd, char *command);
char	*build_command_path(const char *base_path, const char *command);
int		path_is_command_exists(char *path_copy, char *command);
int		is_command_exists(t_cmd *cmd, char *command);

/* executor/pipes/execute_pipes_utils.c */
void	redirecting_pipes(t_cmd *cmd);
void	execute_last_pipes(t_cmd *cmd, int i);
void	execute_middle_pipes(t_cmd *cmd, int i);
void	execute_first_pipes(t_cmd *cmd, int i);

/* executor/pipes/execute_execute_pipes.c */
void	child_pipes_process(t_cmd *cmd, char *com, char **args, int i);
void	execute_fork_pipes(t_cmd *cmd, int i, int redirection_pipe);

/* executor/pipes/pipes_utils.c */
void	init_pipes(t_cmd *cmd);
void	wait_close_pipes(t_cmd *cmd);
void	count_pipes(t_cmd *cmd);
int		find_len_command_pipes(t_cmd *cmd, int i);
void	free_pipes(t_cmd *cmd);

/* executor/pipes/redirecting_pipes.c */
void	redirect_first_pipe(t_cmd *cmd);
void	redirect_middle_pipes(t_cmd *cmd);
void	redirect_last_pipe(t_cmd *cmd);
void	which_pipe_redirect(t_cmd *cmd, int redirection_pipe);

/* executor/try_access.c */
void	execute_executable(t_cmd *cmd, char *command);
void	try_execute(t_cmd *cmd, char *path, char *command);

/* 
	EXPANDER 
*/
/* expander/expander_heredoc.c */
void	init_expand_vars(t_cmd *cmd);
void	init_replace_vars_heredoc(t_replace_vars_heredoc **replace_vars);
void	replace_vars_heredoc(t_cmd *cmd, char *buffer, int i);
void	path_var_exists(t_replace_vars_heredoc *replace, char *buffer, int i);
void	search_var_replace(t_cmd *cmd, char *buffer);

/* expander/expander_utils.c */
void	init_replace_vars(t_replace_vars **replace_vars);
size_t	get_variable_length(const char *token, size_t var_start);
char	*get_variable(const char *token, size_t var_start, size_t var_len);
char	*append_value(t_replace_vars *replace, char **token);
void	get_replaced_token(t_cmd *cmd, char **token, t_replace_vars *replace);

/* expander/expander.c */
void	process_variables(t_cmd *cmd, char **token, t_replace_vars *replace);
void	process_token(t_cmd *cmd, char **token, t_replace_vars *replace_vars);
void	replace_vars(t_cmd *cmd, char **token);
void	replace_before_execute(t_cmd *cmd);

/* 
	PARSER 
*/
/* parser/parser.c */
void	parser_aux_save_token(t_cmd *cmd, int i, int len);
int		parser_save_token(t_cmd *cmd);
int		parse_args(t_cmd *cmd);
void	save_token(t_cmd *cmd, char *token);

/* parser/quote_manage_utils.c */
int		count_quotes(char *line);
int		count_double_quotes(char *token);
int		count_single_quotes(char *token);
int		count_left_single_quotes(char *token);
int		count_left_double_quotes(char *token);

/* parser/quote_manage.c */
void	remove_double_quotes(char **token);
void	remove_single_quotes(char **token);
void	remove_quotes2(t_cmd *cmd, int i);
void	even_quotes(char *token, int left_double_quotes,
			int left_single_quotes, size_t len_token);
int		remove_quotes(t_cmd *cmd);

/* parser/token_utils.c */
void	clean_tokens(t_cmd *cmd);
void	print_tokens(t_cmd *cmd);
int		cmd_token_len(t_cmd *cmd, int len);
int		check_len_token(t_cmd *cmd, int len);
int		find_variables(char **token);

/* 
	PROMPT
*/
/* prompt/prompt.c */
char	*get_prompt(t_cmd *custom_prompt);
char	*update_output(int fd);
char	*exec_custom(t_cmd *cmd, char *full, char *args);
void	print_minishell(void);

/* 
	REDIRECTS 
*/
/* redirects/append_redirects_utils.c */
int		find_first_append_redirect(t_cmd *cmd);
int		find_last_append_redirect(t_cmd *cmd);

/* redirects/append_redirects.c */
void	append_multiple_redirect(t_cmd *cmd);
void	append_redirect(t_cmd *cmd);

/* redirects/execute_redirects.c */
void	execute_appends(t_cmd *cmd, char *com, char **exec_args, int i);
void	execute_output_redirects(t_cmd *cmd, char *com,
			char **exec_args, int i);
void	execute_input_redirects(t_cmd *cmd, char *com, char **exec_args, int i);
void	execute_heredoc_redirects(t_cmd *cmd, char *com, char **args, int i);
void	execute_redirects(t_cmd *cmd, char *com, char **exec_args, int i);

/* redirects/heredoc_redirects_utils.c */
char	*find_heredoc_delim(t_cmd *cmd);

/* redirects/heredoc_redirects.c */
int		heredoc_content(t_cmd *cmd, int fd);
void	heredoc_write_replaced(t_cmd *cmd, int fd, int new_fd);
void	heredoc_child_process(t_cmd *cmd, int fd);
void	heredoc_redirect(t_cmd *cmd);

/* redirects/input_redirects_utils.c */
int		find_first_input_redirect(t_cmd *cmd);
int		find_last_input_redirect(t_cmd *cmd);
void	close_input_redirect(t_cmd *cmd);

/* redirects/input_redirects.c */
void	input_redirect(t_cmd *cmd);
void	input_multiple_redirect(t_cmd *cmd);

/* redirects/output_redirects_utils.c */
int		find_first_output_redirect(t_cmd *cmd);
int		find_last_output_redirect(t_cmd *cmd);
void	close_output_redirect(t_cmd *cmd);

/* redirects/output_redirects.c */
void	output_redirect(t_cmd *cmd);
void	output_multiple_redirect(t_cmd *cmd);

/* minishell.c */
void	init_minishell(t_cmd *cmd, int sig);

/* signals.c */
void	handle_ctrld_heredoc(int sig);
void	handle_ctrlc_heredoc(int sig);
void	handle_ctrlc(int sig);
void	handle_ctrlc2(int sig);
void	handle_ctrld(int sig);

/* utils.c */
char	*ft_strtok(char *str, const char *delim);
char	**ft_strstrdup(char **strstr);
int		is_number(const char *str);
void	free_matrix(char **matrix);

extern int	g_status;

#endif