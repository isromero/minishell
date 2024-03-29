/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 17:57:43 by isromero          #+#    #+#             */
/*   Updated: 2024/03/29 20:03:49 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*command_dir_exists(char *dir, char *path, char *command)
{
	size_t	dir_len;
	char	executable_path[PATH_MAX];
	char	*result;

	dir_len = ft_strlen(dir);
	ft_strcpy(executable_path, dir);
	executable_path[dir_len] = '/';
	ft_strcpy(executable_path + dir_len + 1, command);
	if (access(executable_path, F_OK) == 0)
	{
		result = (char *)malloc((dir_len + ft_strlen(command) + 2)
				* sizeof(char));
		if (!result)
			return (NULL);
		ft_strcpy(result, dir);
		ft_strcat(result, "/");
		ft_strcat(result, command);
		free(path);
		return (result);
	}
	return (NULL);
}

char	*command_dir(t_cmd *cmd, char *command)
{
	char	*path;
	char	*dir;
	char	*result;

	path = ft_getenv("PATH", cmd->env);
	dir = ft_strtok(path, ":");
	if (!is_executable(cmd, command[0]))
	{
		while (dir != NULL)
		{
			result = command_dir_exists(dir, path, command);
			if (result != NULL)
				return (result);
			dir = ft_strtok(NULL, ":");
		}
		free(path);
		return (NULL);
	}
	return (0);
}

char	*build_command_path(const char *base_path, const char *command)
{
	size_t	base_len;
	size_t	command_len;
	size_t	total_len;
	char	*command_path;

	command_len = ft_strlen(command);
	base_len = ft_strlen(base_path);
	total_len = base_len + command_len + 2;
	command_path = (char *)malloc(total_len * sizeof(char));
	ft_strncpy(command_path, base_path, base_len);
	command_path[base_len] = '/';
	ft_strncpy(command_path + base_len + 1, command, command_len);
	command_path[total_len - 1] = '\0';
	return (command_path);
}

int	path_is_command_exists(char *path_copy, char *command)
{
	char	*command_path;
	char	*token;

	token = ft_strtok(path_copy, ":");
	while (token != NULL)
	{
		command_path = build_command_path(token, command);
		if (access(command_path, F_OK) == 0)
		{
			free(path_copy);
			free(command_path);
			return (1);
		}
		free(command_path);
		token = ft_strtok(NULL, ":");
	}
	return (0);
}

int	is_command_exists(t_cmd *cmd, char *command)
{
	char	*path;
	char	*path_copy;

	if (access(command, F_OK) == 0)
		return (1);
	path = ft_getenv("PATH", cmd->env);
	if (path != NULL)
	{
		path_copy = ft_strdup(path);
		free(path);
		if (path_is_command_exists(path_copy, command) == 1)
			return (1);
		free(path_copy);
	}
	return (0);
}
