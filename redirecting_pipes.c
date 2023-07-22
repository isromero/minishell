/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirecting_pipes.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/22 11:21:10 by isromero          #+#    #+#             */
/*   Updated: 2023/07/22 11:21:10 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redirect_first_pipe(t_cmd *cmd)
{
	close(cmd->fd[cmd->count_pipes][READ_END]);
	dup2(cmd->fd[cmd->count_pipes][WRITE_END], STDOUT_FILENO);
	close(cmd->fd[cmd->count_pipes][WRITE_END]);
}

void	redirect_middle_pipes(t_cmd *cmd)
{
	close(cmd->fd[cmd->count_pipes - 1][WRITE_END]);
	dup2(cmd->fd[cmd->count_pipes - 1][READ_END], STDIN_FILENO);
	close(cmd->fd[cmd->count_pipes - 1][READ_END]);
	close(cmd->fd[cmd->count_pipes][READ_END]);
	dup2(cmd->fd[cmd->count_pipes][WRITE_END], STDOUT_FILENO);
	close(cmd->fd[cmd->count_pipes][WRITE_END]);
}

void	redirect_last_pipe(t_cmd *cmd)
{
	close(cmd->fd[cmd->count_pipes - 1][WRITE_END]);
	dup2(cmd->fd[cmd->count_pipes - 1][READ_END], STDIN_FILENO);
	close(cmd->fd[cmd->count_pipes - 1][READ_END]);
}