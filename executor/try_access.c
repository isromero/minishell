/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   try_access.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 17:41:38 by isromero          #+#    #+#             */
/*   Updated: 2024/03/29 20:03:21 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execute_executable(t_cmd *cmd, char *command)
{
	char	*pwd;
	char	*path;

	if (command[0] == '/')
	{
		path = ft_strdup(command);
		try_execute(cmd, path, command);
		free(path);
	}
	else if (command[0] == '.')
	{
		pwd = ft_getenv("PWD", cmd->env);
		path = ft_strjoin(pwd, command + 1);
		try_execute(cmd, path, command);
		free(pwd);
		free(path);
	}
}

void	try_execute(t_cmd *cmd, char *path, char *command)
{
	if (access(path, F_OK) == -1)
	{
		printf("-minishell: %s: No such file or directory\n", command);
		g_status = 127;
		exit(g_status);
	}
	else if (access(path, X_OK) == -1)
	{
		printf("-minishell: %s: Permission denied\n", command);
		g_status = 126;
		exit(g_status);
	}
	else if (access(path, F_OK) == 0)
	{
		if (execve(path, cmd->token, cmd->env) == -1)
		{
			printf("-minishell: %s: Is a directory\n", command);
			g_status = 126;
			exit(g_status);
		}
	}
}
