/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   estructura.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 17:23:46 by isromero          #+#    #+#             */
/*   Updated: 2023/05/21 17:23:46 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_special(char c)
{
	return(c == INPUT_REDIRECT || c == PIPE || c == OUTPUT_REDIRECT \
	|| c == VARIABLE);
}

int	is_special2(char c)
{
	return(c == INPUT_REDIRECT || c == PIPE || c == OUTPUT_REDIRECT);
}

int	is_variable(t_cmd *cmd)
{
	while(cmd->line[cmd->n_line] != ft_isspace(cmd->line[cmd->n_line]) && cmd->line[cmd->n_line] != is_special(cmd->line[cmd->n_line]))
		cmd->n_line++;
	return (cmd->n_line);
}

int is_double_quote(t_cmd *cmd)
{
	if(!ft_strchr(cmd->line, DOUBLE_QUOTE))
		return (-1);
	while(cmd->line[cmd->n_line] != '\0' && cmd->line[cmd->n_line] != DOUBLE_QUOTE)
		cmd->n_line++;
	cmd->n_line++;
	return (cmd->n_line);
}

int is_single_quote(t_cmd *cmd)
{
	if(!ft_strchr(cmd->line, SINGLE_QUOTE))
		return (-1);
	while(cmd->line[cmd->n_line] != '\0' && cmd->line[cmd->n_line] != SINGLE_QUOTE)
		cmd->n_line++;
	cmd->n_line++;
	return (cmd->n_line);
}

void error_special()
{
	printf("syntax error near unexpected token\n");
}

int check_len_special(t_cmd *cmd)
{
	while(!ft_isspace(cmd->line[cmd->n_line]) && is_special(cmd->line[cmd->n_line]))
		cmd->n_line++;
	//Gestión de errores
	// if(cmd->n_line > 2)
	// 	error_special();
	// else if(cmd->n_line == 1)
	// 	return(1);
	// else if (cmd->n_line == 2)
	// 	return(2); 
	// Arreglar esto, hay que guardar la len antigua, no el total
	return (-1);
}

int token_len(t_cmd *cmd)
{
	while(!ft_isspace(cmd->line[cmd->n_line]) && !is_special((cmd->line[cmd->n_line])))
		cmd->n_line++;
	return (cmd->n_line);
}

int	check_len_token(t_cmd *cmd)
{
	while(cmd->line[cmd->n_line] != '\0')
	{
		while(cmd->line[cmd->n_line] != '\0' && !ft_isspace(cmd->line[cmd->n_line]))
			cmd->n_line++;
		if(cmd->line[cmd->n_line] == VARIABLE)
			return(is_variable(cmd));
		else if(cmd->line[cmd->n_line] == SINGLE_QUOTE)
			return(is_single_quote(cmd));
		else if(cmd->line[cmd->n_line] == DOUBLE_QUOTE)
			return(is_double_quote(cmd));
		else if(is_special2(cmd->line[cmd->n_line])) // < > | 
			return(check_len_special(cmd)); // << < > >> |
		else if(cmd->line[cmd->n_line] != '\0')
			return(token_len(cmd));
	}
	return (cmd->n_line);
}

void **save_token(t_cmd *cmd, int len)
{
	int	i;
	int j;

	i = 0;
	j = 0;
	size_t total_size = (cmd->n_tokens + 1) * sizeof(char *) + len * sizeof(char);
	cmd->token = (char **)malloc(total_size);
	cmd->token[cmd->n_tokens] = (char *)(cmd->token + cmd->n_tokens + 1);
	if (cmd->token == NULL) 
	{
		// Manejo del error, por ejemplo, imprimir un mensaje de error y salir de la función
		printf("Error: No se pudo asignar memoria para cmd->token\n");
		return (0);
	}
	while(i < len)
	{
		cmd->token[cmd->n_tokens][i] = cmd->line[i];
		i++;
	}
	cmd->n_tokens++;
	return(0);
}

void print_tokens(t_cmd *cmd)
{
    int i;
    
    for (i = 0; i < cmd->n_tokens; i++) {
        printf("Token %d: %s\n", i, cmd->token[i]);
    }
}

void	parse_args(t_cmd *cmd)
{
	int len;
	int	i;

	i = 0;
	cmd->n_tokens = 0;
	cmd->token = NULL;
	cmd->n_line = 0;
	
	//existe token?
	//si? SI! me lo jalas al string
	while(cmd->line)
	{
		len = check_len_token(cmd);
		if(len)
		{
			save_token(cmd, len);
			print_tokens(cmd);
			// printf("%s\n", cmd->token[0]);
		}
		else
			printf("introducir error\n");

	}
	//no? NO! Gestión de errores y free pa casa
}

char *get_prompt(t_cmd *custom_prompt)
{
    char *username = getenv("USER"); // Obtener el nombre de usuario desde la variable de entorno USER
    char cwd[1024]; 
    getcwd(cwd, sizeof(cwd)); // Obtener la ubicación actual de la terminal
    
    size_t prompt_length = ft_strlen(username) + ft_strlen(cwd) + 13 + ft_strlen(COLOR_GREEN) \
	+ ft_strlen(COLOR_MAGENTA) + ft_strlen(COLOR_YELLOW) + ft_strlen(COLOR_CYAN) + ft_strlen(COLOR_WHITE); // Longitud total del prompt personalizado
    custom_prompt->prompt = (char *)malloc(prompt_length * sizeof(char));

	strcpy(custom_prompt->prompt, COLOR_GREEN);
    strcat(custom_prompt->prompt, username);
    strcat(custom_prompt->prompt, COLOR_MAGENTA);
    strcat(custom_prompt->prompt, "@minishell");
	strcat(custom_prompt->prompt, COLOR_YELLOW);
	strcat(custom_prompt->prompt, ":");
    strcat(custom_prompt->prompt, COLOR_CYAN);
    strcat(custom_prompt->prompt, cwd);
    strcat(custom_prompt->prompt, COLOR_WHITE);
    strcat(custom_prompt->prompt, "$ ");
    
    return (custom_prompt->prompt);
}

void	print_minishell()
{
	printf("\n▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄");
	printf("\n▐   ▄▄       ▄▄  ▄▄▄▄▄▄▄▄▄▄▄  ▄▄        ▄  ▄▄▄▄▄▄▄▄▄▄▄  ▄▄▄▄▄▄▄▄▄▄▄  ▄         ▄  ▄▄▄▄▄▄▄▄▄▄▄  ▄            ▄           ▐");
	printf("\n▐  ▐░░▌     ▐░░▌▐░░░░░░░░░░░▌▐░░▌      ▐░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░▌       ▐░▌▐░░░░░░░░░░░▌▐░▌          ▐░▌          ▐");
	printf("\n▐  ▐░▌░▌   ▐░▐░▌ ▀▀▀▀█░█▀▀▀▀ ▐░▌░▌     ▐░▌ ▀▀▀▀█░█▀▀▀▀ ▐░█▀▀▀▀▀▀▀▀▀ ▐░▌       ▐░▌▐░█▀▀▀▀▀▀▀▀▀ ▐░▌          ▐░▌          ▐");
	printf("\n▐  ▐░▌▐░▌ ▐░▌▐░▌     ▐░▌     ▐░▌▐░▌    ▐░▌     ▐░▌     ▐░▌          ▐░▌       ▐░▌▐░▌          ▐░▌          ▐░▌          ▐");
	printf("\n▐  ▐░▌ ▐░▐░▌ ▐░▌     ▐░▌     ▐░▌ ▐░▌   ▐░▌     ▐░▌     ▐░█▄▄▄▄▄▄▄▄▄ ▐░█▄▄▄▄▄▄▄█░▌▐░█▄▄▄▄▄▄▄▄▄ ▐░▌          ▐░▌          ▐");
	printf("\n▐  ▐░▌  ▐░▌  ▐░▌     ▐░▌     ▐░▌  ▐░▌  ▐░▌     ▐░▌     ▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░▌          ▐░▌          ▐");
	printf("\n▐  ▐░▌   ▀   ▐░▌     ▐░▌     ▐░▌   ▐░▌ ▐░▌     ▐░▌      ▀▀▀▀▀▀▀▀▀█░▌▐░█▀▀▀▀▀▀▀█░▌▐░█▀▀▀▀▀▀▀▀▀ ▐░▌          ▐░▌          ▐");
	printf("\n▐  ▐░▌       ▐░▌     ▐░▌     ▐░▌    ▐░▌▐░▌     ▐░▌               ▐░▌▐░▌       ▐░▌▐░▌          ▐░▌          ▐░▌          ▐");
	printf("\n▐  ▐░▌       ▐░▌ ▄▄▄▄█░█▄▄▄▄ ▐░▌     ▐░▐░▌ ▄▄▄▄█░█▄▄▄▄  ▄▄▄▄▄▄▄▄▄█░▌▐░▌       ▐░▌▐░█▄▄▄▄▄▄▄▄▄ ▐░█▄▄▄▄▄▄▄▄▄ ▐░█▄▄▄▄▄▄▄▄▄ ▐");
	printf("\n▐  ▐░▌       ▐░▌▐░░░░░░░░░░░▌▐░▌      ▐░░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░▌       ▐░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐");
	printf("\n▐   ▀         ▀  ▀▀▀▀▀▀▀▀▀▀▀  ▀        ▀▀  ▀▀▀▀▀▀▀▀▀▀▀  ▀▀▀▀▀▀▀▀▀▀▀  ▀         ▀  ▀▀▀▀▀▀▀▀▀▀▀  ▀▀▀▀▀▀▀▀▀▀▀  ▀▀▀▀▀▀▀▀▀▀▀ ▐");
	printf("\n▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄\n");
	printf("\n");
}

int	main(int argc, char **argv, char **env)
{
	t_cmd	cmd;
	int		i;
	
	i = 0;
	//print_minishell();
	while(1)
	{
		cmd.prompt = get_prompt(&cmd);
		cmd.line = readline(cmd.prompt);
		if(ft_strncmp(cmd.line, "", 1) > 0) 
			add_history(cmd.line);
		parse_args(&cmd);
		free(cmd.line);
		free(cmd.prompt);
	}
	rl_clear_history();
    return (0);
}