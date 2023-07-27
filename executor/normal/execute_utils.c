/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/22 10:55:55 by isromero          #+#    #+#             */
/*   Updated: 2023/07/22 10:55:55 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	**get_exec_args(t_cmd *cmd, int i)
{
	char	**exec_args;
	int		j;

	j = i;
	exec_args = (char **)malloc(sizeof(char *) * (cmd->n_tokens - i + 1));
	if (!exec_args)
		return (NULL);
	while (j < cmd->n_tokens - 1 && !is_special(cmd->token[j][0])
		&& !is_redirects(cmd->token[j][0]))
	{
		exec_args[j - i] = cmd->token[j];
		j++;
	}
	exec_args[j - i] = NULL;
	return (exec_args);
}

void	execute_command_exists(t_cmd *cmd, char *com, char **exec_args, int i)
{
	if (!is_output_redirect(cmd, i) && !is_input_redirect(cmd, i) \
		&& !is_append_redirect(cmd, i) && !is_heredoc_redirect(cmd, i)
		&& !is_executable(cmd, cmd->token[i][0]))
	{
		if (execve(com, exec_args, cmd->env) == -1)
		{
			g_status = 2;
			exit(g_status);
		}
	}
	execute_executable(cmd, cmd->token[i]);
	execute_redirects(cmd, com, exec_args, i);
	exit(0);
}

void	execute_command_no_exists(t_cmd *cmd, int i)
{
	DIR	*dir;

	g_status = 127;
	dir = opendir(cmd->token[i]);
	if (dir)
	{
		printf("-minishell: %s: Is a directory\n", cmd->token[i]);
		closedir(dir);
	}
	else if (cmd->token[i][0] == '/')
    {
        printf("-minishell: %s: No such file or directory\n", cmd->token[i]);
        g_status = 127;
    }
	else
		printf("-minishell: %s: command not found\n", cmd->token[i]);
	exit(g_status);
}

int	execute_builtin_no_child(t_cmd	*cmd, int i)
{
	if (!is_output_redirect(cmd, i) && !is_input_redirect(cmd, i) \
		&& !is_append_redirect(cmd, i) && !is_heredoc_redirect(cmd, i))
	{
		execute_builtin(cmd, i);
		g_status = 0;
		return (1);
	}
	execute_redirects(cmd, NULL, NULL, i);
	g_status = 0;
	return (1);
}

void	execute_builtin_in_child(t_cmd *cmd, int i)
{
	if (!is_output_redirect(cmd, i) && !is_input_redirect(cmd, i) \
	&& !is_append_redirect(cmd, i) && !is_heredoc_redirect(cmd, i))
	{
		execute_builtin(cmd, i);
		g_status = 0;
		exit(g_status);
	}
	execute_appends(cmd, NULL, NULL, i);
	execute_output_redirects(cmd, NULL, NULL, i);
	execute_heredoc_redirects(cmd, NULL, NULL, i);
	execute_input_redirects(cmd, NULL, NULL, i);
	g_status = 0;
	exit(g_status);
}
