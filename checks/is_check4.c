/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_check4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 10:03:45 by isromero          #+#    #+#             */
/*   Updated: 2023/07/26 10:03:45 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_input_redirect(t_cmd *cmd, int len)
{
	int	n_redirects;

	n_redirects = 0;
	while (cmd->token[len] != NULL)
	{
		if (cmd->token[len][0] == INPUT_REDIRECT)
			n_redirects++;
		len++;
	}
	if (n_redirects == 1)
		return (1);
	else if (n_redirects > 1)
		return (n_redirects);
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
