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

void execute(t_cmd *cmd)
{
    int i;
	int	j;
	
	i = 0;
	j = 0;
    while (i < cmd->n_tokens)
    {
        if (is_builtin(cmd, i) == 0)
        {
            pid_t pid = fork();
            if (pid == -1)
            {
                perror("fork");
                return;
            }
            else if (pid == 0)
            {
                char *com = command_dir(cmd, cmd->token[i]);
				/* Si se obtuvo una ruta válida (com != NULL), se crea un nuevo arreglo exec_args para almacenar los argumentos que se pasarán a execve. */
                if (com != NULL)
                {
                    printf("iscommand: %s\n", com); 
					/* Se asigna memoria dinámicamente para exec_args con un tamaño igual al número de tokens 
					restantes en cmd desde la posición i, más 1 para el elemento NULL que se agrega al final del arreglo. */
                    char **exec_args = (char **)malloc(sizeof(char *) * (cmd->n_tokens - i + 1));
                    if (!exec_args)
                        return ;
					j = i; // Así guardamos la distancia ya recorrida
                   	while(j < cmd->n_tokens)
					{
						/* Ejemplo del por qué se = así:
						En la primera iteración del bucle, j tomará el valor de i, que es 2. Si simplemente usamos j como índice para 
						exec_args, se copiarían los tokens a partir del índice 2, pero queremos que se copien desde el índice 0 en exec_args.
						Entonces, para compensar el j = i, restamos i a j, obteniendo j - i que es 0 en este caso. */
						exec_args[j - i] = cmd->token[j];
						j++;
					}
                    exec_args[cmd->n_tokens - i] = NULL; // Agrega el NULL al final del arreglo, es igual que exec_args[j - i]
                    execve(com, exec_args, cmd->env);
                    perror("execve");
                    exit(0);
                }
                if (!com && !is_argument(cmd->token[i][0])) // No solo vale con checkear si es argumento '-', sino también textos acompañados de echos, cat..(mirar más
															//comandos que puedan tener textos, y sino buscar una solución global para esto)
                {
                    perror("command not found"); // Tal vez quitar saltos de línea en errores, checkear eso
                    exit(1);
                }
                else
                    exit(0);
            }
            else
                wait(NULL);
        }
        if (is_builtin(cmd, i) == 1)
        {
            if (ft_strcmp(cmd->token[i], "cd") == 0)
            {
                ft_cd(cmd, i);
                // Avanzamos ya que el primer argumento es cd pero el segundo la ruta, así que tenemos que saltarla
                i++;
            }
            else if (ft_strcmp(cmd->token[i], "pwd") == 0)
                ft_pwd(cmd);
            else if (ft_strcmp(cmd->token[i], "env") == 0)
                ft_env(cmd);
            else if (ft_strcmp(cmd->token[i], "export") == 0 && ft_strchr(cmd->token[i + 1], '='))
            {
                ft_export(cmd, i);
                i++;
            }
            else if (ft_strcmp(cmd->token[i], "unset") == 0)
            {
                ft_unset(cmd, i);
                i++;
            }
            else if (ft_strcmp(cmd->token[i], "echo") == 0)
            // Se iguala ya que echo recorre todo los tokens y devuelve la longitud de i recorrida, no hace falta sumar i++ después
                i = ft_echo(cmd, i);
            else if (ft_strcmp(cmd->token[i], "exit") == 0)
                ft_exit(cmd, i);
                // se va a salir del programa, hace falta que le sume i si hay numero? Creo que no... PENDIENTE
            else
                return;
        }
        i++;
    }
}

void execute_pipes(t_cmd *cmd) /* TRATAR DE PRIMERO PROBAR CON POCOS FD Y LUEGO TRASLADARLO A NUESTRO CÓDIGO */
{
    int fd[cmd->n_pipes][2]; // Arreglo de descriptores de archivo para las pipes
    int status; // Estado de los hijos
    int pid; // Identificador de proceso
    int i = 0;

    // Crear las pipes necesarias
    while (i < cmd->n_pipes)
    {
        if (pipe(fd[i]) == -1)
        {
            perror("pipe");
            exit(1);
        }
        i++;
    }
  
    // Crear los procesos hijos
    i = 0;
    int j;
    j = 0;
    while (i < cmd->n_pipes)
    {
        char *com = command_dir(cmd, cmd->token[i]);
        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            exit(1);
        }
        else if (pid == 0)
        {
            close(fd[i][0]);
            dup2(fd[i][1], STDOUT_FILENO);
            close(fd[i][1]);
            char **exec_args = (char **)malloc(sizeof(char *) * (cmd->n_tokens - i + 1));
                    if (!exec_args)
                        return ;
					j = i; // Así guardamos la distancia ya recorrida
                   	while(j < cmd->n_tokens)
					{
                        if (cmd->token[j][0] == '|')
                            j++;
						exec_args[j - i] = cmd->token[j];
						j++;
					}
                    exec_args[cmd->n_tokens - i] = NULL; // Agrega el NULL al final del arreglo, es igual que exec_args[j - i]
                    execve(com, exec_args, cmd->env);
                    perror("execve");
            exit(1);
        }
        else
        {
            close(fd[i][1]);
            dup2(fd[i][0], STDIN_FILENO);
            close(fd[i][0]);
            char **exec_args = (char **)malloc(sizeof(char *) * (cmd->n_tokens - i + 1));
                    if (!exec_args)
                        return ;
					j = i; // Así guardamos la distancia ya recorrida
                   	while(j < cmd->n_tokens)
					{
                        if (cmd->token[j][0] == '|')
                            j++;
						exec_args[j - i] = cmd->token[j];
						j++;
					}
                    exec_args[cmd->n_tokens - i] = NULL; // Agrega el NULL al final del arreglo, es igual que exec_args[j - i]
                    execve(com, exec_args, cmd->env);
                exit(1);
                  
        }
        wait(&status);
        i++;
    }
}




