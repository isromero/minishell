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

void	output_redirect(t_cmd *cmd)
{
	int	fd;
	int	len;

	len = find_first_output_redirect(cmd);
	fd = open(cmd->token[len + 1], O_WRONLY
			| O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	if (fd == -1)
	{
		perror("");
		return ;
	}
	cmd->stdout = dup(STDOUT_FILENO);
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

void	output_multiple_redirect(t_cmd *cmd)
{
	int	fd;
	int	len;
	int	i;

	i = 0;
	len = find_last_output_redirect(cmd);
	fd = open(cmd->token[len + 1], O_WRONLY
			| O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	if (fd == -1)
	{
		perror("");
		return ;
	}
	while (i < len)
	{
		if (cmd->token[i][0] == OUTPUT_REDIRECT)
			open(cmd->token[i + 1], O_CREAT
				| O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
		i++;
	}
	cmd->stdout = dup(STDOUT_FILENO);
	dup2(fd, STDOUT_FILENO);
	close(fd);
}
