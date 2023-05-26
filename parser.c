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
            strncpy(token, cmd->line + i, len); //METER STRNCPY
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
