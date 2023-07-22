/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 17:45:54 by isromero          #+#    #+#             */
/*   Updated: 2023/05/26 17:45:54 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_pipes(t_cmd *cmd)
{
    int i;

    i = 0;
    while(i < cmd->n_pipes)
    {
        free(cmd->fd[i]);
        i++;
    }
    free(cmd->fd);
    free(cmd->pid);
}

void executor(t_cmd *cmd)
{
    int i;
    int j;

    i = 0;
    j = 0;
    cmd->count_pipes = 0;
    cmd->count_pids = 0;
    cmd->replaced_var = 0;
    cmd->pid = (int *)malloc(sizeof(int) * cmd->n_processes);
    cmd->fd = (int **)malloc(sizeof(int *) * cmd->n_pipes);
    while(j < cmd->n_pipes)
        cmd->fd[j++] = (int *)malloc(sizeof(int) * 2);
    while(cmd->token[i] != NULL)
    {
        if(cmd->token[i][0] == '|')
        {
            if(cmd->token[0][0] == '|') // Si hay un pipe de primer token
            {
                printf("minishell: syntax error near unexpected token `%s'\n", cmd->token[i]);
                free_pipes(cmd);
                return ;
            }
            redirecting_pipes(cmd);
            free_pipes(cmd);
            return ;
        }
        i++;
    }
    free_pipes(cmd);
    execute(cmd);
}

void execute(t_cmd *cmd)
{
    int i;
	int	j;
    int first_variable;

	i = 0;
	j = 0;
    first_variable = 0;
    signal(SIGINT, &handle_ctrlc2);
    while (i < cmd->n_tokens - 1)
    {
        if(is_variable(cmd->token[i][0]) && first_variable == 0)
            first_variable = 1; // Para que solo se ejecute una vez las variables
        if(cmd->no_expand_vars[i] == 0)
            replace_vars(cmd, &cmd->token[i]);
        if (!is_builtin(cmd, i) && !is_argument_extension(cmd, i) && !is_special(cmd->token[i][0]) && !is_redirects(cmd->token[j][0]))
            execute_fork(cmd, i);
        if (is_builtin(cmd, i) && !is_argument_extension(cmd, i) && !is_special(cmd->token[i][0]) && !is_redirects(cmd->token[i][0]))
        {   
            if(!is_output_redirect(cmd, i) && !is_input_redirect(cmd, i) \
                && !is_append_redirect(cmd, i) && !is_heredoc_redirect(cmd, i))
            {
                execute_builtin(cmd, i);
                return ;
            }
            execute_redirects(cmd, NULL, NULL, i);
            g_status = 0; /* Reinicializamos a 0 porque cuando se pone un echo $? necesitamos reestablecer el status después de haberse ejecutado para siguientes iteraciones */
            return ;
        }
        i++;
    }
}

void	execute_builtin(t_cmd *cmd, int n_token)
{
	// Rellenar con los builtins de arriba
    if (ft_strcmp(cmd->token[n_token], "cd") == 0)
    {
        ft_cd(cmd, n_token);
        // Avanzamos ya que el primer argumento es cd pero el segundo la ruta, así que tenemos que saltarla
        n_token++;
    }
    else if (ft_strcmp(cmd->token[n_token], "pwd") == 0)
        ft_pwd(cmd);
    else if (ft_strcmp(cmd->token[n_token], "env") == 0)
        ft_env(cmd);
    else if (ft_strcmp(cmd->token[n_token], "export") == 0 && cmd->token[n_token + 1] != NULL && ft_strchr(cmd->token[n_token + 1], '='))
    {
        ft_export(cmd, n_token);
        n_token++;
    }
    else if (ft_strcmp(cmd->token[n_token], "unset") == 0 && cmd->token[n_token + 1] != NULL)
    {
        ft_unset(cmd, n_token);
        n_token++;
    }
    else if (ft_strcmp(cmd->token[n_token], "echo") == 0)
    // Se iguala ya que echo recorre todo los tokens y devuelve la longitud de i recorrida, no hace falta sumar i++ después
        n_token = ft_echo(cmd, n_token);
    else if (ft_strcmp(cmd->token[n_token], "exit") == 0)
        ft_exit(cmd, n_token);
        // se va a salir del programa, hace falta que le sume i si hay numero? Creo que no... PENDIENTE
    else
        return;
}

