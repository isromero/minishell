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

#include "minishell.h"

void    init_pipes(t_cmd *cmd)
{
	int i;

	i = 0;
	while(i < cmd->n_pipes)
	{
		if (pipe(cmd->fd[i]) == -1)
		{
			perror("pipe");
			exit(1);
		}
		i++;
	}
}

void    wait_close_pipes(t_cmd *cmd)
{
	int i;
	int child_status;

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
		waitpid(cmd->pid[i], &child_status, 0); // Esperar a que el proceso hijo termine
		if (WIFEXITED(child_status) && WEXITSTATUS(child_status) >= 0) // Wexitstatus: Si el hijo terminó y cambió g_status
			g_status = WEXITSTATUS(child_status); // Obtener el estado de salida del proceso hijo
		i++;
	}
}

void	count_pipes(t_cmd *cmd)
{
	int i = 0;
	cmd->n_pipes = 0;
	cmd->n_processes = 0;
	while (cmd->token[i] != NULL)
	{
		if(cmd->token[i][0] == '|')
			cmd->n_pipes++;
		i++;
	}
	cmd->n_processes = cmd->n_pipes + 1;
}

int find_len_command_pipes(t_cmd *cmd, int i)
{
	if(i == 0)
		return (i);
	i--;
	while(i >= 0)
	{
		if(cmd->token[i][0] == '|')
			return (i + 1);
		i--;
	}
	return (0);
}
