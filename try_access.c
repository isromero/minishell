/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   try_access.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 17:41:38 by isromero          #+#    #+#             */
/*   Updated: 2023/06/26 17:41:38 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    execute_executable(t_cmd *cmd, char *command)
{
    char *pwd;
    char *path;

    //CHECKEAR COÑO CAMBIAR -minishell: /mnt/c/Users/isromero/Documents/42cursus/minishell: command not found AL PONER $PWD
    if (command[0] == '/') /* ruta absoluta */
    {
        path = ft_strdup(command);
        try_execute(cmd, path);
    }
    else if(command[0] == '.') /* ruta no absoluta */
    {
        pwd = ft_getenv("PWD", cmd->env);
        path = ft_strjoin(pwd, command + 1);
        try_execute(cmd, path);
    }
}

void try_execute(t_cmd *cmd, char *path)
{
    if(access(path, F_OK) == 0)
        execve(path, cmd->token, cmd->env);
    else if(access(path, F_OK) == -1)
        printf("error:command not found");
}