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