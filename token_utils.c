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

int	is_special(char c)
{
	return(c == INPUT_REDIRECT || c == PIPE || c == OUTPUT_REDIRECT);
}

int	is_variable(char c)
{
	return(c == VARIABLE);
}

int	is_redirects(char c)
{
	return(c == INPUT_REDIRECT || c == OUTPUT_REDIRECT);
}

int	is_redirects_double_char(char *token)
{
	return(ft_strcmp(APPEND_REDIRECT, token) == 0 || ft_strcmp(HEREDOC_REDIRECT, token) == 0);
}

int	is_argument(char c)
{
	return(c == ARGUMENT);
}

int	is_executable(char c)
{
	return(c == '/' || c == '.');
}

int	is_argument_extension(t_cmd *cmd, int i)
{
		//grep $PATH???????????????????????????????????????
	if(cmd->token[i] == NULL)
		return 1;
	else if(cmd->token[i][0] == '-')
		return 1;
	else if(cmd->token[i][0] == '$')
		return 1;
	/* BASTANTE POSIBLE que esto tenga errores en ciertos comandos con 2 argumentos sin '-' etc? */
	/* Esto checkea si el argumento anterior era comando y el actual es una letra, si es así es que es argumento
	con esto se solucionan los cannot access 'argumento' y que no se ejecute de nuevo command not found*/
	else if(i != 0 && ((cmd->token[i][0] >= 'A' && cmd->token[i][0] <= 'Z') \
	 || (cmd->token[i][0] >= 'a' && cmd->token[i][0] <= 'z'))\
	 && (is_command_exists(cmd, cmd->token[i - 1]) \
	 || is_redirects(cmd->token[i - 1][0]) || is_redirects_double_char(cmd->token[i - 1])))
	/* Realmente son solo los redirects */
		return 1;
	/* else if(cmd->token[i][0] == DOUBLE_QUOTE && cmd->token[i][ft_strlen(cmd->token[i]) - 1] == DOUBLE_QUOTE)
		return 1;
	else if(cmd->token[i][0] == SINGLE_QUOTE && cmd->token[i][ft_strlen(cmd->token[i]) - 1] == SINGLE_QUOTE)
		return 1; */
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

int is_double_quote(t_cmd *cmd, int len)
{
	int i;

	cmd->in_double_quote = true;
	i = 0;
	while (cmd->line[i + len] != '\0')
	{
		if (cmd->line[i + len] == DOUBLE_QUOTE && cmd->line[i + len + 1] == ' ')
		{
			i++;
			break ;
		}
		i++;
	}
	cmd->in_double_quote = false;
	return (i);
}

int is_single_quote(t_cmd *cmd, int len)
{
	int	i;
	/* h"hola" */
	cmd->in_single_quote = true;
	i = 1;
	while (cmd->line[i + len] != '\0')
	{
		if (cmd->line[i + len] == SINGLE_QUOTE && cmd->line[i + len + 1] == ' ')
		{
			i++;
			break ;
		}
		i++;
	}
	cmd->in_single_quote = false;
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
	while(cmd->line[i + len] != ' ' && is_special(cmd->line[i + len])) /* cmd->line[i + len] != '\0' hay que meterlo?*/
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

int cmd_token_len(t_cmd *cmd, int len)
{
    int i = 0;
	/* en principio no hace falta checkear los appends y heredocs ya que los detectará este */
    while (cmd->line[i + len] != '\0' && cmd->line[i + len] != ' ' && !is_special2(cmd->line[i + len]))
        i++;
    return i;
}

int	check_len_token(t_cmd *cmd, int len)
{
	while(cmd->line[len] != '\0')
	{
		// No necesario en principio
		/* while(cmd->line[i + len] != '\0' && cmd->line[i + len] != ' ')
			i++; */
		if(cmd->line[len] == VARIABLE)
			return(cmd_token_len(cmd, len));
		else if(cmd->line[len] == SINGLE_QUOTE)
			return(is_single_quote(cmd, len));
		else if(cmd->line[len] == DOUBLE_QUOTE)
			return(is_double_quote(cmd, len));
		else if(is_special2(cmd->line[len])) // << < > >> | 
			return(check_len_special(cmd, len));
		else if(cmd->line[len] != '\0')
			return(cmd_token_len(cmd, len));
	}
	return (0);
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