#include <stdarg.h>

void print_to_stdout(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    vfprintf(stdout, format, args);
    va_end(args);
}

void redirecting_pipes(t_cmd *cmd) /* dobles comandos como grep y cat se quedan en bucle, error de restauración de stdout stdin?? */
{
    int i;
    int first_time;
    int len;

    i = 0;
    len = 0;
    first_time = 0; 
    init_pipes(cmd);
    signal(SIGINT, &handle_ctrlc2);
    while(i < cmd->n_tokens - 1)
    {
        if(cmd->token[i][0] == '|' && cmd->count_pipes == 0 && first_time == 0)
        {
            first_time = 1;
            execute_first_pipes(cmd, 0);
            cmd->count_pids++;
        }
        else if(cmd->token[i][0] == '|' && (first_time == 1 || cmd->count_pipes > 0) && cmd->count_pipes < cmd->n_pipes/*  - 1 */)
        {
            first_time = 0;
            len = find_len_command_pipes(cmd, i);
            execute_middle_pipes(cmd, len);
            cmd->count_pids++;
        }
        else if (!is_special(cmd->token[i][0]) && cmd->token[i + 1] == NULL && (first_time == 1 || cmd->count_pipes > 0) && cmd->count_pipes == cmd->n_pipes /* - 1 */) 
        {
            first_time = 0;
            len = find_len_command_pipes(cmd, i);
            execute_last_pipes(cmd, len);
        }
        i++;
    }
    wait_close_pipes(cmd);
}

void    execute_last_pipes(t_cmd *cmd, int i)
{
    char    *com;
    char    **exec_args;
    int     j;

    com = NULL;
    exec_args = NULL;
    j = 0;
    if(cmd->no_expand_vars[i] == 0)
        replace_vars(cmd, &cmd->token[i]);
    if (!is_argument_extension(cmd, i) && !is_redirects(cmd->token[i][0]) && !is_redirects_double_char(cmd->token[i])) //Double char redirects no hace falta?
    {
        if(!is_executable(cmd, cmd->token[i][0])) // Soluciona un leak a la hora de pasarle una ruta absoluta que es un directorio
            com = command_dir(cmd, cmd->token[i]);
        cmd->pid[cmd->count_pids] = fork(); //Checkear error de fork
        if(cmd->pid[cmd->count_pids] == 0)
        {
            if(is_builtin(cmd, i))
            {
                close(cmd->fd[cmd->count_pipes - 1][WRITE_END]);
                dup2(cmd->fd[cmd->count_pipes - 1][READ_END], STDIN_FILENO);
                close(cmd->fd[cmd->count_pipes - 1][READ_END]);
                if(!is_output_redirect(cmd, i) && !is_input_redirect(cmd, i) \
                && !is_append_redirect(cmd, i) && !is_heredoc_redirect(cmd, i))
                {
                    execute_builtin(cmd, i);
                    g_status = 0;
                    exit(g_status);
                }
                execute_appends(cmd, com, exec_args, i);
                execute_output_redirects(cmd, com, exec_args, i);
                execute_heredoc_redirects(cmd, com, exec_args, i);
                execute_input_redirects(cmd, com, exec_args, i);
                g_status = 0;
                exit(g_status);
            }
            else if (com != NULL || is_executable(cmd, cmd->token[i][0]))
            {
                exec_args = (char **)malloc(sizeof(char *) * (cmd->n_tokens - i + 1));
                j = i;
                while(j < cmd->n_tokens - 1 && !is_special(cmd->token[j][0]) && !is_redirects(cmd->token[j][0]))
                {
                    exec_args[j - i] = cmd->token[j];
                    j++;
                }
                exec_args[j - i] = NULL;

                // AQUÍ LLEGA MAL EL NÚMERO DE COUNT PIPES SI HAY UN PIPE SOLO, CON 0 FUNCIONA(ls | grep utils)
                print_to_stdout("count_pipes: %d\n", cmd->count_pipes - 1);
                close(cmd->fd[cmd->count_pipes - 1][WRITE_END]);
                dup2(cmd->fd[cmd->count_pipes - 1][READ_END], STDIN_FILENO);
                close(cmd->fd[cmd->count_pipes - 1][READ_END]);
                if(!is_output_redirect(cmd, i) && !is_input_redirect(cmd, i) && \
                    !is_append_redirect(cmd, i) && !is_heredoc_redirect(cmd, i) && !is_executable(cmd, cmd->token[i][0]))
                {
                    if(execve(com, exec_args, cmd->env) == -1)
                    {
                        g_status = 2;
                        exit(g_status);
                    }
                }
                /* los appends y heredocs van antes ya que son 2 carácteres en vez de 1 */
                execute_executable(cmd, cmd->token[i]);
                execute_appends(cmd, com, exec_args, i);
                execute_output_redirects(cmd, com, exec_args, i);
                execute_heredoc_redirects(cmd, com, exec_args, i);
                execute_input_redirects(cmd, com, exec_args, i);
                exit(0);
            }
            else if (!com && !is_executable(cmd, cmd->token[i][0]))
            {
                g_status = 127;
                DIR* dir = opendir(cmd->token[i]);
                if(dir)
                {
                    printf("-minishell: %s: Is a directory\n", cmd->token[i]);
                    closedir(dir);
                }
                else
                        printf(cmd->token[i][0] == '/' ? "-minishell: %s: No such file or directory\n" \
                    : "-minishell: %s: command not found\n", cmd->token[i]);
                exit(g_status);
            }
        }
        else
        {
            free(exec_args); 
            free(com);
        }  
    }
}

