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
		perror("open");
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
		perror("open");
		return ;
	}
	cmd->stdout = dup(STDOUT_FILENO);
	dup2(fd, STDOUT_FILENO);
	close(fd);
}
