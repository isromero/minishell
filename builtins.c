/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 17:47:35 by isromero          #+#    #+#             */
/*   Updated: 2023/05/26 17:47:35 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int ft_echo(t_cmd *cmd, int echo_token)
{
    int first_echo_token;
	int	first_iteration;

    first_echo_token = echo_token;
	first_iteration = 0;
    echo_token++; // Avanzar a la posición después de "echo"
	if (ft_strcmp(cmd->token[0], "echo") == 0 && ft_strcmp(cmd->token[1], "$?") == 0) 
	{
    	ft_putnbr_fd(g_status, STDOUT_FILENO);
		ft_putchar_fd('\n', STDOUT_FILENO);
		return (0);
	}
	if(ft_strcmp(cmd->token[first_echo_token + 1], "-n") == 0)
		echo_token++;
	while (cmd->token[echo_token])
	{
		 if(cmd->no_expand_vars[echo_token] == 0)
            replace_vars(cmd, &cmd->token[echo_token]);
		if(is_special(cmd->token[echo_token][0]) && (cmd->in_single_quote == 0 || cmd->in_double_quote == 0))
			break ;
		if(first_iteration != 0)
			printf(" %s", cmd->token[echo_token]);
		else if(first_iteration == 0)
		{
			printf("%s", cmd->token[echo_token]);
			first_iteration++;
		}
		echo_token++;
	}
	if (ft_strcmp(cmd->token[first_echo_token + 1], "-n") != 0)
		printf("\n");
    return (echo_token);
}

void	ft_cd(t_cmd *cmd, int cd_token)
{
	char	*path;
	char	cwd[1024];
	char	*oldpwd;

	path = cmd->token[cd_token + 1];
	oldpwd = ft_getenv("PWD", cmd->env);

	//chdir falla al cambiar al directorio especificado, no devuelve 0 
	if (chdir(path) != 0)
		printf("-minishell: cd: %s: Not a directory\n", path);
	getcwd(cwd, sizeof(cwd));
	setenv(oldpwd, cwd, 1);
}


/* CADA VEZ QUE SE LLAMA O CUANDO HACEMOS GETENV LA ULTIMA VARIABLE TIENE QUE CONTENER EL ULTIMO COMANDO ASI QUE: _=/usr/bin/env */
void ft_env(t_cmd *cmd)
{
    int i;
    int size;

    i = 0;
    if (!cmd->env || !cmd->env[0])
        return ;

    /* if (cmd->n_tokens > 2) // Hace falta gestionarlo????????????????????????????????
    {
		printf("%d\n", cmd->n_tokens);
        printf("minishell: env: Demasiados argumentos\n"); // Cambiar error
        return 127;
    } */
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
}

void	ft_pwd(t_cmd *cmd)
{
	char	dir[1024];
	char	*pwd;

	pwd = ft_getenv("$PWD", cmd->env);
	if (getcwd(dir, sizeof(dir)) != NULL)
		printf("%s\n", dir);
	else if (pwd)
		printf("%s\n", pwd);
	else
		perror("");
	free (pwd);
	//comprobacion pipe
}

void free_env(char **env)
{
    if (env == NULL)
        return;
    for (int i = 0; env[i] != NULL; i++)
        free(env[i]);
    free(env);
}

/*	CHECKEAR GESTIÓN DE EXPORT SIN ARGUMENTOS */
/* LEAK EXPORT */
void ft_export(t_cmd *cmd, int export_token) 
{
	int i = 0, len_of_env = 0;
	int	pos_var = 0;
	char **new_env = NULL;
	int	j;
	while (cmd->env[len_of_env] != NULL)
		len_of_env++;
	new_env = (char **)malloc(sizeof(char *) * (len_of_env + 2));
	i = 0;
	while (cmd->env[i] != NULL)
	{
		new_env[i] = ft_strdup(cmd->env[i]);
		if (new_env[i] == NULL)
		{
			// Error de asignación de memoria
			j = 0;
			while (j < i)
			{
				free(new_env[j]);
				j++;
			}
			free(new_env);
			return;
		}
		i++;
	}
	if(var_exists(cmd, cmd->token[export_token + 1]) == 1)
	{
		pos_var = len_var_in_env(cmd, cmd->token[export_token + 1]);
		new_env[pos_var] = ft_strdup(ft_strreplace(new_env[pos_var], new_env[pos_var], cmd->token[export_token + 1]));
	}
	else if(var_exists(cmd, cmd->token[export_token + 1]) == 0)
		new_env[len_of_env] = ft_strdup(cmd->token[export_token + 1]);
	if (new_env[len_of_env] == NULL)
	{
		j = 0;
		while (j < len_of_env)
		{
			free(new_env[j]);
			j++;
		}
		free(new_env);
		return;
	}
	new_env[len_of_env + 1] = NULL;
	cmd->env = new_env;
}

