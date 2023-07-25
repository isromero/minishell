/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 20:54:01 by isromero          #+#    #+#             */
/*   Updated: 2023/06/01 20:54:01 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	init_pipes(t_cmd *cmd)
{
	int	i;
	int	j;

	cmd->count_pipes = 0;
	cmd->count_pids = 0;
	cmd->pid = (int *)malloc(sizeof(int) * cmd->n_processes);
	cmd->fd = (int **)malloc(sizeof(int *) * cmd->n_pipes);
	j = 0;
	while (j < cmd->n_pipes)
		cmd->fd[j++] = (int *)malloc(sizeof(int) * 2);
	i = 0;
	while (i < cmd->n_pipes)
	{
		if (pipe(cmd->fd[i]) == -1)
		{
			perror("pipe");
			exit(1);
		}
		i++;
	}
}

void	wait_close_pipes(t_cmd *cmd)
{
	int	i;
	int	child_status;

	i = 0;
	while (i < cmd->n_pipes)
	{
		close(cmd->fd[i][READ_END]);
		close(cmd->fd[i][WRITE_END]);
		i++;
	}
	i = 0;
	while (i < cmd->n_processes)
	{
		waitpid(cmd->pid[i], &child_status, 0);
		if (WIFEXITED(child_status) && WEXITSTATUS(child_status) >= 0)
			g_status = WEXITSTATUS(child_status);
		i++;
	}
}

void	count_pipes(t_cmd *cmd)
{
	int	i;

	i = 0;
	cmd->n_pipes = 0;
	cmd->n_processes = 0;
	while (cmd->token[i] != NULL)
	{
		if (cmd->token[i][0] == '|')
			cmd->n_pipes++;
		i++;
	}
	cmd->n_processes = cmd->n_pipes + 1;
}

int	find_len_command_pipes(t_cmd *cmd, int i)
{
	if (i == 0)
		return (i);
	i--;
	while (i >= 0)
	{
		if (cmd->token[i][0] == '|')
			return (i + 1);
		i--;
	}
	return (0);
}

void	free_pipes(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (i < cmd->n_pipes)
	{
		free(cmd->fd[i]);
		i++;
	}
	free(cmd->fd);
	free(cmd->pid);
}