void    execute_middle_pipes(t_cmd *cmd, int i)
{
    char    *com;
    char    **exec_args;
    int     j;

    com = NULL;
    exec_args = NULL;
    j = 0;
    if(cmd->no_expand_vars[i] == 0)
        replace_vars(cmd, &cmd->token[i]);
    if (!is_argument_extension(cmd, i) && !is_redirects(cmd->token[i][0]) && !is_redirects_double_char(cmd->token[i]))  //Double char redirects no hace falta?
    {
        if(!is_executable(cmd, cmd->token[i][0])) // Soluciona un leak a la hora de pasarle una ruta absoluta que es un directorio
            com = command_dir(cmd, cmd->token[i]);
        cmd->pid[cmd->count_pids] = fork(); //Checkear error de fork
        if(cmd->pid[cmd->count_pids] == 0)
        {
            if(is_builtin(cmd, i))
            {
                close(cmd->fd[cmd->count_pipes - 1][WRITE_END]);
                dup2(cmd->fd[cmd->count_pipes - 1][READ_END], STDIN_FILENO);
                close(cmd->fd[cmd->count_pipes - 1][READ_END]);
                close(cmd->fd[cmd->count_pipes][READ_END]);
                dup2(cmd->fd[cmd->count_pipes][WRITE_END], STDOUT_FILENO);
                close(cmd->fd[cmd->count_pipes][WRITE_END]);
                cmd->count_pipes++;
                if(!is_output_redirect(cmd, i) && !is_input_redirect(cmd, i) \
                && !is_append_redirect(cmd, i) && !is_heredoc_redirect(cmd, i))
                {
                    execute_builtin(cmd, i);
                    g_status = 0;
                    exit(g_status);
                }
                execute_appends(cmd, com, exec_args, i);
                execute_output_redirects(cmd, com, exec_args, i);
                execute_heredoc_redirects(cmd, com, exec_args, i);
                execute_input_redirects(cmd, com, exec_args, i);
                g_status = 0;
                exit(g_status);
            }
            else if (com != NULL || is_executable(cmd, cmd->token[i][0]))
            {
                exec_args = (char **)malloc(sizeof(char *) * (cmd->n_tokens - i + 1));
                j = i;
                while(j < cmd->n_tokens - 1 && !is_special(cmd->token[j][0]) && !is_redirects(cmd->token[j][0]))
                {
                    exec_args[j - i] = cmd->token[j];
                    j++;
                }
                exec_args[j - i] = NULL;
                print_to_stdout("count_pipes: %d\n", cmd->count_pipes - 1);
                close(cmd->fd[cmd->count_pipes - 1][WRITE_END]);
                dup2(cmd->fd[cmd->count_pipes - 1][READ_END], STDIN_FILENO);
                close(cmd->fd[cmd->count_pipes - 1][READ_END]);
                print_to_stdout("count_pipes: %d\n", cmd->count_pipes);
                close(cmd->fd[cmd->count_pipes][READ_END]);
                dup2(cmd->fd[cmd->count_pipes][WRITE_END], STDOUT_FILENO);
                close(cmd->fd[cmd->count_pipes][WRITE_END]);
                cmd->count_pipes++;
                if(!is_output_redirect(cmd, i) && !is_input_redirect(cmd, i) && \
                    !is_append_redirect(cmd, i) && !is_heredoc_redirect(cmd, i) && !is_executable(cmd, cmd->token[i][0]))
                {
                    if(execve(com, exec_args, cmd->env) == -1)
                    {
                        g_status = 2;
                        exit(g_status);
                    }
                }
                /* los appends y heredocs van antes ya que son 2 carácteres en vez de 1 */
                execute_executable(cmd, cmd->token[i]);
                execute_appends(cmd, com, exec_args, i);
                execute_output_redirects(cmd, com, exec_args, i);
                execute_heredoc_redirects(cmd, com, exec_args, i);
                execute_input_redirects(cmd, com, exec_args, i);
                exit(0);
            }
            else if (!com && !is_executable(cmd, cmd->token[i][0]))
            {
                g_status = 127;
                DIR* dir = opendir(cmd->token[i]);
                if(dir)
                {
                    printf("-minishell: %s: Is a directory\n", cmd->token[i]);
                    closedir(dir);
                }
                else
                        printf(cmd->token[i][0] == '/' ? "-minishell: %s: No such file or directory\n" \
                    : "-minishell: %s: command not found\n", cmd->token[i]);
                exit(g_status);
            }    
        }
        else
        {
            free(exec_args);
            free(com);
            cmd->count_pipes++;
        }
    }
}

