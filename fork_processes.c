/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_processes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/22 11:15:32 by isromero          #+#    #+#             */
/*   Updated: 2023/07/22 11:15:32 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    parent_process(char *com, char **exec_args)
{
    free(com);
    free(exec_args);
    int child_status;
    wait(&child_status);
    if (WIFEXITED(child_status) && WEXITSTATUS(child_status) >= 0)
    {
        g_status = WEXITSTATUS(child_status);
        return ;
    }
}

void    child_process(t_cmd *cmd, char *com, char **exec_args, int i)
{
    if(!is_executable(cmd, cmd->token[i][0])) // Soluciona un leak a la hora de pasarle una ruta absoluta que es un directorio
        com = command_dir(cmd, cmd->token[i]);
    if (com != NULL || is_executable(cmd, cmd->token[i][0]))
        execute_command_exists(cmd, com, exec_args, i);
    else if (!com && !is_executable(cmd, cmd->token[i][0]))
        execute_command_no_exists(cmd, i);
    else // No necesario, pero por si acaso
        exit(0);
}

void    execute_fork(t_cmd *cmd, int i)
{
    char **exec_args;
    char *com;

    exec_args = get_exec_args(cmd, i);
    com = NULL;
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit(1);
    }
    else if (pid == 0)
        child_process(cmd, com, exec_args, i);
    else
        parent_process(com, exec_args);
}