/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_redirects_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 10:10:11 by isromero          #+#    #+#             */
/*   Updated: 2024/03/29 20:01:35 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	find_first_input_redirect(t_cmd *cmd)
{
	int	len;

	len = 0;
	while (cmd->token[len] != NULL)
	{
		if (cmd->token[len][0] == INPUT_REDIRECT)
			return (len);
		len++;
	}
	return (0);
}

int	find_last_input_redirect(t_cmd *cmd)
{
	int	len;

	len = cmd->n_tokens - 2;
	while (len >= 0)
	{
		if (cmd->token[len][0] == INPUT_REDIRECT)
			return (len);
		len--;
	}
	return (0);
}

void	close_input_redirect(t_cmd *cmd)
{
	if (dup2(cmd->stdin, STDIN_FILENO) == -1)
	{
		perror("Failed to restore stdin");
		exit(1);
	}
	close(cmd->stdin);
}
