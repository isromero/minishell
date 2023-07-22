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

void	clean_tokens(t_cmd *cmd)
{
	int i;

	i = 0;
	while (i < cmd->n_tokens)
	{
		free(cmd->token[i]);
		i++;
	}
	free(cmd->token);
	cmd->token = NULL;
	cmd->n_tokens = 0;
}

void	print_tokens(t_cmd *cmd)
{
	int i;

	i = 0;
	while (i < cmd->n_tokens)
	{
		printf("Token %d: %s\n", i, cmd->token[i]);
		i++;
	}
}

void save_token(t_cmd *cmd, char *token)
{
	int i;
	char **new_token;

	cmd->n_tokens++;
	new_token = (char **)malloc(cmd->n_tokens * sizeof(char *));
	if (new_token == NULL)
	{
		printf("Error: No se pudo asignar memoria para cmd->token\n");
		return;
	}
	i = 0;
	if (cmd->token != NULL)
	{
		while (i < cmd->n_tokens - 1)
		{
			new_token[i] = cmd->token[i];
			i++;
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

int	is_executable(t_cmd *cmd, char c)
{
	if(cmd->replaced_var == 1)
		return (0);
	return(c == '/' || c == '.');
}

int	is_argument_extension(t_cmd *cmd, int i)
{
		//grep $PATH???????????????????????????????????????
	if(cmd->token[i] == NULL)
		return 1;
	else if(cmd->token[i][0] == '-')
		return 1;
/* 	else if(cmd->token[i][0] == '$')
		return 1; */
	/* BASTANTE POSIBLE que esto tenga errores en ciertos comandos con 2 argumentos sin '-' etc? */
	/* Esto checkea si el argumento anterior era comando y el actual es una letra, si es así es que es argumento
	con esto se solucionan los cannot access 'argumento' y que no se ejecute de nuevo command not found*/
	else if(i != 0 && ((cmd->token[i][0] >= 'A' && cmd->token[i][0] <= 'Z') \
	 || (cmd->token[i][0] >= 'a' && cmd->token[i][0] <= 'z'))\
	 && (is_command_exists(cmd, cmd->token[i - 1]) \
	 || is_redirects(cmd->token[i - 1][0]) || is_redirects_double_char(cmd->token[i - 1])))
	/* Realmente son solo los redirects */
		return 1;
	// TODO ESTO DE BUILTINS NO ES NECESARIO EN PRINCIPIO
	/* else if(cmd->token[i][0] == DOUBLE_QUOTE && cmd->token[i][ft_strlen(cmd->token[i]) - 1] == DOUBLE_QUOTE)
		return 1;
	else if(cmd->token[i][0] == SINGLE_QUOTE && cmd->token[i][ft_strlen(cmd->token[i]) - 1] == SINGLE_QUOTE)
		return 1; */
	/* else if(i >= 2 && ft_strcmp("echo", cmd->token[i - 2]) == 0 && ft_strcmp("-n", cmd->token[i - 1]) == 0)
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
		return 1; */
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
		if (cmd->line[i + len] == DOUBLE_QUOTE && cmd->line[i + len + 1] == ' ' && cmd->in_double_quote == true) // En el caso de estar dentro de comillas si el siguiente es un espacio directamente se vuelve false in_single_quote
																												 // Esto se hace para poder detectar luego si existe otra comilla y un espacio después
		{
			cmd->in_double_quote = false;
			i++;
		}
		if (cmd->line[i + len] == DOUBLE_QUOTE && cmd->line[i + len + 1] == ' ' && cmd->in_double_quote == false) // Se sale del bucle después de saber que es la última comilla y hay un espacio
			break ;
		i++;
	}
	return (i);
}

int is_single_quote(t_cmd *cmd, int len)
{
	int	i;

	cmd->in_single_quote = true;
	i = 1;
	while (cmd->line[i + len] != '\0')
	{
		if (cmd->line[i + len] == SINGLE_QUOTE && cmd->line[i + len + 1] == ' ' && cmd->in_single_quote == true) // En el caso de estar dentro de comillas si el siguiente es un espacio directamente se vuelve false in_single_quote
																												 // Esto se hace para poder detectar luego si existe otra comilla y un espacio después
		{
			cmd->in_single_quote = false;
			i++;
		}
		if (cmd->line[i + len] == SINGLE_QUOTE && cmd->line[i + len + 1] == ' ' && cmd->in_single_quote == false) // Se sale del bucle después de saber que es la última comilla y hay un espacio
			break ;
		i++;
	}
	return (i);
}

int	check_len_special(t_cmd *cmd, int len)
{
	int	i;
	int	inputs;
	int	outputs;

	i = 0;
	inputs = 0;
	outputs = 0;
	while(cmd->line[i + len] != ' ' && is_special(cmd->line[i + len])) /* cmd->line[i + len] != '\0' hay que meterlo?*/
		i++;
	while(cmd->line[inputs + len] != ' ' && cmd->line[inputs + len] == INPUT_REDIRECT)
		inputs++;
	while(cmd->line[outputs + len] != ' ' && cmd->line[outputs + len] == OUTPUT_REDIRECT)
		outputs++;
	// Gestión de errores similares a bash
	if(i > 2)
	{
		printf("-minishell: syntax error near unexpected token '");
		i = 0;
		while(i < 2) // No queremos que printee más de 2 '<<' o '>>'
		{
			printf("%c", cmd->line[i + len + 2]);
			i++;
		}
		printf("'\n");
		return (-1);
	}
	else if(i == 1)
		return(1);
	else if (i == 2)
		return(2);
	return (-1);
}

int cmd_token_len(t_cmd *cmd, int len)
{
	int i = 0;
	int	is_double_quote;
	int is_single_quote;

	is_double_quote = 0;
	is_single_quote = 0;
	while (cmd->line[i + len] != '\0')
	{
		if((cmd->line[i + len] == ' ' || is_special2(cmd->line[i + len])) && is_double_quote % 2 == 0 && is_single_quote % 2 == 0)
			break;
		else if(cmd->line[i + len] == DOUBLE_QUOTE)
			is_double_quote++;
		else if(cmd->line[i + len] == SINGLE_QUOTE)
			is_single_quote++;
 		i++;
	}
	return i;
}

int	check_len_token(t_cmd *cmd, int len)
{
	while(cmd->line[len] != '\0')
	{
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
