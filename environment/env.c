/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 17:48:36 by isromero          #+#    #+#             */
/*   Updated: 2023/05/26 17:48:36 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_getenv(const char *name, char **env) 
{
	size_t name_len = ft_strlen(name);
	char *value;
	char *value_start;
	char *value_end;
	int i;

	value = NULL;
	i = 0;
	while (env[i] != NULL && env[i][0] != ' ')
	{
		if (ft_strncmp(name, env[i], name_len) == 0 && env[i][name_len] == '=')
		{
			value_start = env[i] + name_len + 1;  // Puntero al inicio del valor después del '='
			// Buscar el primer carácter de tubería '|' o el final de línea '\0'
			value_end = ft_strchr(value_start, '|');
			if (value_end == NULL)
				value_end = ft_strchr(value_start, '\0');
			// Calcular la longitud del valor y copiarlo en una nueva cadena
			size_t value_len = value_end - value_start + 1;
			value = (char *)malloc((value_len + 1) * sizeof(char));
			if (value == NULL)
			{
				free(value);
				return NULL;
			}
			ft_strncpy(value, value_start, value_len);
			value[value_len] = '\0';
			return (value);
		}
		i++;
	}
	// La variable de entorno no fue encontrada
	return NULL;
}

void	free_env(char **env)
{
	if (env == NULL)
		return;
	for (int i = 0; env[i] != NULL; i++)
		free(env[i]);
	free(env);
}

void	malloc_env(t_cmd *cmd, char **env)
{
	int i;
	int env_count;

	i = 0;
	env_count = 0;
	while (env[i] != NULL)
	{
		env_count++;
		i++;
	}
	cmd->env = malloc((env_count + 1) * sizeof(char *));
	if (cmd->env == NULL)
		return;
}

char	**ft_strstrdup(char **strstr)
{
	char **dup;

	int j = 0;
	while (strstr && strstr[j])
		j++;
	dup = malloc((j + 1) * sizeof(char *));
	if (!dup)
		return (NULL);
	int i = 0;
	while (strstr[i])
	{
		dup[i] = ft_strdup(strstr[i]);
		i++;
	}
	dup[i] = NULL;
	return (dup);
}

void	init_env(t_cmd *cmd, char **env) 
{
	if (env[0] == NULL) /* if env is NULL */
		get_default_env(cmd);
	else
		cmd->env = ft_strstrdup(env);
}

void	get_default_env(t_cmd *cmd)
{
	char pwd[4096];
	int env_count = 5; // Número de variables de entorno predeterminadas
	int env_index = 0;
	int i = 0;
	char **default_env;
	
	default_env = malloc((env_count + 1) * sizeof(char *));
	default_env[env_index++] = ft_strjoin("PWD=", getcwd(pwd, sizeof(pwd)));
	default_env[env_index++] = ft_strdup("SHLVL=1");
	default_env[env_index++] = ft_strdup("PATH=/usr/local/sbin:/usr/local/bin:/usr/bin:/bin");
	default_env[env_index++] = ft_strjoin("_=", "./minishell");
	default_env[env_index] = NULL;
	cmd->env = malloc((env_count + 1) * sizeof(char *));
	while (i < env_count)
	{
		cmd->env[i] = default_env[i];
		i++;
	}
	cmd->env[env_count] = NULL;
	free_matrix(default_env);
}


////////////////////////////////////// FUNCIÓN QUE HAY QUE ARREGLAR ////////////////////////////////////////////////////////////////////////////

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