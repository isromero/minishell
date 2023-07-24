/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_check3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 17:21:04 by isromero          #+#    #+#             */
/*   Updated: 2023/07/23 17:21:04 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int is_single_quote(t_cmd *cmd, int len)
{
	int	i;

	cmd->in_single_quote = true;
	i = 1;
	while (cmd->line[i + len] != '\0')
	{
		if (cmd->line[i + len] == SINGLE_QUOTE && cmd->line[i + len + 1] == ' '
		&& cmd->in_single_quote == true) // En el caso de estar dentro de comillas si el siguiente es un espacio directamente se vuelve false in_single_quote
																												 // Esto se hace para poder detectar luego si existe otra comilla y un espacio después
		{
			cmd->in_single_quote = false;
			i++;
		}
		if (cmd->line[i + len] == SINGLE_QUOTE && cmd->line[i + len + 1] == ' '
		&& cmd->in_single_quote == false) // Se sale del bucle después de saber que es la última comilla y hay un espacio
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
	if (i > 2)
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
	else if (i == 1)
		return (1);
	else if (i == 2)
		return (2);
	return (-1);
}
