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

/* En desuso */
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

/* Le pasamos doble puntero porque podemos modificar directamente el valor dentro de la funcion y se verá actualizado en executes por pasarle la memoria */
void replace_vars(t_cmd *cmd, char **token)
{
    size_t token_len;
    char *replaced_token;
    size_t replaced_len;
    size_t j;

    token_len = ft_strlen(*token);
    replaced_token = malloc(token_len + 1);
    replaced_len = 0;
    j = 0;
    while (j < token_len)
    {
        if ((*token)[j] == VARIABLE && (*token)[j + 1] != '\0')
        {
            size_t var_start = j + 1;
            size_t var_len = 0;
            while ((*token)[j + 1 + var_len] != VARIABLE && (*token)[j + 1 + var_len] != '\0' && (*token)[j + 1 + var_len] != SINGLE_QUOTE)
                var_len++;
            char *var = malloc(var_len + 1);
            ft_strncpy(var, *token + var_start, var_len);
            var[var_len] = '\0';
            char *value = ft_getenv(var, cmd->env);
            if (value != NULL)
            {
                size_t value_len = ft_strlen(value);
                char *new_replaced_token = malloc(replaced_len + value_len + 1);
                ft_memcpy(new_replaced_token, replaced_token, replaced_len);
                ft_memcpy(new_replaced_token + replaced_len, value, value_len);
                new_replaced_token[replaced_len + value_len] = '\0';
                free(replaced_token);
                replaced_token = new_replaced_token;
                replaced_len += value_len;
            }
            free(var);
            j += var_len; // Saltar al siguiente símbolo de variable
        }
        else
            replaced_token[replaced_len++] = (*token)[j];
        j++;
    }
    replaced_token[replaced_len] = '\0';
    free(*token);
    *token = replaced_token;
}
