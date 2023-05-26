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
    echo_token++; // Avanzar a la posición después de "echo"

    while (cmd->token[echo_token])
    {
        printf("%s ", cmd->token[echo_token]);
        echo_token++;
    }

    printf("\n");
	return(echo_token);
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
        printf("minishell: env: Demasiados argumentos\n"); // Cambiar error
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
    return (status);
}

int	ft_pwd(t_cmd *cmd)
{
	char	dir[1024];
	int		s;
	char	*pwd;

	s = 1;
	pwd = ft_getenv("$PWD", cmd->env);
	if (getcwd(dir, sizeof(dir)) != NULL)
	{
		printf("%s\n", dir);
		s = 0;
	}
	else if (pwd)
		printf("%s\n", pwd);
	else
		perror("");
	free (pwd);
	//comprobacion pipe
	return (s);
}

void ft_export(t_cmd *cmd, int export_token) // CHECKEAR SI HAY UN '=' PARA EXPORT
{
	int i = 0, len_of_env = 0, len_of_export = 0;
	char **new_env = NULL;
	int j;

	// Calcular la longitud del entorno existente
	while (cmd->env[len_of_env] != NULL)
		len_of_env++;
	// Calcular la longitud del nuevo export
	while (cmd->token[export_token + 1][len_of_export] != '\0')
	{
		// Aquí puedes realizar cualquier validación o modificación adicional
		len_of_export++;
	}
	// Asignar memoria para el nuevo entorno
	new_env = (char **)malloc(sizeof(char *) * (len_of_env + 2));
	if (new_env == NULL)
	{
		// Error de asignación de memoria
		return;
	}
	// Copiar el entorno existente al nuevo entorno
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
	// Añadir la nueva variable al nuevo entorno
	new_env[len_of_env] = ft_strdup(cmd->token[export_token + 1]);
	if (new_env[len_of_env] == NULL)
	{
		// Error de asignación de memoria
		j = 0;
		while (j < len_of_env)
		{
			free(new_env[j]);
			j++;
		}
		free(new_env);
		return;
	}
	// Establecer el último elemento del nuevo entorno como NULL
	new_env[len_of_env + 1] = NULL;
	// Liberar el entorno anterior y asignar el nuevo entorno
	cmd->env = new_env;
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

void ft_unset(t_cmd *cmd, int unset_token)
{
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
	free(cmd->env);
	free(cmd->prompt); 
	
    // Salir del programa con el código de salida especificado
    exit(exit_code);
}

void	ft_exit(t_cmd *cmd, int exit_token)
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