void    execute_first_pipes(t_cmd *cmd, int i)
{
    char    *com;
    char    **exec_args;
    int     j;

    com = NULL;
    exec_args = NULL;
    j = 0;
    if(cmd->no_expand_vars[i] == 0)
        replace_vars(cmd, &cmd->token[i]);
    if (!is_argument_extension(cmd, i) && !is_redirects(cmd->token[i][0]) && !is_redirects_double_char(cmd->token[i]))  //Double char redirects no hace falta?
    {
        if(!is_executable(cmd, cmd->token[i][0])) // Soluciona un leak a la hora de pasarle una ruta absoluta que es un directorio
            com = command_dir(cmd, cmd->token[i]);
        cmd->pid[cmd->count_pids] = fork(); //Checkear error de fork
        if(cmd->pid[cmd->count_pids] == 0)
        {
            if(is_builtin(cmd, i))
            {
                close(cmd->fd[cmd->count_pipes][READ_END]);
                dup2(cmd->fd[cmd->count_pipes][WRITE_END], STDOUT_FILENO);
                close(cmd->fd[cmd->count_pipes][WRITE_END]);
                cmd->count_pipes++;
                if(!is_output_redirect(cmd, i) && !is_input_redirect(cmd, i) \
                && !is_append_redirect(cmd, i) && !is_heredoc_redirect(cmd, i))
                {
                    execute_builtin(cmd, i);
                    g_status = 0;
                    exit(g_status);
                }
                execute_appends(cmd, com, exec_args, i);
                execute_output_redirects(cmd, com, exec_args, i);
                execute_heredoc_redirects(cmd, com, exec_args, i);
                execute_input_redirects(cmd, com, exec_args, i);
                g_status = 0;
                exit(g_status);
            }
            else if (com != NULL || is_executable(cmd, cmd->token[i][0]))
            {
                exec_args = (char **)malloc(sizeof(char *) * (cmd->n_tokens - i + 1));
                j = i;
                while(j < cmd->n_tokens - 1 && !is_special(cmd->token[j][0]) && !is_redirects(cmd->token[j][0]))
                {
                    exec_args[j - i] = cmd->token[j];
                    j++;
                }
                exec_args[j - i] = NULL;
                print_to_stdout("count_pipes: %d\n", cmd->count_pipes);
                close(cmd->fd[cmd->count_pipes][READ_END]);
                dup2(cmd->fd[cmd->count_pipes][WRITE_END], STDOUT_FILENO);
                close(cmd->fd[cmd->count_pipes][WRITE_END]);
                cmd->count_pipes++;
                if(!is_output_redirect(cmd, i) && !is_input_redirect(cmd, i) && \
                    !is_append_redirect(cmd, i) && !is_heredoc_redirect(cmd, i) && !is_executable(cmd, cmd->token[i][0]))
                {
                    if(execve(com, exec_args, cmd->env) == -1)
                    {
                        g_status = 2;
                        exit(g_status);
                    }
                }
                /* los appends y heredocs van antes ya que son 2 carácteres en vez de 1 */
                execute_executable(cmd, cmd->token[i]);
                execute_appends(cmd, com, exec_args, i);
                execute_output_redirects(cmd, com, exec_args, i);
                execute_heredoc_redirects(cmd, com, exec_args, i);
                execute_input_redirects(cmd, com, exec_args, i);
                exit(0);
            }
            else if (!com && !is_executable(cmd, cmd->token[i][0]))
            {
                g_status = 127;
                DIR* dir = opendir(cmd->token[i]);
                if(dir)
                {
                    printf("-minishell: %s: Is a directory\n", cmd->token[i]);
                    closedir(dir);
                }
                else
                        printf(cmd->token[i][0] == '/' ? "-minishell: %s: No such file or directory\n" \
                    : "-minishell: %s: command not found\n", cmd->token[i]);
                exit(g_status);
            }
        }
        else
        {
            free(exec_args);
            free(com);
            cmd->count_pipes++;
        }
    }
}

