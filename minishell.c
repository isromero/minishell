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
    
    size_t prompt_length = ft_strlen(username) + ft_strlen(cwd) + 13 + ft_strlen(COLOR_GREEN) \
	+ ft_strlen(COLOR_MAGENTA) + ft_strlen(COLOR_YELLOW) + ft_strlen(COLOR_CYAN) + ft_strlen(COLOR_WHITE); // Longitud total del prompt personalizado
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
    static char *save;
    char *token;
    int i;
    int j;

    if (str != NULL)
        save = str;
    if (save == NULL || *save == '\0')
        return NULL;
    // Salta los delimitadores iniciales
    i = 0;
    while (save[i] && strchr(delim, save[i]) != NULL)
        i++;
    // Encuentra el inicio del token
    j = i;
    while (save[j] && strchr(delim, save[j]) == NULL)
        j++;
    // Asigna memoria para el token y copia los caracteres
    token = (char *)malloc(sizeof(char) * (j - i + 1));
    if (token == NULL)
        return NULL;
    memcpy(token, save + i, sizeof(char) * (j - i));
    token[j - i] = '\0';
    // Actualiza el puntero save para el siguiente llamado
    save += j;
    if (save[j] == '\0')
        save = NULL;
    else
        save++;
    return token;
}

char  *is_command_exists(t_cmd *cmd, char *command) 
{
    char 	*path;
    char 	*dir;
	size_t	dir_len;
	size_t	command_len;
	char	*aux;
	
	path = ft_getenv("PATH", cmd->env);
	dir = ft_strtok(path, ":");
	aux = NULL;
    while (dir != NULL) 
	{
        // Construye la ruta completa al archivo ejecutable
        char executable_path[PATH_MAX];
        dir_len = ft_strlen(dir);
        command_len = ft_strlen(command);

		strcpy(executable_path, dir);
        executable_path[dir_len] = '/';
        strcpy(executable_path + dir_len + 1, command);
		
        // Verifica si el archivo ejecutable existe y es ejecutable
        if (access(executable_path, X_OK) == 0) 
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

void execute(t_cmd *cmd) {
    pid_t pid;

    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
		//printf("%s\n", cmd->token[0]);
        execve(is_command_exists(cmd, cmd->token[0]), cmd->token, NULL);
        perror("execve");  // En caso de error en execve
        exit(1);
    } else {
        wait(NULL);
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
		free(cmd.line);
		free(cmd.prompt);
	}
	rl_clear_history();
    return (0);
}