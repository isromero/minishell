/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 20:03:42 by isromero          #+#    #+#             */
/*   Updated: 2024/03/29 20:03:44 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	redirecting_pipes(t_cmd *cmd)
{
	int	i;
	int	first_time;

	i = 0;
	first_time = 0;
	signal(SIGINT, &handle_ctrlc2);
	signal(SIGQUIT, &handle_ctrl4);
	replace_before_execute(cmd);
	while (i < cmd->n_tokens - 1)
	{
		if (cmd->token[i][0] == '|' && cmd->count_pipes == 0 && first_time == 0)
		{
			first_time = 1;
			execute_first_pipes(cmd, 0);
		}
		else if (cmd->token[i][0] == '|' && (first_time == 1
				|| cmd->count_pipes > 0) && cmd->count_pipes < cmd->n_pipes)
			execute_middle_pipes(cmd, find_len_command_pipes(cmd, i));
		else if (!is_special(cmd->token[i][0]) && cmd->token[i + 1] == NULL
			&& (first_time == 1 || cmd->count_pipes > 0)
			&& cmd->count_pipes == cmd->n_pipes)
			execute_last_pipes(cmd, find_len_command_pipes(cmd, i));
		i++;
	}
	wait_close_pipes(cmd);
}

void	execute_last_pipes(t_cmd *cmd, int i)
{
	if (!is_argument_extension(cmd, i) && !is_redirects(cmd->token[i][0])
		&& !is_redirects_double_char(cmd->token[i]))
		execute_fork_pipes(cmd, i, 2);
}

void	execute_middle_pipes(t_cmd *cmd, int i)
{
	if (!is_argument_extension(cmd, i) && !is_redirects(cmd->token[i][0])
		&& !is_redirects_double_char(cmd->token[i]))
	{
		execute_fork_pipes(cmd, i, 1);
		cmd->count_pipes++;
		cmd->count_pids++;
	}
}

void	execute_first_pipes(t_cmd *cmd, int i)
{
	if (!is_argument_extension(cmd, i) && !is_redirects(cmd->token[i][0])
		&& !is_redirects_double_char(cmd->token[i]))
	{
		execute_fork_pipes(cmd, i, 0);
		cmd->count_pipes++;
		cmd->count_pids++;
	}
}
