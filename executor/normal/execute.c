/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 17:45:54 by isromero          #+#    #+#             */
/*   Updated: 2024/03/29 20:03:56 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	executor(t_cmd *cmd)
{
	int	i;

	i = 0;
	cmd->replaced_var = 0;
	while (cmd->token[i] != NULL)
	{
		if (cmd->token[i][0] == '|')
		{
			if (cmd->token[0][0] == '|')
				printf("minishell: syntax error near unexpected token `%s'\n",
					cmd->token[i]);
			else
			{
				init_pipes(cmd);
				redirecting_pipes(cmd);
			}
			return ;
		}
		i++;
	}
	execute(cmd);
}

void	execute(t_cmd *cmd)
{
	int	i;
	int	return_code;

	i = 0;
	return_code = 0;
	signal(SIGINT, &handle_ctrlc2);
	signal(SIGQUIT, &handle_ctrl4);
	replace_before_execute(cmd);
	while (i < cmd->n_tokens - 1)
	{
		if (!is_argument_extension(cmd, i) && !is_special(cmd->token[i][0])
			&& !is_redirects(cmd->token[i][0])
			&& !is_redirects_double_char(cmd->token[i]))
		{
			if (!is_builtin(cmd, i))
				return_code = execute_fork(cmd, i);
			else
				return_code = execute_builtin_no_child(cmd, i);
			if (return_code == 1)
				return ;
		}
		i++;
	}
}
