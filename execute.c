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

void executor(t_cmd *cmd)
{
    int i;

    i = 0;
    cmd->replaced_var = 0;
    while(cmd->token[i] != NULL)
    {
        if(cmd->token[i][0] == '|')
        {
            if(cmd->token[0][0] == '|')
                printf("minishell: syntax error near unexpected token `%s'\n", cmd->token[i]);
            else
                redirecting_pipes(cmd);
        }
        i++;
    }
    execute(cmd);
}

void execute(t_cmd *cmd)
{
    int i;
    int return_code;

	i = 0;
    return_code = 0;
    signal(SIGINT, &handle_ctrlc2);
    while (i < cmd->n_tokens - 1)
    {
        if(cmd->no_expand_vars[i] == 0)
            replace_vars(cmd, &cmd->token[i]);
        if (!is_argument_extension(cmd, i) && cmd->replaced_var <= 1 && !is_special(cmd->token[i][0]) && !is_redirects(cmd->token[i][0]) && !is_redirects_double_char(cmd->token[i]))
        {
            if(!is_builtin(cmd, i))
                return_code = execute_fork(cmd, i);
            else
                return_code = execute_builtin_no_child(cmd, i);
            if(return_code == 1)
                return ;
        }
        i++;
    }
}

void	execute_builtin(t_cmd *cmd, int n_token)
{
    if (ft_strcmp(cmd->token[n_token], "cd") == 0)
    {
        ft_cd(cmd, n_token);
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
        n_token = ft_echo(cmd, n_token);
    else if (ft_strcmp(cmd->token[n_token], "exit") == 0)
        ft_exit(cmd, n_token);
    else
        return;
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
