/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_pipes_processes.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 18:46:22 by isromero          #+#    #+#             */
/*   Updated: 2023/07/23 18:46:22 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	child_pipes_process(t_cmd *cmd, char *com, char **args, int i)
{
	if (is_builtin(cmd, i))
		execute_builtin_in_child(cmd, i);
	else if (!is_builtin(cmd, i))
	{
		if (com != NULL || is_executable(cmd, cmd->token[i][0]))
			execute_command_exists(cmd, com, args, i);
		else if (!com && !is_executable(cmd, cmd->token[i][0]))
			execute_command_no_exists(cmd, i);
	}
	else
		exit(0);
}

void	execute_fork_pipes(t_cmd *cmd, int i, int redirection_pipe)
{
	char	**exec_args;
	char	*com;

	com = NULL;
	exec_args = get_exec_args(cmd, i);
	if (!is_executable(cmd, cmd->token[i][0]))
		com = command_dir(cmd, cmd->token[i]);
	cmd->pid[cmd->count_pids] = fork();
	if (cmd->pid[cmd->count_pids] == -1)
	{
		perror("fork");
		exit(1);
	}
	else if (cmd->pid[cmd->count_pids] == 0)
    {
        if (redirection_pipe == 0)
            redirect_first_pipe(cmd);
        else if (redirection_pipe == 1)
            redirect_middle_pipes(cmd);
        else if (redirection_pipe == 2)
            redirect_last_pipe(cmd);
		child_pipes_process(cmd, com, exec_args, i);
     }
	else
	{
		free(exec_args);
		free(com);
	}
}