char  *command_dir(t_cmd *cmd, char *command) 
{
    char 	*path;
    char 	*dir;
	size_t	dir_len;

    path = ft_getenv("PATH", cmd->env);
    dir = ft_strtok(path, ":");
    if(!is_executable(cmd, command[0]))
    {
        while (dir != NULL) 
        {
            // Construye la ruta completa al archivo ejecutable
            char executable_path[PATH_MAX];
            dir_len = ft_strlen(dir);

            ft_strcpy(executable_path, dir);
            executable_path[dir_len] = '/';
            ft_strcpy(executable_path + dir_len + 1, command);
            
            // Verifica si el archivo ejecutable existe y es ejecutable
            if (access(executable_path, F_OK) == 0)
            {
                char *result = (char *)malloc((dir_len + ft_strlen(command) + 2) * sizeof(char)); // +2 para '/' y '\0'
                if (result == NULL)
                {
                    free(path);
                    return NULL;
                }
                ft_strcpy(result, dir);
                ft_strcat(result, "/");
                ft_strcat(result, command);
                free(path);
                return (result);
            }
            dir = ft_strtok(NULL, ":");
        }
        // Para lo demás significa que no existe
        free(path);
        // g_status = 126; ??? Es cuando el archivo no existe? pero esto está bien?
        return (NULL);
    }
    // El archivo ejecutable no existe en ningún directorio del PATH
    return (0);
}


/* Es una función muy parecida a command dir pero no devuelve con access ni nada, solo construye el
comando pasado */
char *build_command_path(const char *base_path, const char *command)
{
    size_t base_len = ft_strlen(base_path);
    size_t command_len = ft_strlen(command);
    size_t total_len = base_len + command_len + 2; // +2 para '/' y '\0'

    char *command_path = (char *)malloc(total_len * sizeof(char));
    ft_strncpy(command_path, base_path, base_len);
    command_path[base_len] = '/';
    ft_strncpy(command_path + base_len + 1, command, command_len);
    command_path[total_len - 1] = '\0';

    return (command_path);
}


int is_command_exists(t_cmd *cmd, char *command)
{
    if (access(command, F_OK) == 0) {
        return 1; 
        /*  El comando existe en la ruta actual
        (no necesario, pero útil por si proporcionamos una ruta completa en vez de solo el comando
        aunque actualmente eso lo hayamos solucionado de otra manera, en un futuro podemos mantener esto) */
    }

    char *path = ft_getenv("PATH", cmd->env);
    if (path != NULL)
    {
        char *path_copy = ft_strdup(path);
        free(path);
        char *token = ft_strtok(path_copy, ":");
        while (token != NULL) {
            char *command_path = build_command_path(token, command);
            if (access(command_path, F_OK) == 0) {
                free(path_copy);
                free(command_path);
                return 1; // El comando existe en una de las rutas de PATH
            }
            free(command_path);
            token = ft_strtok(NULL, ":");
        }
        free(path_copy);
    }
   /*  if (is_builtin(cmd, command))
        return 1; */

    return 0; // El comando no existe en ninguna ubicación conocida
}

/* minishell -> ls -l | <
minishell: syntax error near unexpected token `newline' */

/* checkear env -i ./minishell */