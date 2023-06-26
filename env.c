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

#include "minishell.h"

char *ft_getenv(const char *name, char **env) 
{
    size_t name_len = ft_strlen(name);
    int i;

    i = 0;
    while (env[i] != NULL && env[i][0] != ' ')
    {
        if (ft_strncmp(name, env[i], name_len) == 0 && env[i][name_len] == '=')
        {
            char *value_start = env[i] + name_len + 1;  // Puntero al inicio del valor después del '='
            // Buscar el primer carácter de tubería '|' o el final de línea '\0'
            char *value_end = ft_strchr(value_start, '|');
            if (value_end == NULL)
                value_end = ft_strchr(value_start, '\0');
            // Calcular la longitud del valor y copiarlo en una nueva cadena
            size_t value_len = value_end - value_start + 1;
            char *value = (char *)malloc((value_len + 1) * sizeof(char));
            if (value == NULL)
            {
                // Manejo de error por falta de memoria
                return NULL;
            }
            ft_strncpy(value, value_start, value_len);
            value[value_len] = '\0';
            return value;
        }
        i++;
    }
    // La variable de entorno no fue encontrada
    return NULL;
}

void get_default_env(t_cmd *cmd, char **env)
{
    char pwd[1024];
    char *pwd_command;

    getcwd(pwd, sizeof(pwd));
    int env_index = 0;
    while (env[env_index] != NULL)
    {
        if (strncmp(env[env_index], "PWD=", 4) == 0)
        {
            free(env[env_index]);
            env[env_index] = malloc(strlen(pwd) + 5);
            sprintf(env[env_index], "PWD=%s", pwd);
        }
        else if (strncmp(env[env_index], "SHLVL=", 6) == 0)
        {
            free(env[env_index]);
            env[env_index] = strdup("SHLVL=1");
        }
        else if (strncmp(env[env_index], "OLDPWD=", 7) == 0)
        {
            free(env[env_index]);
            env[env_index] = strdup("OLDPWD=");
        }
        else if (strncmp(env[env_index], "_=", 2) == 0)
        {
            free(env[env_index]);
            env[env_index] = malloc(strlen(pwd) + 14);
            sprintf(env[env_index], "_=%s/minishell", pwd);
        }
        env_index++;
    }
    cmd->env = env;
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