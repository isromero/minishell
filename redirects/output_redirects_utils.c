/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output_redirects_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 18:27:31 by isromero          #+#    #+#             */
/*   Updated: 2024/03/29 20:01:19 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	find_first_output_redirect(t_cmd *cmd)
{
	int	len;

	len = 0;
	while (cmd->token[len] != NULL)
	{
		if (cmd->token[len][0] == OUTPUT_REDIRECT)
			return (len);
		len++;
	}
	return (0);
}

int	find_last_output_redirect(t_cmd *cmd)
{
	int	len;

	len = cmd->n_tokens - 2;
	while (len >= 0)
	{
		if (cmd->token[len][0] == OUTPUT_REDIRECT)
			return (len);
		len--;
	}
	return (0);
}

void	close_output_redirect(t_cmd *cmd)
{
	dup2(cmd->stdout, STDOUT_FILENO);
	close(cmd->stdout);
}
