/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 17:20:04 by isromero          #+#    #+#             */
/*   Updated: 2023/05/18 17:20:04 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parse_args(t_cmd *cmd)
{
	int	i;

	i = 0;
	//Se necesitan más funcionalidades, a parte de dividir en tokens
	cmd->token = ft_split(cmd->line, ' ');
	while(cmd->token[i])
		i++;
	cmd->n_tokens = i;
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

int	main(int argc, char **argv)
{
	t_cmd	cmd;
	
	
	while(1)
	{
		cmd.prompt = get_prompt(&cmd);
		cmd.line = readline(cmd.prompt);
		parse_args(&cmd);
		free(cmd.line);
		free(cmd.prompt);
	}
    return (0);
}