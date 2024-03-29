/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirecting_pipes.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 20:03:27 by isromero          #+#    #+#             */
/*   Updated: 2024/03/29 20:03:28 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	redirect_first_pipe(t_cmd *cmd)
{
	int	i;

	i = 0;
	close(cmd->fd[cmd->count_pipes][READ_END]);
	dup2(cmd->fd[cmd->count_pipes][WRITE_END], STDOUT_FILENO);
	close(cmd->fd[cmd->count_pipes][WRITE_END]);
	while (i < cmd->n_pipes)
	{
		close(cmd->fd[i][READ_END]);
		close(cmd->fd[i][WRITE_END]);
		i++;
	}
}

void	redirect_middle_pipes(t_cmd *cmd)
{
	int	i;

	i = 0;
	close(cmd->fd[cmd->count_pipes - 1][WRITE_END]);
	dup2(cmd->fd[cmd->count_pipes - 1][READ_END], STDIN_FILENO);
	close(cmd->fd[cmd->count_pipes - 1][READ_END]);
	close(cmd->fd[cmd->count_pipes][READ_END]);
	dup2(cmd->fd[cmd->count_pipes][WRITE_END], STDOUT_FILENO);
	close(cmd->fd[cmd->count_pipes][WRITE_END]);
	while (i < cmd->n_pipes)
	{
		close(cmd->fd[i][READ_END]);
		close(cmd->fd[i][WRITE_END]);
		i++;
	}
}

void	redirect_last_pipe(t_cmd *cmd)
{
	int	i;

	i = 0;
	close(cmd->fd[cmd->count_pipes - 1][WRITE_END]);
	dup2(cmd->fd[cmd->count_pipes - 1][READ_END], STDIN_FILENO);
	close(cmd->fd[cmd->count_pipes - 1][READ_END]);
	while (i < cmd->n_pipes)
	{
		close(cmd->fd[i][READ_END]);
		close(cmd->fd[i][WRITE_END]);
		i++;
	}
}

void	which_pipe_redirect(t_cmd *cmd, int redirection_pipe)
{
	if (redirection_pipe == 0)
		redirect_first_pipe(cmd);
	else if (redirection_pipe == 1)
		redirect_middle_pipes(cmd);
	else if (redirection_pipe == 2)
		redirect_last_pipe(cmd);
}
