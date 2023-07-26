/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_redirects_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 18:49:58 by isromero          #+#    #+#             */
/*   Updated: 2023/07/24 18:49:58 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	find_first_heredoc_redirect(t_cmd *cmd)
{
	int	len;

	len = 0;
	while (cmd->token[len] != NULL)
	{
		if (ft_strcmp(cmd->token[len], HEREDOC_REDIRECT) == 0)
			return (len);
		len++;
	}
	return (0);
}

int	find_last_heredoc_redirect(t_cmd *cmd)
{
	int	len;

	len = cmd->n_tokens - 2;
	while (len >= 0)
	{
		if (ft_strcmp(cmd->token[len], HEREDOC_REDIRECT) == 0)
			return (len);
		len--;
	}
	return (0);
}

char	*find_heredoc_delim(t_cmd *cmd)
{
	int		len;
	char	*delim;

	delim = NULL;
	len = cmd->n_tokens - 2;
	while (len >= 0)
	{
		if (ft_strcmp(cmd->token[len], HEREDOC_REDIRECT) == 0)
		{
			delim = cmd->token[len + 1];
			return (delim);
		}
		len--;
	}
	return (0);
}
