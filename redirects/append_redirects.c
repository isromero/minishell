/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output_redirects3.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 18:27:17 by isromero          #+#    #+#             */
/*   Updated: 2023/07/24 18:27:17 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	append_multiple_redirect(t_cmd *cmd)
{
	int	fd;
	int	len;
	int	i;

	i = 0;
	len = find_last_append_redirect(cmd);
	fd = open(cmd->token[len + 1], O_WRONLY
			| O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
	if (fd == -1)
	{
		perror("");
		return ;
	}
	while (i < len)
	{
		if (ft_strcmp(cmd->token[len], APPEND_REDIRECT) == 0)
			open(cmd->token[i + 1], O_CREAT
				| O_WRONLY | O_APPEND, S_IRUSR | S_IWUSR);
		i++;
	}
	cmd->stdout = dup(STDOUT_FILENO);
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

void	append_redirect(t_cmd *cmd)
{
	int	fd;
	int	len;

	len = find_first_append_redirect(cmd);
	fd = open(cmd->token[len + 1], O_WRONLY
			| O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
	if (fd == -1)
	{
		perror("");
		return ;
	}
	cmd->stdout = dup(STDOUT_FILENO);
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

int	is_append_redirect(t_cmd *cmd, int len)
{
	int	n_redirects;

	n_redirects = 0;
	while (cmd->token[len] != NULL)
	{
		if (ft_strcmp(cmd->token[len], APPEND_REDIRECT) == 0)
			n_redirects++;
		len++;
	}
	if (n_redirects == 1)
		return (1);
	else if (n_redirects > 1)
		return (n_redirects);
	return (0);
}

void	close_output_redirect(t_cmd *cmd)
{
	dup2(cmd->stdout, STDOUT_FILENO);
	close(cmd->stdout);
}
