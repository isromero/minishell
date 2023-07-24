/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_check2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 17:11:14 by isromero          #+#    #+#             */
/*   Updated: 2023/07/23 17:11:14 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_executable(t_cmd *cmd, char c)
{
	if (cmd->replaced_var == 1)
		return (0);
	return (c == '/' || c == '.');
}

int	is_argument_extension(t_cmd *cmd, int i)
{
		//grep $PATH???????????????????????????????????????
	if (cmd->token[i] == NULL)
		return (1);
	else if(cmd->token[i][0] == '-')
		return (1);
	else if (i != 0 && ((cmd->token[i][0] >= 'A' && cmd->token[i][0] <= 'Z') \
	 || (cmd->token[i][0] >= 'a' && cmd->token[i][0] <= 'z'))\
	 && (is_command_exists(cmd, cmd->token[i - 1]) \
	 || is_redirects(cmd->token[i - 1][0]) || is_redirects_double_char(cmd->token[i - 1])))
		return (1);
	return (0);
}

int	is_pipe(char c)
{
	return (c == PIPE);
}

int	is_special2(char c)
{
	return (c == INPUT_REDIRECT || c == PIPE || c == OUTPUT_REDIRECT);
}

int	is_double_quote(t_cmd *cmd, int len)
{
	int i;

	cmd->in_double_quote = true;
	i = 0;
	while (cmd->line[i + len] != '\0')
	{
		if (cmd->line[i + len] == DOUBLE_QUOTE && cmd->line[i + len + 1] == ' '
		&& cmd->in_double_quote == true) // En el caso de estar dentro de comillas si el siguiente es un espacio directamente se vuelve false in_single_quote
																												 // Esto se hace para poder detectar luego si existe otra comilla y un espacio después
		{
			cmd->in_double_quote = false;
			i++;
		}
		if (cmd->line[i + len] == DOUBLE_QUOTE && cmd->line[i + len + 1] == ' '
		&& cmd->in_double_quote == false) // Se sale del bucle después de saber que es la última comilla y hay un espacio
			break ;
		i++;
	}
	return (i);
}
