/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_redirects.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 19:37:01 by adgutier          #+#    #+#             */
/*   Updated: 2023/06/15 18:01:454 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//redirigimos al input
// si es comando que no puede recivir el input lo ejecutamos a pelo.
//La redirección < hola.txt intenta redirigir la entrada estándar del comando ls desde el archivo "hola.txt". 
//Sin embargo, como el comando ls no está diseñado para leer datos de entrada estándar, no se producirá ningún cambio en el comportamiento del comando.

int     is_input_redirect(t_cmd *cmd)
{
	int len;
	int n_redirects;
	

	len = 0;
	n_redirects = 0;
	while(cmd->token[len] != NULL)
	{
		if(cmd->token[len][0] == INPUT_REDIRECT)
			n_redirects++;
		len++;
	}
	if(n_redirects == 1)
		return (1);
	else if(n_redirects > 1)
		return (n_redirects);
	return(0);
}

int     find_first_input_redirect(t_cmd *cmd)
{
	int len;

	len = 0;
	while(cmd->token[len] != NULL)
	{
		if(cmd->token[len][0] == INPUT_REDIRECT)
			return (len);
		len++;
	}
	return(0);
}

int find_last_input_redirect(t_cmd *cmd)
{
	int len;

	len = cmd->n_tokens - 2;
	while(len >= 0)
	{
		if(cmd->token[len][0] == INPUT_REDIRECT) /* Meter más tipos de redirects */
			return (len);
		len--;
	}
	return(0);
}

void    input_redirect(t_cmd *cmd)
{
	int	fd;
	int	len;

	len = find_first_input_redirect(cmd);
	fd = open(cmd->token[len + 1], O_RDONLY | S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        perror("");
        return ;
    }
    cmd->stdin = dup(STDIN_FILENO);
    dup2(fd, STDIN_FILENO); 
    close(fd);
}

void	input_multiple_redirect(t_cmd *cmd)
{
	int	fd;
	int	len;
	int	i;

	i = 0;
	len = find_last_input_redirect(cmd);
	fd = open(cmd->token[len + 1], O_RDONLY | S_IRUSR | S_IWUSR);
	if (fd == -1)
    {
        perror("");
        return ;
    }
	while(i < len)
	{
		if(cmd->token[i][0] == INPUT_REDIRECT)
			open(cmd->token[i + 1], O_RDONLY | S_IRUSR | S_IWUSR);
		i++;
	}
    cmd->stdin = dup(STDIN_FILENO);
    dup2(fd, STDIN_FILENO); 
    close(fd);
}

int     is_heredoc_redirect(t_cmd *cmd)
{
	int len;
	int n_redirects;
	

	len = 0;
	n_redirects = 0;
	while(cmd->token[len] != NULL)
	{
		if(ft_strcmp(cmd->token[len], HEREDOC_REDIRECT) == 0)
			n_redirects++;
		len++;
	}
	if(n_redirects == 1)
		return (1);
	else if(n_redirects > 1)
		return (n_redirects);
	return(0);
}


int     find_first_heredoc_redirect(t_cmd *cmd)
{
	int len;

	len = 0;
	while(cmd->token[len] != NULL)
	{
		if(ft_strcmp(cmd->token[len], HEREDOC_REDIRECT) == 0)
			return (len);
		len++;
	}
	return(0);
}

int find_last_heredoc_redirect(t_cmd *cmd)
{
	int len;

	len = cmd->n_tokens - 2;
	while(len >= 0)
	{
		if(ft_strcmp(cmd->token[len], HEREDOC_REDIRECT) == 0) /* Meter más tipos de redirects */
			return (len);
		len--;
	}
	return(0);
}

char *find_heredoc_delim(t_cmd *cmd)
{
	int len;

	len = cmd->n_tokens - 2;
	while(len >= 0)
	{
		if(ft_strcmp(cmd->token[len], HEREDOC_REDIRECT) == 0) /* Meter más tipos de redirects */
			return (cmd->token[len + 1]); /* Cuidado tal vez con ACCESO A NULLS */
		len--;
	}
	return(0);
}

int	heredoc_content(t_cmd *cmd, int fd) 
{
	char *line;

	/* Gestionar variables, cadenas literales(QUOTES) */
	
	line = readline(">");
	if(ft_strcmp(line, find_heredoc_delim(cmd)) == 0)
	{
		free(line);
		return(1);
	}
	else if(line && *line != '\n')
	{
		ft_putstr_fd(line, fd);
		ft_putchar_fd('\n', fd);
		free(line);
	}
	else if(*line == '\n') /* ? */
		ft_putchar_fd('\n', fd);
	return (0);
}

char* replace_vars_heredoc(t_cmd *cmd, const char *buffer, int i)
{
    char *path;
    char *var;
    int var_length = 0;
    int j;

    var = NULL;
    i++;
    j = i;
    while (buffer[j] != '\0' && buffer[j] != ' ' && buffer[j] != '\n' &&
           buffer[j] != '$' && buffer[j] != '\t')
    {
        var_length++;
        j++;
    }
    var = malloc(var_length + 1);
    strncpy(var, &buffer[i], var_length);
    var[var_length] = '\0';
    path = ft_getenv(var, cmd->env);
    free(var);

    if (path != NULL)
    {
        // Crear una nueva cadena con la parte seleccionada reemplazada por el valor
        char *result = malloc(strlen(buffer) + strlen(path) + 1);
        strncpy(result, buffer, i - 1);
        strcpy(result + i - 1, path);
        strcat(result, buffer + i + var_length);
        free(path);
        return result;
    }
    else
    {
        // Si no se encuentra el valor, devolver una copia del buffer original
        return strdup(buffer);
    }
}

