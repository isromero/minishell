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



void execute(t_cmd *cmd) // NO HACER DOBLE FORK.....
{
    int i;
	int	j;
    char *com;
    char **exec_args;
	
	i = 0;
	j = 0;
    com = NULL;
    exec_args = NULL;
    while (i < cmd->n_tokens - 1)
    {
        if (!is_builtin(cmd, i) && !is_argument_extension(cmd, i) && !is_special(cmd->token[i][0])/* Pendiente introducir is_special con todo */) 
        {
            // HACER UN INT QUE DEVUELVA ERROR Y NO ENTRAR
            printf("ESTO ES LA I %d\n", i);
            pid_t pid = fork();
            if (pid == -1)
            {
                perror("fork");
                return;
            }
            else if (pid == 0)
            {
                com = command_dir(cmd, cmd->token[i]);
				/* Si se obtuvo una ruta válida (com != NULL), se crea un nuevo arreglo exec_args para almacenar los argumentos que se pasarán a execve. */
                if (com != NULL)
                {
					/* Se asigna memoria dinámicamente para exec_args con un tamaño igual al número de tokens 
					restantes en cmd desde la posición i, más 1 para el elemento NULL que se agrega al final del arreglo. */
                    exec_args = (char **)malloc(sizeof(char *) * (cmd->n_tokens - i + 1));
                    if (!exec_args)
                        return ;
					j = i; // Así guardamos la distancia ya recorrida
                    while(j < cmd->n_tokens - 1 && !is_special(cmd->token[j][0]))
                    {
                        /* Ejemplo del por qué se = así:
                        En la primera iteración del bucle, j tomará el valor de i, que es 2. Si simplemente usamos j como índice para 
                        exec_args, se copiarían los tokens a partir del índice 2, pero queremos que se copien desde el índice 0 en exec_args.
                        Entonces, para compensar el j = i, restamos i a j, obteniendo j - i que es 0 en este caso. */
                        exec_args[j - i] = cmd->token[j];
                        j++;
                    }
                    exec_args[j - i] = NULL;
                    if(!is_redirect(cmd))
                        execve(com, exec_args, cmd->env);
                    else if(is_redirect(cmd) == 1)
                    {
                        output_redirect(cmd);
                        execve(com, exec_args, cmd->env);
                        close_redirect(cmd);
                    }
                    else if(is_redirect(cmd) > 1)
                    {
                        output_multiple_redirect(cmd);
                        execve(com, exec_args, cmd->env);
                        close_redirect(cmd);
                    }
                    free(exec_args);
                    perror("execve");
                    exit(1);
                }
                if (!com)
                {
                    perror("command not found");
                    exit(1);
                }
                else
                    exit(0);
            }
            else
                wait(NULL);
        }
        if (is_builtin(cmd, i) && !is_argument_extension(cmd, i))
            execute_builtin(cmd, i); // En principio funciona, aunque tal vez necesita pruebas(antes estaba metido todo directo el execute, con su i correspondiente)
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
    else if (ft_strcmp(cmd->token[n_token], "export") == 0 && ft_strchr(cmd->token[n_token + 1], '='))
    {
        ft_export(cmd, n_token);
        n_token++;
    }
    else if (ft_strcmp(cmd->token[n_token], "unset") == 0)
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

void executor(t_cmd *cmd)
{
    int i;
    int j;

    i = 0;
    j = 0;
    cmd->count_pipes = 0;
    cmd->count_pids = 0;
    cmd->fd = (int **)malloc(sizeof(int *) * cmd->n_pipes); // Proteger mallocs + liberaciones?
    cmd->pid = (int *)malloc(sizeof(int) * cmd->n_processes);
    while(j < cmd->n_pipes)
    {
        cmd->fd[j] = (int *)malloc(sizeof(int) * 2);
        // Reservamos para fd[x_numero_de_fd][2]
        j++;
    }
    while(cmd->token[i] != NULL)
    {
        if(cmd->token[i][0] == '|' && cmd->count_pipes == 0)
        {
            init_pipes(cmd);
            // envía salida
            dup2(cmd->fd[cmd->count_pipes][WRITE_END], STDOUT_FILENO);
            close(cmd->fd[cmd->count_pipes][WRITE_END]);
            close(cmd->fd[cmd->count_pipes][READ_END]);
            execute_pipes(cmd);
            cmd->count_pids++;
        }
        else if(cmd->token[i][0] == '|' && cmd->count_pipes > 0 && cmd->count_pipes < cmd->n_pipes - 1)
        {
            // recibe entrada
            dup2(cmd->fd[cmd->count_pipes][READ_END], STDIN_FILENO);
            close(cmd->fd[cmd->count_pipes][WRITE_END]);
            close(cmd->fd[cmd->count_pipes][READ_END]);
            cmd->count_pipes++;
            // envía salida
            dup2(cmd->fd[cmd->count_pipes][WRITE_END], STDOUT_FILENO);
            close(cmd->fd[cmd->count_pipes][WRITE_END]);
            close(cmd->fd[cmd->count_pipes][READ_END]);
            execute_pipes(cmd);
            cmd->count_pids++;
        }
        else if (cmd->count_pipes > 0 && cmd->count_pipes == cmd->n_pipes - 1) 
        {
            // recibe entrada
            dup2(cmd->fd[cmd->count_pipes][READ_END], STDIN_FILENO);
            close(cmd->fd[cmd->count_pipes][WRITE_END]);
            close(cmd->fd[cmd->count_pipes][READ_END]);
            execute_pipes(cmd);
            // cerramos el último pipe que se queda abierto porque solo recibe y no envía??
            //...^
            cmd->count_pids++; // no necesario
        }
        i++;
    }
    wait_close_pipes(cmd);
    execute(cmd);
}

void execute_pipes(t_cmd *cmd)
{
    int i;
    i = 0;
    while(i < cmd->n_tokens - 1)
    {
        if(i == 0)
            execute_first_pipes(cmd, i);
        if(i == find_len_last_command(cmd))
             execute_last_pipes(cmd, i);
        if((i != 0 && i != find_len_last_command(cmd)) && (!is_argument_extension(cmd, i) && !is_pipe(cmd->token[i][0])))
            execute_middle_pipes(cmd, i);
            
        i++;
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
    cmd->pid[cmd->count_pids] = fork(); //Checkear error de for
    if(cmd->pid[cmd->count_pids] == 0)
    {
        if(is_builtin(cmd, i))
            execute_builtin(cmd, i);
        else
        {
            com = command_dir(cmd, cmd->token[i]);
            if (com != NULL)
            {
                exec_args = (char **)malloc(sizeof(char *) * (cmd->n_tokens - i + 1));
                j = i;
                while(j < cmd->n_tokens - 1 && cmd->token[j][0] != '|' && cmd->token[j][0] != '>')
                {
                    exec_args[j - i] = cmd->token[j];
                    j++;
                }
                exec_args[j - i] = NULL;
                close(cmd->fd[0][READ_END]);
                dup2(cmd->fd[0][WRITE_END], STDOUT_FILENO);
                close(cmd->fd[0][WRITE_END]);
                if(is_redirect_pipes(cmd, i) == 1)
                {
                    output_redirect(cmd);
                    execve(com, exec_args, NULL);
                    close_redirect(cmd);
                }
               /*  else if(is_redirect_pipes(cmd, i) >= 1)
                {
                    output_multiple_redirect(cmd);
                    execve(com, exec_args, NULL);
                    close_redirect(cmd);
                } */
                else if(is_redirect_pipes(cmd, i) == 0)
                    execve(com, exec_args, NULL);
                perror("");
                exit(0);
            }
            if (!com && !is_argument_extension(cmd, i)) 
            {
                perror("command not found");
                exit(1);
            }
        }
        cmd->count_pids++;
    }
    close(cmd->fd[0][WRITE_END]);
    free(exec_args);
}

void    execute_middle_pipes(t_cmd *cmd, int i)
{
    char    *com;
    char    **exec_args;
    int j;

    com = NULL;
    exec_args = NULL;
    j = 0;
    cmd->pid[cmd->count_pids] = fork();
    if(cmd->pid[cmd->count_pids] == 0)
    {
        if(is_builtin(cmd, i))
            execute_builtin(cmd, i);
        else
        {
            com = command_dir(cmd, cmd->token[i]);
            if(com != NULL)
            {
                exec_args = (char **)malloc(sizeof(char *) * (cmd->n_tokens - i + 1));
                j = i;
                while(j < cmd->n_tokens - 1 && cmd->token[j][0] != '|' && cmd->token[j][0] != '>')
                {
                    exec_args[j - i] = cmd->token[j];
                    j++;
                }
                exec_args[j - i] = NULL;
                close(cmd->fd[0][WRITE_END]);
                dup2(cmd->fd[0][READ_END], STDIN_FILENO);
                close(cmd->fd[0][READ_END]);
                if(is_redirect_pipes(cmd, i) == 1)
                {
                    output_redirect(cmd);
                    execve(com, exec_args, NULL);
                    close_redirect(cmd);
                }
               /*  else if(is_redirect_pipes(cmd, i) >= 1)
                {
                    output_multiple_redirect(cmd);
                    execve(com, exec_args, NULL);
                    close_redirect(cmd);
                } */
                if(is_redirect_pipes(cmd, i) == 0)
                     execve(com, exec_args, NULL);
                perror("");
                exit(0);
            }
            if (!com && !is_argument_extension(cmd, i)) 
            {
                perror("command not found");
                exit(1);
            }
        }
        cmd->count_pids++;
    }
    close(cmd->fd[0][WRITE_END]);
    close(cmd->fd[0][READ_END]);
    free(exec_args);
}

void    execute_last_pipes(t_cmd *cmd, int i)
{
    char    *com;
    char    **exec_args;
    int j;

    com = NULL;
    j = 0;
    exec_args = NULL;
    cmd->pid[cmd->count_pids] = fork();
    if(cmd->pid[cmd->count_pids] == 0)
    {
        if(is_builtin(cmd, i))
            execute_builtin(cmd, i);
        else
        {
            com = command_dir(cmd, cmd->token[i]);
            if(com != NULL)
            {
                exec_args = (char **)malloc(sizeof(char *) * (cmd->n_tokens - i + 1));
                j = i;
                while(j < cmd->n_tokens - 1 && cmd->token[j][0] != '|' && cmd->token[j][0] != '>')
                {
                    exec_args[j - i] = cmd->token[j];
                    j++;
                }
                exec_args[j - i] = NULL;
                close(cmd->fd[0][WRITE_END]);
                dup2(cmd->fd[0][READ_END], STDIN_FILENO);
                close(cmd->fd[0][READ_END]);
                if(is_redirect_pipes(cmd, i) == 1)
                {
                    output_redirect(cmd);
                    execve(com, exec_args, NULL);
                    close_redirect(cmd);
                }
                /*  else if(is_redirect_pipes(cmd, i) >= 1)
                {
                    output_multiple_redirect(cmd);
                    execve(com, exec_args, NULL);
                    close_redirect(cmd);
                } */
                else if(is_redirect_pipes(cmd, i) == 0)
                    execve(com, exec_args, NULL);
                perror("");
                exit(0);
            }
            if (!com && !is_argument_extension(cmd, i)) 
            {
                perror("command not found");
                exit(1);
            }
        }
    }
    close(cmd->fd[0][READ_END]);
    close(cmd->fd[0][WRITE_END]);
    free(exec_args);
}


char  *command_dir(t_cmd *cmd, char *command) 
{
    char 	*path;
    char 	*dir;
	size_t	dir_len;
	
	path = ft_getenv("PATH", cmd->env);
	dir = ft_strtok(path, ":");
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
			// El archivo ejecutable existe
			ft_strcat(dir, "/");
			ft_strcat(dir, command);
            return (dir);
        }
        dir = ft_strtok(NULL, ":");
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
    if (path != NULL) {
        char *path_copy = ft_strdup(path);
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

// METER UN TRY_ACCESS PARA EJECUTAR COSAS DENTRO DE NUESTRA MINISHELL