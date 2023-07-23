/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 21:59:54 by isromero          #+#    #+#             */
/*   Updated: 2023/07/23 21:59:54 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void execute_builtin_exit(t_cmd *cmd, int exit_code)
{
	clean_tokens(cmd);
	free(cmd->line);
	free(cmd->prompt);
	free_matrix(cmd->env);
	free(cmd->no_expand_vars);
	exit(exit_code);
}

void	ft_exit(t_cmd *cmd, int exit_token) // Con textos de argumentos creo que hay que devolver 2
{
	int	exit_code;

	if(is_number(cmd->token[exit_token + 1]) && ft_atoi(cmd->token[exit_token + 1]) >= INT_MIN && ft_atoi(cmd->token[exit_token + 1]) <= INT_MAX) // Si es un número cualquiera entonces es que hay un argumento número y:
	{
		exit_code = ft_atoi(cmd->token[exit_token + 1]);
		if(exit_code >= 0 && exit_code <= 255) // Habría que checkear si le pasas exit "numero random" debe devolver ese numero random o 1 predeterminado PENDIENTE
			execute_builtin_exit(cmd, exit_code);
		else if(exit_code < 0 || exit_code > 255) // Esto significa que está fuera de los límites del exit, y bash hace el % del valor pasado para hacer que esté en el rango válido
		{
			exit_code = (exit_code % 256);
			execute_builtin_exit(cmd, exit_code);
		}
	}
	else
		execute_builtin_exit(cmd, 1); // En el caso de que no sea ningun número ni nada devolvemos un error predeterminado, el 1
}
