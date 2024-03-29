/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   append_redirects_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 10:08:54 by isromero          #+#    #+#             */
/*   Updated: 2024/03/29 20:01:47 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	find_first_append_redirect(t_cmd *cmd)
{
	int	len;

	len = 0;
	while (cmd->token[len] != NULL)
	{
		if (ft_strcmp(cmd->token[len], APPEND_REDIRECT) == 0)
			return (len);
		len++;
	}
	return (0);
}

int	find_last_append_redirect(t_cmd *cmd)
{
	int	len;

	len = cmd->n_tokens - 2;
	while (len >= 0)
	{
		if (ft_strcmp(cmd->token[len], APPEND_REDIRECT) == 0)
			return (len);
		len--;
	}
	return (0);
}
