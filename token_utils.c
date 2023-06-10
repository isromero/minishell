/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 17:49:03 by isromero          #+#    #+#             */
/*   Updated: 2023/05/26 17:49:03 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void clean_tokens(t_cmd *cmd)
{
    for (int i = 0; i < cmd->n_tokens; i++)
    {
        free(cmd->token[i]);
    }
    free(cmd->token);
    cmd->token = NULL;
    cmd->n_tokens = 0;
}

void print_tokens(t_cmd *cmd)
{
    int i;
    for (i = 0; i < cmd->n_tokens; i++)
		printf("Token %d: %s\n", i, cmd->token[i]);
}

void save_token(t_cmd *cmd, char *token)
{
    cmd->n_tokens++;
    char **new_token = (char **)malloc(cmd->n_tokens * sizeof(char *));
    if (new_token == NULL)
    {
        printf("Error: No se pudo asignar memoria para cmd->token\n");
        return;
    }
    if (cmd->token != NULL)
    {
        for (int i = 0; i < cmd->n_tokens - 1; i++)
        {
            new_token[i] = cmd->token[i];
        }
        free(cmd->token);
    }
    new_token[cmd->n_tokens - 1] = token;
    cmd->token = new_token;
}

void count_pipes(t_cmd *cmd)
{
    int i = 0;
	cmd->n_pipes = 0;
	cmd->n_processes = 0;
    while (cmd->token[i] != NULL)
	{
		if(cmd->token[i][0] == '|')
			cmd->n_pipes++;
		i++;
	}
	cmd->n_processes = cmd->n_pipes + 1;
}

int find_len_last_command(t_cmd *cmd)
{
    int i;

    i = cmd->n_tokens - 2; // 5
    while(i >= 0)
    {
        if(cmd->token[i][0] == '|')
            return (i + 1); // La posición del token después del último pipe
        i--;
    }
    return (0);
}

int	is_special(char c)
{
	return(c == INPUT_REDIRECT || c == PIPE || c == OUTPUT_REDIRECT \
	|| c == VARIABLE);
}

int	is_argument(char c)
{
	return(c == ARGUMENT);
}

int	is_argument_extension(t_cmd *cmd, int i)
{
		//grep $PATH???????????????????????????????????????
	if(cmd->token[i] == NULL)
		return 1;
	else if(cmd->token[i][0] == '-')
		return 1;
	/* BASTANTE POSIBLE que esto tenga errores en ciertos comandos con 2 argumentos sin '-' etc? */
	/* Esto checkea si el argumento anterior era comando y el actual es una letra, si es así es que es argumento
	con esto se solucionan los cannot access 'argumento' y que no se ejecute de nuevo command not found*/
	else if(i != 0 && ((cmd->token[i][0] >= 'A' && cmd->token[i][0] <= 'Z') \
	 || (cmd->token[i][0] >= 'a' && cmd->token[i][0] <= 'z'))\
	 && (is_command_exists(cmd, cmd->token[i - 1]) || \
	/* Realmente son solo los redirects */is_special(cmd->token[i - 1][0])))
		return 1;
	else if(cmd->token[i][0] == DOUBLE_QUOTE && cmd->token[i][ft_strlen(cmd->token[i]) - 1] == DOUBLE_QUOTE)
		return 1;
	else if(cmd->token[i][0] == SINGLE_QUOTE && cmd->token[i][ft_strlen(cmd->token[i]) - 1] == SINGLE_QUOTE)
		return 1;
	else if(i >= 2 && ft_strcmp("echo", cmd->token[i - 2]) == 0 && ft_strcmp("-n", cmd->token[i - 1]) == 0)
		return 1;
	else if(i >= 1 && ft_strcmp("echo", cmd->token[i - 1]) == 0 && ft_strcmp("-n", cmd->token[i]) == 0)
		return 1;
	else if(i >= 1 && ft_strcmp("echo", cmd->token[i - 1]) == 0)
		return 1;
	else if(i >= 1 && ft_strcmp("echo", cmd->token[i - 1]) == 0 && (cmd->token[i][0] == DOUBLE_QUOTE && cmd->token[i][ft_strlen(cmd->token[i]) - 1] == DOUBLE_QUOTE))
		return 1;
	else if(i >= 1 && ft_strcmp("echo", cmd->token[i - 1]) == 0 && (cmd->token[i][0] == SINGLE_QUOTE && cmd->token[i][ft_strlen(cmd->token[i]) - 1] == SINGLE_QUOTE))
		return 1;
	//gestionar export con pipes
	else if(i >= 1 && ft_strcmp("export", cmd->token[i - 1]) == 0 && ft_strchr(cmd->token[i], '='))
		return 1;
	else if(i >= 1 && ft_strcmp("unset", cmd->token[i - 1]) == 0)
		return 1;
	else if(i >= 1 && ft_strcmp("pwd", cmd->token[i - 1]) == 0)
		return 1;
	else if(i >= 1 && ft_strcmp("cd", cmd->token[i - 1]) == 0)
		return 1;
	return(0);
}