// Haciendo la lógica de los pipes:
void    pipes()
{
    int fd1[2]; // Esto solo crea para 1 pipe y luego para otro
    int fd2[2]; // así que habrá que crear en un futuro tantos como sean necesarios  -->>>>>>>>>>>> // Creo que hay que hacer un algoritmo que sepa cuántos pipes hay
                                                                                                    // el primer elemento que su siguiente es pipe y el último que su
                                                                                                    // anterior es pipe habrá que gestionarlos de x manera, los demás todos igual
                                                                                                    // (hablo de procesos)
    int status; // Estado de los childs
    int pid; // Identifica el proceso de los childs

    pipe(fd1);

    pid = fork();
    if(pid == 0) /* child 1 */
    {
        close(fd1[READ_END]); /* cerrar extremo que no vamos a utilizar ya que el primer comando solo hace write */

        dup2(fd1[WRITE_END], STDOUT_FILENO); /* esto hace que la salida estándar también escriba en el pipe 
        STDOUT se duplica de fd1[1(write_end)] y como no lo necesitamos ya, lo cerramos:*/
        close(fd1[WRITE_END]);
        // execve(com, exec_args, cmd->env) // POSIBLE SOLUCION: pasarlo como argumentos al programa e incorporarlo directamente en el execute(posible idea)
    }
    else /* parent */
    {
        close(fd1[WRITE_END]); /* extremo que ya no es necesario */

        pipe(fd2);
        pid = fork();

        if(pid == 0) /* child 2 */
        {
            close(fd2[READ_END]);

            dup2(fd1[READ_END], STDIN_FILENO);
            close(fd1[READ_END]);

            dup2(fd2[WRITE_END], STDOUT_FILENO);
            close(fd2[WRITE_END]);

        }
        else /* parent */
        {
            close(fd1[READ_END]);
            close(fd2[WRITE_END]);

            pid = fork();

            if(pid == 0) /* child 3 */
            {
                dup2(fd2[READ_END], STDIN_FILENO);
                close(fd2[READ_END]);
            }
        }
    }
    close(fd2[READ_END]);

    wait(&status);
    wait(&status);
    wait(&status);
}

/* void	execute_builtin(t_cmd *cmd, int n_token)
{
	// Rellenar con los builtins de arriba
} */

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

		strcpy(executable_path, dir);
        executable_path[dir_len] = '/';
        strcpy(executable_path + dir_len + 1, command);
		
        // Verifica si el archivo ejecutable existe y es ejecutable
        if (access(executable_path, F_OK) == 0) 
		{
			// El archivo ejecutable existe
			strcat(dir, "/");
			strcat(dir, command);
            return (dir);
        }
        dir = ft_strtok(NULL, ":");
    }
    // El archivo ejecutable no existe en ningún directorio del PATH
    return (0);
}

int is_command_exists(t_cmd *cmd, char *command) 
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

		strcpy(executable_path, dir);
        executable_path[dir_len] = '/';
        strcpy(executable_path + dir_len + 1, command);
		
        // Verifica si el archivo ejecutable existe y es ejecutable
        if (access(executable_path, F_OK) == 0)
            return (1);
        dir = ft_strtok(NULL, ":");
    }
    // El archivo ejecutable no existe en ningún directorio del PATH
    return (0);
}

///En principio esto ya no nos va a servir, pero por si acaso:
////////////////////////////////////////////////////////////////////////////////////////////////

// Esto servirá para el executor, donde queremos decirle que checkee si hay una variable, y si la variable que se le pasa está en env, la ejecutamos
// Ejecutar = printear el valor de la variable (lo sustituido en token)
/* char	*resizeString(const char* str, size_t new_size, int i) 
{
    char	*new_str;

	// Quedarse solo con la lógica(está mal seguro)
	new_str = malloc(new_size);  // Asigna nueva memoria
    strcpy(new_str, str);  // Copia el contenido del string original
	free(str[i]); 
    return (new_str);
}

char **sustitute_token(t_cmd *cmd, int selected)
{
	int	i;
	int j;
	char *path;

	i = 0;
	j = 0;
	while(cmd->token[i][0] != '\0')
	{
		if(cmd->token[i][0] == VARIABLE)
		{
			// Quedarse solo con la lógica(está mal seguro)
			path = getenv(cmd->token[i]);
			path[g] = path[g + 1];
			cmd->token[i] = resizeString(cmd->token[i], ft_strlen(path));
			cmd->token[i] = path;
		}
		i++;
	}
}
 */
////////////////////////////////////////////////////////////////////////////////////////////////