/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_check2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 17:11:14 by isromero          #+#    #+#             */
/*   Updated: 2023/07/23 17:11:14 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_executable(t_cmd *cmd, char c)
{
	(void)cmd;
	return (c == '/' || c == '.');
}

int	is_argument_extension(t_cmd *cmd, int i)
{
	if (cmd->token[i] == NULL)
		return (1);
	else if (cmd->token[i][0] == '-')
		return (1);
	else if (i != 0 && ((cmd->token[i][0] >= 'A' && cmd->token[i][0] <= 'Z')
		|| (cmd->token[i][0] >= 'a' && cmd->token[i][0] <= 'z'))
		&& (is_command_exists(cmd, cmd->token[i - 1])
		|| is_redirects(cmd->token[i - 1][0])
		|| is_redirects_double_char(cmd->token[i - 1])))
		return (1);
	return (0);
}

int	is_pipe(char c)
{
	return (c == PIPE);
}

int	is_builtin(t_cmd *cmd, int n_token)
{
	if (ft_strcmp(cmd->token[n_token], "echo") == 0)
		return (1);
	else if (ft_strcmp(cmd->token[n_token], "cd") == 0)
		return (1);
	else if (ft_strcmp(cmd->token[n_token], "pwd") == 0)
		return (1);
	else if (ft_strcmp(cmd->token[n_token], "export") == 0)
		return (1);
	else if (ft_strcmp(cmd->token[n_token], "unset") == 0)
		return (1);
	else if (ft_strcmp(cmd->token[n_token], "env") == 0)
		return (1);
	else if (ft_strcmp(cmd->token[n_token], "exit") == 0)
		return (1);
	return (0);
}
