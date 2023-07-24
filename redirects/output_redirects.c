/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 21:11:23 by isromero          #+#    #+#             */
/*   Updated: 2023/06/04 21:11:23 by isromero         ###   ########.fr       */
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

int	is_output_redirect(t_cmd *cmd, int len)
{
	int	n_redirects;

	n_redirects = 0;
	while (cmd->token[len] != NULL)
	{
		if (cmd->token[len][0] == OUTPUT_REDIRECT)
			n_redirects++;
		len++;
	}
	if (n_redirects == 1)
		return (1);
	else if (n_redirects > 1)
		return (n_redirects);
	return (0);
}
