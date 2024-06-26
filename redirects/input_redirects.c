/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_redirects.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isromero <isromero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 20:01:30 by isromero          #+#    #+#             */
/*   Updated: 2024/03/29 20:01:30 by isromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	input_redirect(t_cmd *cmd)
{
	int	fd;
	int	len;

	len = find_first_input_redirect(cmd);
	fd = open(cmd->token[len + 1], O_RDONLY | S_IRUSR | S_IWUSR);
	if (fd == -1)
		printf("-minishell: %s: No such file or directory\n", cmd->token[len
			+ 1]);
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
		perror("open");
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
