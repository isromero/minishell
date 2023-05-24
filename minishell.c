/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 17:20:04 by isromero          #+#    #+#             */
/*   Updated: 2023/05/18 17:20:04 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *ft_getenv(const char *name, char **env) 
{
    size_t name_len = ft_strlen(name);

    // Recorrer el arreglo hasta encontrar el puntero nulo
    for (int i = 0; env[i] != NULL; i++)
	{
        // Comparar el nombre de la variable con la clave en cada elemento del arreglo
        if (ft_strncmp(name, env[i], name_len) == 0 && env[i][name_len] == '=') 
		{
            // Retornar el puntero al valor de la variable de entorno (después del '=')
            return &env[i][name_len + 1];
        }
    }
    // La variable de entorno no fue encontrada
    return NULL;
}

int	is_special(char c)
{
	return(c == INPUT_REDIRECT || c == PIPE || c == OUTPUT_REDIRECT \
	|| c == VARIABLE);
}

int	is_special2(char c)
{
	return(c == INPUT_REDIRECT || c == PIPE || c == OUTPUT_REDIRECT);
}

int	is_variable(t_cmd *cmd, int len)
{
	int	i;

	i = 0;
	while(cmd->line[i + len] != ' ' && cmd->line[i + len] != '\0' && !is_special2(cmd->line[i + len]))
		i++;
	return (i);
}

// ARREGLAR COMILLAS AL FINAL SOLO, NO DETECTA
int is_double_quote(t_cmd *cmd, int len)
{
	int i;

	i = 1;
	if(!ft_strchr(&cmd->line[1], DOUBLE_QUOTE))
			return (-1);
	while(cmd->line[i + len] != '\0' && cmd->line[i + len] != DOUBLE_QUOTE)
		i++;
	i++;
	return (i);
}

int is_single_quote(t_cmd *cmd, int len)
{
	int	i;

	i = 1;
	if(!ft_strchr(&cmd->line[1], SINGLE_QUOTE))
		return (-1);
	while(cmd->line[i + len] != '\0' && cmd->line[i + len] != SINGLE_QUOTE)
		i++;
	i++;
	return (i);
}

void error_special()
{
	printf("syntax error near unexpected token\n");
}

int check_len_special(t_cmd *cmd, int len)
{
	int i;

	i = 0;
	while(cmd->line[i + len] != ' ' && is_special(cmd->line[i + len]))
		i++;
	// Gestión de errores
	printf("len especial:%d\n", i);
	if(i > 2)
		error_special();
	else if(i == 1)
		return(1);
	else if (i == 2)
		return(2);
	return (-1);
}

int is_token(t_cmd *cmd, int len)
{
    int i = 0;
    while (cmd->line[i + len] != '\0' && cmd->line[i + len] != ' ' && !is_special2(cmd->line[i + len]))
        i++;
    return i;
}

int	check_len_token(t_cmd *cmd, int len)
{
	int i = 0;
	while(cmd->line[len] != '\0')
	{
		while(cmd->line[i + len] != '\0' && cmd->line[i + len] != ' ')
			i++;
		if(cmd->line[len] == VARIABLE)
			return(is_variable(cmd, len));
		else if(cmd->line[len] == SINGLE_QUOTE)
			return(is_single_quote(cmd, len));
		else if(cmd->line[len] == DOUBLE_QUOTE)
			return(is_double_quote(cmd, len));
		else if(is_special2(cmd->line[len] )) // < > | 
			return(check_len_special(cmd, len)); // << < > >> |
		else if(cmd->line[len] != '\0')
			return(is_token(cmd, len));
	}
	return i;
}

void save_token(t_cmd *cmd, char *token)
{
    size_t total_size = (cmd->n_tokens + 1) * sizeof(char *);
    char **new_token = (char **)malloc(total_size);
    if (new_token == NULL)
    {
        printf("Error: No se pudo asignar memoria para cmd->token\n");
        return;
    }
    ft_memcpy(new_token, cmd->token, cmd->n_tokens * sizeof(char *));
    new_token[cmd->n_tokens] = token;
    free(cmd->token);
    cmd->token = new_token;
    cmd->n_tokens++;
}

void clean_tokens(t_cmd *cmd)
{
    for (int i = 0; i < cmd->n_tokens; i++)
    {
        free(cmd->token[i]);
    }
    free(cmd->token);
    cmd->token = NULL;
    cmd->n_tokens = 0;
}

void print_tokens(t_cmd *cmd)
{
    int i;
    for (i = 0; i < cmd->n_tokens; i++)
		printf("Token %d: %s\n", i, cmd->token[i]);
}

void parse_args(t_cmd *cmd)
{
    int len;
    int i = 0;
    cmd->n_tokens = 0;
    cmd->token = NULL;

    while (cmd->line[i] != '\0')
    {
        while (cmd->line[i] == ' ')
            i++;
        len = check_len_token(cmd, i);
        if (len > 0)
        {
            char *token = (char *)malloc((len + 1) * sizeof(char));
            if (token == NULL)
            {
				perror("Error: No se pudo asignar memoria para el token");
				clean_tokens(cmd);
                return;
            }
            strncpy(token, cmd->line + i, len);
            token[len] = '\0';
            save_token(cmd, token);
            i += len;
        }
        else
        {
			perror("Error: introducir token válido");
        	break;
        }
    }
    print_tokens(cmd);
}

char *get_prompt(t_cmd *custom_prompt)
{
    char *username = getenv("USER"); // Obtener el nombre de usuario desde la variable de entorno USER
    char cwd[1024];
    getcwd(cwd, sizeof(cwd)); // Obtener la ubicación actual de la terminal
    
    size_t prompt_length = ft_strlen(username) + ft_strlen(cwd) + ft_strlen(COLOR_GREEN) \
	+ ft_strlen(COLOR_MAGENTA) + ft_strlen(COLOR_YELLOW) + ft_strlen(COLOR_CYAN) + ft_strlen(COLOR_WHITE) + 16; // Longitud total del prompt personalizado
    custom_prompt->prompt = (char *)malloc(prompt_length * sizeof(char));

	strcpy(custom_prompt->prompt, COLOR_GREEN);
    strcat(custom_prompt->prompt, username);
    strcat(custom_prompt->prompt, COLOR_MAGENTA);
    strcat(custom_prompt->prompt, "@minishell");
	strcat(custom_prompt->prompt, COLOR_YELLOW);
	strcat(custom_prompt->prompt, ":");
    strcat(custom_prompt->prompt, COLOR_CYAN);
    strcat(custom_prompt->prompt, cwd);
    strcat(custom_prompt->prompt, COLOR_WHITE);
    strcat(custom_prompt->prompt, "$ ");

    return (custom_prompt->prompt);
}

void	print_minishell()
{
	printf("\n▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄");
	printf("\n▐   ▄▄       ▄▄  ▄▄▄▄▄▄▄▄▄▄▄  ▄▄        ▄  ▄▄▄▄▄▄▄▄▄▄▄  ▄▄▄▄▄▄▄▄▄▄▄  ▄         ▄  ▄▄▄▄▄▄▄▄▄▄▄  ▄            ▄           ▐");
	printf("\n▐  ▐░░▌     ▐░░▌▐░░░░░░░░░░░▌▐░░▌      ▐░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░▌       ▐░▌▐░░░░░░░░░░░▌▐░▌          ▐░▌          ▐");
	printf("\n▐  ▐░▌░▌   ▐░▐░▌ ▀▀▀▀█░█▀▀▀▀ ▐░▌░▌     ▐░▌ ▀▀▀▀█░█▀▀▀▀ ▐░█▀▀▀▀▀▀▀▀▀ ▐░▌       ▐░▌▐░█▀▀▀▀▀▀▀▀▀ ▐░▌          ▐░▌          ▐");
	printf("\n▐  ▐░▌▐░▌ ▐░▌▐░▌     ▐░▌     ▐░▌▐░▌    ▐░▌     ▐░▌     ▐░▌          ▐░▌       ▐░▌▐░▌          ▐░▌          ▐░▌          ▐");
	printf("\n▐  ▐░▌ ▐░▐░▌ ▐░▌     ▐░▌     ▐░▌ ▐░▌   ▐░▌     ▐░▌     ▐░█▄▄▄▄▄▄▄▄▄ ▐░█▄▄▄▄▄▄▄█░▌▐░█▄▄▄▄▄▄▄▄▄ ▐░▌          ▐░▌          ▐");
	printf("\n▐  ▐░▌  ▐░▌  ▐░▌     ▐░▌     ▐░▌  ▐░▌  ▐░▌     ▐░▌     ▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░▌          ▐░▌          ▐");
	printf("\n▐  ▐░▌   ▀   ▐░▌     ▐░▌     ▐░▌   ▐░▌ ▐░▌     ▐░▌      ▀▀▀▀▀▀▀▀▀█░▌▐░█▀▀▀▀▀▀▀█░▌▐░█▀▀▀▀▀▀▀▀▀ ▐░▌          ▐░▌          ▐");
	printf("\n▐  ▐░▌       ▐░▌     ▐░▌     ▐░▌    ▐░▌▐░▌     ▐░▌               ▐░▌▐░▌       ▐░▌▐░▌          ▐░▌          ▐░▌          ▐");
	printf("\n▐  ▐░▌       ▐░▌ ▄▄▄▄█░█▄▄▄▄ ▐░▌     ▐░▐░▌ ▄▄▄▄█░█▄▄▄▄  ▄▄▄▄▄▄▄▄▄█░▌▐░▌       ▐░▌▐░█▄▄▄▄▄▄▄▄▄ ▐░█▄▄▄▄▄▄▄▄▄ ▐░█▄▄▄▄▄▄▄▄▄ ▐");
	printf("\n▐  ▐░▌       ▐░▌▐░░░░░░░░░░░▌▐░▌      ▐░░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░▌       ▐░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐");
	printf("\n▐   ▀         ▀  ▀▀▀▀▀▀▀▀▀▀▀  ▀        ▀▀  ▀▀▀▀▀▀▀▀▀▀▀  ▀▀▀▀▀▀▀▀▀▀▀  ▀         ▀  ▀▀▀▀▀▀▀▀▀▀▀  ▀▀▀▀▀▀▀▀▀▀▀  ▀▀▀▀▀▀▀▀▀▀▀ ▐");
	printf("\n▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄\n");
	printf("\n");
}

char *ft_strtok(char *str, const char *delim)
{
    static char *save = NULL;

    if (str != NULL)
        save = str;
    if (save == NULL || *save == '\0')
        return NULL;
    // Encuentra el inicio del token
    while (*save && strchr(delim, *save) != NULL)
        save++;
    if (*save == '\0')
        return NULL;
    char *token = save;
    // Encuentra el final del token
    while (*save && strchr(delim, *save) == NULL)
        save++;
    if (*save != '\0')
        *save++ = '\0';
    return token;
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


int	find_variables(char **token)
{
	int i;

	i = 0;
	while(token[i][0] != '\0')
	{
		if(token[i][0] == VARIABLE)
			return(1);
		i++;
	}
	return (0);
}

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

void print_vars(t_cmd *cmd)
{
	int	i;
	char *path;
	char *aux;
	char *var;

	i = 0;
	aux = NULL;
	while(cmd->token[i])
	{
		if(cmd->token[i][0] == VARIABLE)
		{
			aux = cmd->token[i];
			var = &aux[1];

			// Quedarse solo con la lógica(está mal seguro)
			path = ft_getenv(var, cmd->env);
			if(path == NULL)
				return ;
			printf("minishell: %s\n", path);
		}
		i++;
	}
}

// int ft_setenv(const char *name, const char *value, int overwrite) 
// {
//     size_t name_len = strlen(name);
//     size_t value_len = strlen(value);
// 	int	i;

// 	i = 0;
//     while (cmd->env[i] != NULL) 
// 	{
//         if (strncmp(cmd->env[i], name, name_len) == 0 && cmd->env[i][name_len] == '=') 
// 		{
//             if (overwrite == 0)
//                 return 0;
//             char *new_env = malloc(name_len + value_len + 2);
//             if (new_env == NULL)
//                 return -1;

//             strcpy(new_env, name);
//             strcat(new_env, "=");
//             strcat(new_env, value);

//             cmd->env[i] = new_env;
//             return 0;
//         }
// 		i++;
//     }
//     size_t env_len = 0;
//     while (cmd->env[env_len] != NULL)
//         env_len++;
//     char **new_environ = realloc(environ, (env_len + 2) * sizeof(char *));
//     if (new_environ == NULL)
//         return -1;
//     char *new_env = malloc(name_len + value_len + 2);
//     if (new_env == NULL)
//         return -1;
//     strcpy(new_env, name);
//     strcat(new_env, "=");
//     strcat(new_env, value);
//     new_environ[env_len] = new_env;
//     new_environ[env_len + 1] = NULL;
//     environ = new_environ;
//     return 0;
// }

int	is_builtin(t_cmd *cmd, int n_token)
{
	if (ft_strcmp(cmd->token[n_token], "echo") == 0)
		return(1);
	else if (ft_strcmp(cmd->token[n_token], "cd") == 0)
		return(1);
	else if (ft_strcmp(cmd->token[n_token], "pwd") == 0)
		return(1);
	else if (ft_strcmp(cmd->token[n_token], "export") == 0)
		return(1);
	else if (ft_strcmp(cmd->token[n_token], "unset") == 0)
		return(1);
	else if (ft_strcmp(cmd->token[n_token], "env") == 0)
		return(1);
	else if (ft_strcmp(cmd->token[n_token], "exit") == 0)
		return(1);
	return (0);
}

void	cd_builtin(t_cmd *cmd, int cd_token)
{
	char	*path;
	char	cwd[1024];
	char	*oldpwd;

	path = cmd->token[cd_token + 1];
	oldpwd = ft_getenv("PWD", cmd->env);

	//chdir falla al cambiar al directorio especificado, no devuelve 0 
	if (chdir(path) != 0)
		perror("");
	getcwd(cwd, sizeof(cwd));
	setenv(oldpwd, cwd, 1);
}

int ft_env(t_cmd *cmd)
{
    int status = 0;
    int i;
    int size;

    i = 0;
    status = 0;
    if (!cmd->env || !cmd->env[0])
        return -1;

    if (cmd->n_tokens > 1)
    {
		printf("%d\n", cmd->n_tokens);
        printf("minishell: env: Demasiados argumentos\n");
        return 127;
    }
    while (cmd->env[i] != NULL)
    {
        size = ft_strlen(cmd->env[i]);
        if (strcmp(cmd->env[i] + size - 2, "''") != 0)
        {
            printf("%s\n", cmd->env[i]);
        }
        i++;
    }
	//comprobacion de si hay pipes despues
	
    return status;
}

void execute(t_cmd *cmd)
{
    int i;

	i = 0;
	if (cmd->n_tokens > 0 && strcmp(cmd->token[i], "env") == 0)
    {
        // int status = 
		ft_env(cmd);
        // Manejar el status del comando env según sea necesario
        return;
    }
	
   	while (i < cmd->n_tokens)
    {
		if(is_builtin(cmd, i) == 0)
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
				if (com != NULL)
				{
					printf("iscommand: %s\n", com);
					execve(com, cmd->token, cmd->env);
					perror("execve");  // En caso de error en execve
					exit(1);
				}
				if (!com)
				{
					perror("command not found");
					exit(1);
				}  
			}
			else
				wait(NULL);
		}
		if(is_builtin(cmd, i) == 1)
		{
			if (ft_strcmp(cmd->token[i], "cd") == 0)
			{
				cd_builtin(cmd, i);
				// Avanzamos ya que el primer argumento es cd pero el segundo la ruta, así que tenemos que saltarla
				i++;
			}
		}
		i++;
    }
}


int	main(int argc, char **argv, char **env)
{
	t_cmd	cmd;
	(void)argc;
	(void)argv;
	//print_minishell();
	cmd.env = env;
	while(1)
	{
		cmd.prompt = get_prompt(&cmd);
		cmd.line = readline(cmd.prompt);
		if(ft_strncmp(cmd.line, "", 1) > 0) 
			add_history(cmd.line);
		parse_args(&cmd);
		execute(&cmd);
		clean_tokens(&cmd);
		free(cmd.line);
		free(cmd.prompt);
	}
	rl_clear_history();
    return (0);
}