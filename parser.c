/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 17:53:11 by isromero          #+#    #+#             */
/*   Updated: 2023/05/26 17:53:11 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// $USER$PATH NO FUNCIONA EL PARSEO ENTRE VARIABLES JUNTAS /////////////////////////////////////////////////////////////////////

void parse_args(t_cmd *cmd)
{
    int len = 0;
    int i = 0;
    cmd->n_tokens = 0;
    cmd->token = NULL;
    // Cálculo del número de tokens y almacenamiento de los tokens
    while (cmd->line[i] != '\0')
    {
        while (cmd->line[i] == ' ')
            i++;
        len = check_len_token(cmd, i);
        if(len == -1)
        {
            printf("-minishell: no closing quote\n");
            break;
        }
        else if(len > 0)
        {
            char *token = (char *)malloc((len + 1) * sizeof(char));
            if (token == NULL)
            {
                perror("Error: No se pudo asignar memoria para el token");
                clean_tokens(cmd);
                return;
            }
            ft_strncpy(token, cmd->line + i, len);
            token[len] = '\0';
            save_token(cmd, token);
            i += len;
        }
        /* En principio esto no hace falta */
        /*   else
        {
            perror("Error: introducir token válido");
            break;
        } */
    }
    // Asegurarse de que el último elemento del arreglo sea NULL
    save_token(cmd, NULL);
    print_tokens(cmd);
}