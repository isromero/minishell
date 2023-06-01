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

void    init_pipes(t_cmd *cmd, int fd[cmd->n_pipes][2])
{
    int i;

    i = 0;
    while(i <= cmd->n_pipes - 1)
    {
        if (pipe(fd[i]) == -1)
        {
            perror("pipe");
            exit(1);
        }
        i++;
    }
}

void    wait_close_pipes(t_cmd *cmd, int fd[cmd->n_pipes][2])
{
    int i;

    i = 0;
    while (i < cmd->n_pipes)
    {
        close(fd[i][READ_END]);
        close(fd[i][WRITE_END]);
        i++;
    }
    i = 0;
    while (i < cmd->n_processes)
    {
        wait(NULL);
        i++;
    }
}