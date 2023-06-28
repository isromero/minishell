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

int parse_args(t_cmd *cmd)
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
        if(len > 0)
        {
            char *token = (char *)malloc((len + 1) * sizeof(char));
            if (token == NULL)
            {
                perror("Error: No se pudo asignar memoria para el token");
                clean_tokens(cmd);
                return (-1);
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
    if(remove_quotes(cmd) % 2 != 0)
    {
        printf("-minishell: no closing quote\n");
        return (-1);
    }
    print_tokens(cmd);
    return (0);
}

int count_double_quotes(char *token)
{
    int i;
    int count;

    i = 0;
    count = 0;
    while(token[i] != '\0')
    {
        if(token[i] == DOUBLE_QUOTE)
            count++;
        i++;
    }
    return(count);
}

int count_single_quotes(char *token)
{
    int i;
    int count;

    i = 0;
    count = 0;
    while(token[i] != '\0')
    {
        if(token[i] == SINGLE_QUOTE)
            count++;
        i++;
    }
    return(count);
}

int count_left_single_quotes(char *token)
{
    int i;

    i = 0;
    while (token[i] != DOUBLE_QUOTE && token[i] == SINGLE_QUOTE && token[i] != '\0')
        i++;
    if(i % 2 == 0) /* Si las dobles comillas son un numero par tiene que borrar las comillas simples también */
    {
        while(token[i] != SINGLE_QUOTE && token[i] == DOUBLE_QUOTE && token[i] != '\0')
            i++;
    }
    return (i);
}

int count_left_double_quotes(char *token)
{
    int i;

    i = 0;
    while (token[i] != SINGLE_QUOTE && token[i] == DOUBLE_QUOTE && token[i] != '\0')
        i++;
    if(i % 2 == 0) /* Si las dobles comillas son un numero par tiene que borrar las comillas simples también */
    {
        while(token[i]!= DOUBLE_QUOTE && token[i] == SINGLE_QUOTE && token[i] != '\0')
            i++;
    }
    return (i);
}


int remove_quotes(t_cmd *cmd)
{
    int i;
    int double_quotes;
    int single_quotes;

    i = 0;
    while(cmd->token[i] != NULL)
    {
        single_quotes = count_single_quotes(cmd->token[i]);
        double_quotes = count_double_quotes(cmd->token[i]);
        if(cmd->token[i][0] == DOUBLE_QUOTE && double_quotes >= 2)
            cmd->token[i] = ft_substr(cmd->token[i], count_left_double_quotes(cmd->token[i]), ft_strlen(cmd->token[i]) - (count_left_double_quotes(cmd->token[i]) * 2));
        else if(cmd->token[i][0] == SINGLE_QUOTE && single_quotes >= 2)
            cmd->token[i] = ft_substr(cmd->token[i], count_left_single_quotes(cmd->token[i]), ft_strlen(cmd->token[i]) - (count_left_single_quotes(cmd->token[i]) * 2));
        i++;
    }
    return(single_quotes + double_quotes); // Si devuelve un número impar de comillas es que hay un error
}