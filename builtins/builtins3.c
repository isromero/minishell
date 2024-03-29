/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 21:59:54 by isromero          #+#    #+#             */
/*   Updated: 2024/03/29 20:04:19 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_exit(t_cmd *cmd, int exit_token)
{
	int	exit_code;

	if (is_number(cmd->token[exit_token + 1]) && ft_atoi(cmd->token
			[exit_token + 1]) >= INT_MIN
		&& ft_atoi(cmd->token[exit_token + 1]) <= INT_MAX)
	{
		exit_code = ft_atoi(cmd->token[exit_token + 1]);
		if (exit_code >= 0 && exit_code <= 255)
			execute_builtin_exit(cmd, exit_code);
		else if (exit_code < 0 || exit_code > 255)
		{
			exit_code = (exit_code % 256);
			execute_builtin_exit(cmd, exit_code);
		}
	}
	else
		execute_builtin_exit(cmd, 1);
}

void	execute_builtin_exit(t_cmd *cmd, int exit_code)
{
	clean_tokens(cmd);
	free(cmd->line);
	free(cmd->prompt);
	free_matrix(cmd->env);
	free(cmd->no_expand_vars);
	exit(exit_code);
}
