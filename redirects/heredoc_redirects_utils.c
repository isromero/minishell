/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_redirects_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 18:49:58 by isromero          #+#    #+#             */
/*   Updated: 2024/03/29 20:01:40 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
