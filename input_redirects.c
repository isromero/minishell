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
	char *delim;
	size_t delim_len;

	delim = NULL;
	delim_len = 0;
	len = cmd->n_tokens - 2;
	while (len >= 0)
	{
		if (ft_strcmp(cmd->token[len], HEREDOC_REDIRECT) == 0)
		{
			delim = cmd->token[len + 1];
			delim_len = ft_strlen(delim);
			if ((delim[0] == '\'' || delim[0] == '\"') && delim_len > 1 && delim[delim_len - 1] == delim[0]) /* Si delim_len > 1 significa que no es solo un carácter y que puede tener comillas */
			{
				cmd->in_quote_heredoc = 1;
				memmove(delim, delim + 1, delim_len - 2);
				delim[delim_len - 2] = '\0';
			}
			else if((delim[0] == '\'' || delim[0] == '\"') && delim_len > 1 && delim[delim_len - 1] != delim[0] && cmd->in_quote_heredoc == 0) /* En bash si no cierras comillas del delimitador no puedes cerrar el proceso con el delimitador */
			{
				printf("MICOÑOGORDO=%c\n",delim[0]);
				printf("MICOÑOGORDO=%d\n",delim[delim_len - 1]);
				delim = "NOT DELIMITATORXXxXxxXXxxXXXXXX"; /* nombre inventado para no poder cerrar el proceso del heredoc */
			}
			int b;
			b = 0;
			while(delim[b])
				printf("delimitator: %d\n", delim[b++]);
			return delim;
		}
		len--;
	}
	return (0);
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

void replace_vars_heredoc(t_cmd *cmd, char *buffer, int i) /* parece tener algunos errores de memoria con la longitud muy de vez en cuando, CHECKEAR!!!!!!!!!! */
{
    char *path;
    char *var;
    int var_length;
    int j;

    var = NULL;
	var_length = 0;
    j = ++i;
    while (buffer[j] != '\0' && buffer[j] != ' ' && buffer[j] != '\n' &&
           buffer[j] != '$' && buffer[j] != '\t' && !is_special(buffer[j]) && !is_redirects(buffer[j]))  /* más comprobaciones??? \t??*/
    {
        var_length++;
        j++;
    }
    var = malloc(var_length);
    ft_strncpy(var, &buffer[i], var_length);
    var[var_length] = '\0';
    path = ft_getenv(var, cmd->env);
    if (path != NULL)
    {
        // Crear una nueva cadena con el reemplazo
        size_t replace_length = ft_strlen(path);
        char *replacement = malloc(replace_length);
        strncpy(replacement, path, replace_length);
        replacement[replace_length] = '\0';

        // Actualizar el buffer con el reemplazo
        char *start = &buffer[i - 1];
        char *end = &buffer[i - 1 + var_length];
        ft_memmove(start + replace_length, end + 1, strlen(end));
        ft_memcpy(start, replacement, replace_length);
        free(path);
        free(replacement);
    }
}

void replace_env_vars(t_cmd *cmd, char *buffer) 
{
	int	i;

	i = 0;
	while(buffer[i] != '\0')
	{
		if(buffer[i] == '$')
			replace_vars_heredoc(cmd, buffer, i);
		i++;
	}
}

void    heredoc_redirect(t_cmd **cmd)
{
	int	fd;
	pid_t pid;

	cmd[0]->in_quote_heredoc = 0;
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
		fd = open("/tmp/heredocBURMITO", O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
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
		fd = open("/tmp/heredocBURMITO", O_RDONLY); /* Lo abrimos de nuevo después de cerrar para empezar desde el principio del archivo */
        char buffer[1024]; 
        ssize_t bytes_read;
        while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) 
		{
			char *delim = find_heredoc_delim(cmd[0]);
			if (delim && delim[0] != '\"' && delim[0] != '\'' && cmd[0]->in_quote_heredoc == 0) /* cosas como "'a" NO FUNCIONAN, gestionar??????*/
				replace_env_vars(cmd[0], buffer);
			write(1, buffer, strlen(buffer));
		}
        close(fd);
        exit(0);
	}
	else
	{
		wait(NULL);
		signal(SIGINT, &handle_ctrlc);
		/* BORRAR ARCHIVO CUANDO MANDEMOS SEÑAL DE CTRL-D */
		if (unlink("/tmp/heredocBURMITO") == -1)
		{
            perror("unlink");
            exit(1);
        }
		exit(0);
	}
}

void close_input_redirect(t_cmd *cmd)
{
	dup2(cmd->stdin, STDIN_FILENO);
    close(cmd->stdin);
}