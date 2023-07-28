/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 17:45:54 by isromero          #+#    #+#             */
/*   Updated: 2023/05/26 17:45:54 by isromero         ###   ########.fr       */
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
				redirecting_pipes(cmd);
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
	while (i < cmd->n_tokens - 1)
	{
		if (cmd->no_expand_vars[i] == 0)
			replace_vars(cmd, &cmd->token[i]);
		i++;
	}
	i = 0;
	while (i < cmd->n_tokens - 1)
	{
		if (!is_argument_extension(cmd, i)
			&& !is_special(cmd->token[i][0])
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
