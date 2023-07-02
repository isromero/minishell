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

int parse_args(t_cmd *cmd)
{
    int len = 0;
    int i = 0;
    int n_tokens = 0;
    cmd->n_tokens = 0;
    cmd->token = NULL;
    cmd->in_single_quote = false;
    cmd->in_double_quote = false;
    if(count_quotes(cmd->line) % 2 != 0)
    {
        printf("-minishell: no closing quote\n");
        return(-1);
    }
    // Cálculo del número de tokens y almacenamiento de los tokens
    while (cmd->line[i] != '\0')
    {
        while (cmd->line[i] == ' ' && cmd->in_single_quote == false && cmd->in_double_quote == false) // Si hay espacios y no está en comillas las saltamos, sino las guardamos
            i++;
        len = check_len_token(cmd, i);
        if(len > 0)
        {
            char *token = (char *)malloc((len + 1) * sizeof(char));
            if (token == NULL)
            {
                printf("malloc error\n");
                clean_tokens(cmd);
                return (-1);
            }
            ft_strncpy(token, cmd->line + i, len);
            token[len] = '\0';
            save_token(cmd, token);
            if(remove_quotes(cmd, n_tokens) == -1)
                return(-1);
            n_tokens++;
            i += len;
        }
        else if(len == -1)
            return(-1);
        else
            i++;
        // En principio esto no hace falta
       /*  else if(len == 0) // En el caso de encontrar comillas seguidas para borrarlas:
            i += 2; */
        // En principio esto no hace falta
        /*   else
        {
            perror("Error: introducir token válido");
            break;
        } */
    }
    // Asegurarse de que el último elemento del arreglo sea NULL
    save_token(cmd, NULL);
    print_tokens(cmd);
    return (0);
}

int count_quotes(char *line)
{
    int i;
    int count;

    i = 0;
    count = 0;
    while(line[i] != '\0')
    {
        if(line[i] == DOUBLE_QUOTE || line[i] == SINGLE_QUOTE)
            count++;
        i++;
    }
    return(count);
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
    return (i);
}

int count_left_double_quotes(char *token)
{
    int i;

    i = 0;
    while (token[i] != SINGLE_QUOTE && token[i] == DOUBLE_QUOTE && token[i] != '\0')
        i++;
    return (i);
}

void remove_double_quotes(char **token)  
{ 
    int len = ft_strlen(*token); 
    int i = 0, j = 0; 
     
    while (i < len)  
    { 
        if ((*token)[i] != DOUBLE_QUOTE) 
        { 
            (*token)[j] = (*token)[i]; 
            j++; 
        } 
        i++; 
    } 
    (*token)[j] = '\0'; 
} 
 
void remove_single_quotes(char **token)  
{ 
    int len = ft_strlen(*token); 
    int i = 0, j = 0; 
     
    while (i < len) 
    { 
        if ((*token)[i] != SINGLE_QUOTE) 
        { 
            (*token)[j] = (*token)[i]; 
            j++; 
        } 
        i++; 
    } 
    (*token)[j] = '\0'; 
} 
 
int remove_quotes(t_cmd *cmd, int i) 
{ 
    size_t double_quotes; 
    size_t single_quotes; 
    int left_double_quotes; 
    int left_single_quotes; 
    size_t len_token; 
 
    single_quotes = count_single_quotes(cmd->token[i]); 
    double_quotes = count_double_quotes(cmd->token[i]); 
    len_token = ft_strlen(cmd->token[i]); 
    left_double_quotes = count_left_double_quotes(cmd->token[i]); 
    left_single_quotes = count_left_single_quotes(cmd->token[i]); 
    if(cmd->token[i][0] == DOUBLE_QUOTE && double_quotes >= 2) 
    { 
        remove_double_quotes(&cmd->token[i]); 
        // Se ejecutará solo cuando sea par en la izquierda y no te pasen comillas distintas sin nada dentro 
        // Ejemplo ""''"" 
        if(left_double_quotes % 2 == 0 && single_quotes + double_quotes != len_token) 
            remove_single_quotes(&cmd->token[i]); 
    } 
    else if(cmd->token[i][0] == SINGLE_QUOTE && single_quotes >= 2) 
    { 
        remove_single_quotes(&cmd->token[i]); 
        // Se ejecutará solo cuando sea par en la izquierda y no te pasen comillas distintas sin nada dentro 
        // Ejemplo ""''"" 
        if(left_single_quotes % 2 == 0 && single_quotes + double_quotes != len_token) 
            remove_double_quotes(&cmd->token[i]); 
    } 
    // Gestión de comillas token vacío con comillas después de borrarlas 
    // Ejemplo: """" 
    if(cmd->token[0] != NULL && ft_strlen(cmd->token[0]) == 0) 
        printf("-minishell: command not found\n"); 
    return (0); 
}
// No gestionado de quotes:
// ejemplo: "'hola"' /* printf("-minishell: error while looking for matching quote\n"); */
/* o h"hola" (texto delante de comillas sin espacio) */