int len_var_in_env(t_cmd *cmd, char *token)
{
	int i;
	int	j;
	char *aux;
	char *aux_token;

	i = 0;
	aux = NULL;
	aux_token = NULL;
	aux_token = malloc(ft_strlen(token) + 1);
	ft_strcpy(aux_token, token);
    while (aux_token[i] != '=')
        i++;
    aux_token[i] = '\0';
	i = 0;
	while (cmd->env[i] != NULL)
	{
		j = 0;
		aux = malloc(ft_strlen(cmd->env[i]) + 1);
		while(cmd->env[i][j] != '=')
		{
			aux[j] = cmd->env[i][j];
			j++;
		}
		aux[j] = '\0';
        if (ft_strcmp(aux, aux_token) == 0)
        {
            free(aux);
			free(aux_token);
            return i;
        }
        free(aux);
		i++;
	}
	free(aux_token);
	return 0;
} 

int var_exists(t_cmd *cmd, char *token)
{
    int i = 0;
    char *aux_token = malloc(ft_strlen(token) + 1);
    int token_idx = 0;

    while (token[i] != '=')
    {
        aux_token[token_idx] = token[i];
        i++;
        token_idx++;
    }
    aux_token[token_idx] = '\0';
    i = 0;
    while (cmd->env[i] != NULL)
    {
        int j = 0;
        while (cmd->env[i][j] != '=')
        {
            if (cmd->env[i][j] != aux_token[j])
                break;
            j++;
        }
        if (cmd->env[i][j] == '=' && aux_token[j] == '\0')
        {
            free(aux_token);
            return 1;
        }
        i++;
    }
    free(aux_token);
    return 0;
}


bool compareVariableName(const char* variable, const char* name)
{
    int i = 0;

    while (variable[i] != '=' && name[i] != '\0')
    {
        if (variable[i] != name[i])
            return false;
        i++;
    }
    // Verificar si el nombre de la variable tiene la misma longitud
    // que el nombre en el token
    return (variable[i] == '=' && name[i] == '\0');
}

/* CHECKEAR GESTIÓN DE UNSET SIN ARGUMENTOS */

void ft_unset(t_cmd *cmd, int unset_token)
{
	// Meter perrors con errores de argumentos(bad identifiers)
    int i = 0, j = 0;
    int len_of_env = 0;
    char **new_env = NULL;

    while (cmd->env[len_of_env] != NULL)
        len_of_env++;

    new_env = (char **)malloc(sizeof(char *) * (len_of_env + 1));
	if(new_env == NULL)
		return ;
    while (cmd->env[i] != NULL)
    {
        if (!compareVariableName(cmd->env[i], cmd->token[unset_token + 1]))
        {
            new_env[j] = ft_strdup(cmd->env[i]);
            j++;
        }
        i++;
    }
    new_env[j] = NULL;
    cmd->env = new_env;
}

void execute_builtin_exit(t_cmd *cmd, int exit_code)
{
    // Realiza cualquier limpieza necesaria antes de salir
	// Esto vale?????????????????????????????
	clean_tokens(cmd);
	free(cmd->line);
	free(cmd->prompt);
    // Salir del programa con el código de salida especificado
    exit(exit_code);
}

void	ft_exit(t_cmd *cmd, int exit_token) // Con textos de argumentos creo que hay que devolver 2
{
	int	exit_code;

	if(is_number(cmd->token[exit_token + 1]) && ft_atoi(cmd->token[exit_token + 1]) >= INT_MIN && ft_atoi(cmd->token[exit_token + 1]) <= INT_MAX) // Si es un número cualquiera entonces es que hay un argumento número y:
	{
		exit_code = ft_atoi(cmd->token[exit_token + 1]);
		if(exit_code >= 0 && exit_code <= 255) // Habría que checkear si le pasas exit "numero random" debe devolver ese numero random o 1 predeterminado PENDIENTE
			execute_builtin_exit(cmd, exit_code);
		else if(exit_code < 0 || exit_code > 255) // Esto significa que está fuera de los límites del exit, y bash hace el % del valor pasado para hacer que esté en el rango válido
		{
			exit_code = (exit_code % 256);
			execute_builtin_exit(cmd, exit_code);
		}
	}
	else
		execute_builtin_exit(cmd, 1); // En el caso de que no sea ningun número ni nada devolvemos un error predeterminado, el 1
}