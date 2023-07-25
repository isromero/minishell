/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_check3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 17:21:04 by isromero          #+#    #+#             */
/*   Updated: 2023/07/23 17:21:04 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

int	is_single_quote(t_cmd *cmd, int len)
{
	int	i;

	cmd->in_single_quote = true;
	i = 1;
	while (cmd->line[i + len] != '\0')
	{
		if (cmd->line[i + len] == SINGLE_QUOTE && cmd->line[i + len + 1] == ' '
			&& cmd->in_single_quote == true)
		{
			cmd->in_single_quote = false;
			i++;
		}
		if (cmd->line[i + len] == SINGLE_QUOTE && cmd->line[i + len + 1] == ' '
			&& cmd->in_single_quote == false)
			break ;
		i++;
	}
	return (i);
}

int	check_len_special(t_cmd *cmd, int len)
{
	int	i;

	i = 0;
	while (cmd->line[i + len] != ' ' && is_special(cmd->line[i + len]))
		i++;
	if (i > 2)
	{
		printf("-minishell: syntax error near unexpected token '");
		i = 0;
		while (i < 2)
		{
			printf("%c", cmd->line[i + len + 2]);
			i++;
		}
		printf("'\n");
		return (-1);
	}
	else if (i == 1)
		return (1);
	else if (i == 2)
		return (2);
	return (-1);
}
