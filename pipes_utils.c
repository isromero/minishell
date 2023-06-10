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
        wait(NULL);
        i++;
    }
    free(cmd->fd);
    free(cmd->pid);
}