int	is_pipe(char c)
{
	return(c == PIPE);
}

int	is_special2(char c)
{
	return(c == INPUT_REDIRECT || c == PIPE || c == OUTPUT_REDIRECT);
}

int	is_variable(t_cmd *cmd, int len)
{
	int	i;

	i = 0;
	while(cmd->line[i + len] != ' ' && cmd->line[i + len] != '\0' && !is_special2(cmd->line[i + len]))
		i++;
	return (i);
}

// ARREGLAR COMILLAS AL FINAL SOLO, NO DETECTA
int is_double_quote(t_cmd *cmd, int len)
{
	int i;

	i = 1;
	if(!ft_strchr(&cmd->line[1], DOUBLE_QUOTE))
			return (-1);
	while(cmd->line[i + len] != '\0' && cmd->line[i + len] != DOUBLE_QUOTE)
		i++;
	i++;
	return (i);
}

int is_single_quote(t_cmd *cmd, int len)
{
	int	i;

	i = 1;
	if(!ft_strchr(&cmd->line[1], SINGLE_QUOTE))
		return (-1);
	while(cmd->line[i + len] != '\0' && cmd->line[i + len] != SINGLE_QUOTE)
		i++;
	i++;
	return (i);
}

void error_special()
{
	printf("syntax error near unexpected token\n");
}

int check_len_special(t_cmd *cmd, int len)
{
	int i;

	i = 0;
	while(cmd->line[i + len] != ' ' && is_special(cmd->line[i + len]))
		i++;
	// Gestión de errores
	if(i > 2)
		error_special();
	else if(i == 1)
		return(1);
	else if (i == 2)
		return(2);
	return (-1);
}

int is_token(t_cmd *cmd, int len)
{
    int i = 0;
    while (cmd->line[i + len] != '\0' && cmd->line[i + len] != ' ' && !is_special2(cmd->line[i + len]))
        i++;
    return i;
}

int	check_len_token(t_cmd *cmd, int len)
{
	int i = 0;
	while(cmd->line[len] != '\0')
	{
		while(cmd->line[i + len] != '\0' && cmd->line[i + len] != ' ')
			i++;
		if(cmd->line[len] == VARIABLE)
			return(is_variable(cmd, len));
		else if(cmd->line[len] == SINGLE_QUOTE)
			return(is_single_quote(cmd, len));
		else if(cmd->line[len] == DOUBLE_QUOTE)
			return(is_double_quote(cmd, len));
		else if(is_special2(cmd->line[len] )) // < > | 
			return(check_len_special(cmd, len)); // << < > >> |
		else if(cmd->line[len] != '\0')
			return(is_token(cmd, len));
	}
	return i;
}

int	find_variables(char **token)
{
	int i;

	i = 0;
	while(token[i][0] != '\0')
	{
		if(token[i][0] == VARIABLE)
			return(1);
		i++;
	}
	return (0);
}
