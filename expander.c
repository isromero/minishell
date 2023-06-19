/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 17:51:23 by isromero          #+#    #+#             */
/*   Updated: 2023/05/26 17:51:23 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int special_for_vars(char c)
{
	return(c == INPUT_REDIRECT || c == PIPE || c == OUTPUT_REDIRECT);
}

void print_vars(t_cmd *cmd, int i)
{

	char *path;
	char *aux;
	char *var;

	aux = NULL;
	if(cmd->token[i][0] == VARIABLE)
	{
		aux = cmd->token[i];
		var = &aux[1];
		path = ft_getenv(var, cmd->env);
		if(path == NULL)
			return ;
		printf("-minishell: %s\n", path);
		free(path);
	}
}

void replace_vars(t_cmd *cmd, int i)
{
    char *path;
    char *aux;
    char *var;

    aux = NULL;
    while (cmd->token[i] != NULL && !special_for_vars(cmd->token[i][0]))
    {
        if (cmd->token[i][0] == VARIABLE)
        {
            aux = cmd->token[i];
            var = &aux[1];
            path = ft_getenv(var, cmd->env);
            if (path != NULL)
            {
                replace_token(cmd, i, path); // Sustituir el token por el valor expandido
                free(path);
            }
        }
        i++;
    }
}

void replace_token(t_cmd *cmd, int index, const char *new_token)
{
    free(cmd->token[index]); // Liberar el token original
    cmd->token[index] = ft_strdup(new_token); // Asignar el nuevo token duplicado
}