void replace_env_vars(t_cmd *cmd, char *buffer) 
{
	int	i;
	char *result;

	i = 0;
	result = NULL;
	while(buffer[i] != '\0' && buffer[i] != '\n' && buffer[i] != '\t')
	{
		if(buffer[i] == '$')
			buffer = replace_vars_heredoc(cmd, buffer, i);
		i++;
	}
}

void    heredoc_redirect(t_cmd **cmd)
{
	int	fd;
	pid_t pid;

	pid = fork();
	signal(SIGINT, SIG_IGN);
	if (pid == -1)
	{  
		perror("fork");
    	exit(1);
	}
	else if (pid == 0)
	{
		printf("delimitator: %s\n", find_heredoc_delim(cmd[0]));
		/* preguntar a pacheco sobre archivos temporales */
		/* tal vez hacer unlink al abrir y al cerrar */
		fd = open("abueno", O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
		if (fd == -1)
		{
			perror("");
			exit(1);
		}
		/* DEVUELVE DOBLE PROMPT, ARREGLAR */
		signal(SIGINT, SIG_IGN);
		signal(SIGINT, &handle_ctrlc_heredoc);
		while (1)
		{
			if(heredoc_content(cmd[0], fd) == 1)
				break;
		}
		close(fd);
		fd = open("abueno", O_RDONLY); /* Lo abrimos de nuevo después de cerrar para empezar desde el principio del archivo */
        char buffer[1024]; 
        ssize_t bytes_read;
        while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) 
		{
			replace_env_vars(cmd[0], buffer);
			write(1, buffer, strlen(buffer));
			write(1, "\n", 1);
		}
        close(fd);
        exit(0);
	}
	else
	{
		wait(NULL);
		signal(SIGINT, &handle_ctrlc);
		/* BORRAR ARCHIVO CUANDO MANDEMOS SEÑAL DE CTRL-D */
		if (unlink("abueno") == -1)
		{
            perror("unlink");
            exit(1);
        }
		exit(0);
	}
}

// void replace_vars_heredoc(t_cmd *cmd, char *line)
// {
//     char *path;
//     char *aux;
//     char *var;

//     aux = NULL;
// 	aux = line;
// 	var = &aux[1];
// 	path = ft_getenv(var, cmd->env);
// 	if (path != NULL)
// 	{
// 		free(line); // Liberar el token original
//     	line = ft_strdup(path); // Asignar el nuevo token duplicado
// 		free(path);
// 	}
// 	else /* En el caso de que no exista liberamos path y dejamos line como estaba */
// 	{
// 		free(path);
// 		return ;
// 	}	
// }

// int	heredoc_content(t_cmd *cmd, int fd) 
// {
// 	char *line;
// 	char *aux;
// 	int len;

// 	/* Gestionar variables, cadenas literales(QUOTES) */
// 	line = NULL;
// 	aux = readline(">");
// 	len = ft_strlen(line);
// 	line = (char *)malloc(sizeof(char) * (len + 2)); /* > + '\0' */
// 	strcpy(line, aux);
// 	line[len] = '\0';
// 	if(ft_strcmp(line, find_heredoc_delim(cmd)) == 0)
// 	{
// 		free(line);
// 		return(1);
// 	}
// 	else if(line && *line != '\n')
// 	{
// 		if(line[0] == VARIABLE)
// 			replace_vars_heredoc(cmd, line);
// 		ft_putstr_fd(line, fd);
// 		ft_putchar_fd('\n', fd);
// 		free(line);
// 	}
// 	else if(*line == '\n') /* ? */
// 		ft_putchar_fd('\n', fd);
// 	return (0);
// }

// void    heredoc_redirect(t_cmd **cmd)
// {
// 	int	fd;
// 	pid_t pid;

// 	pid = fork();
// 	signal(SIGINT, SIG_IGN);
// 	if (pid == -1)
// 	{  
// 		perror("fork");
//     	exit(1);
// 	}
// 	else if (pid == 0)
// 	{
// 		printf("delimitator: %s\n", find_heredoc_delim(cmd[0]));
// 		/* preguntar a pacheco sobre archivos temporales */
// 		/* tal vez hacer unlink al abrir y al cerrar */
// 		fd = open("/tmp/heredocBURMITO", O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
// 		if (fd == -1)
// 		{
// 			perror("");
// 			exit(1);
// 		}
// 		/* DEVUELVE DOBLE PROMPT, ARREGLAR */
// 		signal(SIGINT, SIG_IGN);
// 		signal(SIGINT, &handle_ctrlc_heredoc);
// 		while (1)
// 		{
// 			if(heredoc_content(cmd[0], fd) == 1)
// 				break;
// 		}
// 		close(fd);
// 		fd = open("/tmp/heredocBURMITO", O_RDONLY); /* Lo abrimos de nuevo después de cerrar para empezar desde el principio del archivo */
//         char buffer[1024]; 
//         ssize_t bytes_read;
//         while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0)
//             write(1, buffer, bytes_read);
//         close(fd);
//         exit(0);
// 	}
// 	else
// 	{
// 		wait(NULL);
// 		signal(SIGINT, &handle_ctrlc);
// 		/* BORRAR ARCHIVO CUANDO MANDEMOS SEÑAL DE CTRL-D */
// 		if (unlink("/tmp/heredocBURMITO") == -1)
// 		{
//             perror("unlink");
//             exit(1);
//         }
// 		exit(0);
// 	}
// }

void close_input_redirect(t_cmd *cmd)
{
	dup2(cmd->stdin, STDIN_FILENO);
    close(cmd->stdin);
}