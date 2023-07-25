/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_redirects.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adgutier <adgutier@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 19:37:01 by adgutier          #+#    #+#             */
/*   Updated: 2023/07/25 11:41:04 by adgutier         ###   ########.fr       */
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

void	input_redirect(t_cmd *cmd)
{
	int	fd;
	int	len;

	len = find_first_input_redirect(cmd);
	fd = open(cmd->token[len + 1], O_RDONLY | S_IRUSR | S_IWUSR);
	if (fd == -1)
	{
		perror("");
		return ;
	}
	cmd->stdin = dup(STDIN_FILENO);
	dup2(fd, STDIN_FILENO);
	close(fd);
}

void	input_multiple_redirect(t_cmd *cmd)
{
	int	fd;
	int	len;
	int	i;

	i = 0;
	len = find_last_input_redirect(cmd);
	fd = open(cmd->token[len + 1], O_RDONLY | S_IRUSR | S_IWUSR);
	if (fd == -1)
	{
		perror("");
		return ;
	}
	while (i < len)
	{
		if (cmd->token[i][0] == INPUT_REDIRECT)
			open(cmd->token[i + 1], O_RDONLY | S_IRUSR | S_IWUSR);
		i++;
	}
	cmd->stdin = dup(STDIN_FILENO);
	dup2(fd, STDIN_FILENO);
	close(fd);
}
